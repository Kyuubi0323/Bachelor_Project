#include "sdk.h"
#include "log.h"
#include "utils.h"
#include "simcom.h"
#include "sms.h"

#define SMS_TASK_STACK_SIZE		4096
#define SMS_RECV_TIME_OUT 		1000

static sms_p sms_proc = {
	sms_dispose,
	sms_send_text,
	sms_read_text
};

static sMsgQRef smsMsgQ = NULL;
static sMsgQRef smsMsgQRes = NULL;
static sTaskRef smsTask = NULL;
static uint8_t smsTaskStack[SMS_TASK_STACK_SIZE];

static int sms_error_code = 0;
static sms_t sms_object = {0};
static void(*sms_msg_event)(char* number, char* msg) = NULL;

static void sms_on_urc_proc(int flag, void* data);
static void sTask_SmsProc(void *ptr);

sms_t* sms_create(simcom_t* simcom) {
    sms_t* object = NULL;
    do {
		if (smsMsgQ == NULL) {
			sms_error_code = sAPI_MsgQCreate(&smsMsgQ, "Sms Queue", sizeof(SIM_MSG_T), 4, SC_FIFO);
			if (sms_error_code != SC_SUCCESS)
				break;
		}

		if (smsMsgQRes == NULL) {
			sms_error_code = sAPI_MsgQCreate(&smsMsgQRes, "Sms Queue Res", sizeof(SIM_MSG_T), 4, SC_FIFO);
			if (sms_error_code != SC_SUCCESS) {
				sAPI_MsgQDelete(smsMsgQ);
				smsMsgQ = NULL;
				break;
			}
		}

		if (smsTask == NULL) {
			sms_error_code = sAPI_TaskCreate(&smsTask, smsTaskStack, SMS_TASK_STACK_SIZE, 150, (char*)"Sms Task" , sTask_SmsProc, NULL);
			if (sms_error_code != SC_SUCCESS) {
				sAPI_MsgQDelete(smsMsgQRes);
				sAPI_MsgQDelete(smsMsgQ);
				smsMsgQRes = NULL;
				smsMsgQ = NULL;
				break;
			}
		}

		object = &sms_object;
		object->proc = &sms_proc;
		object->simcom = simcom;
		simcom_register_sms_event(sms_on_urc_proc);
    } while (0);
	return object;
}

void sms_dispose(sms_t* sms) {
	if (smsTask != NULL) {
		sAPI_TaskDelete(smsTask);
		smsTask = NULL;
	}
	if (smsMsgQ != NULL) {
		sAPI_MsgQDelete(smsMsgQ);
		smsMsgQ = NULL;
	}
	if (smsMsgQRes != NULL) {
		sAPI_MsgQDelete(smsMsgQRes);
		smsMsgQRes = NULL;
	}
}

int sms_send_text(sms_t* sms, const char* number_phone, const char* message) {
	SIM_MSG_T msg;

	do {
		sms_error_code = -1;
		if (sms == NULL)
			break;
		
		sms_error_code = sAPI_SmsSendMsg(1, (UINT8*)message, strlen(message), (UINT8*)number_phone, smsMsgQRes);
		if (sms_error_code != SC_SMS_SUCESS)
			break;
		
		sms_error_code = sAPI_MsgQRecv(smsMsgQRes, &msg, SMS_RECV_TIME_OUT);
		if (sms_error_code != SC_SMS_SUCESS)
			break;
		
		logi("Sms send text [%s]: %s -> %s", number_phone, message, (const char*)(msg.arg3));
		ufree(msg.arg3);
	} while (0);
	return sms_error_code;
}

char* sms_read_text(sms_t* sms, int index, uint8_t* buffer, int size) {
	return NULL;
}

int sms_delete(sms_t* sms, int index) {
	return -1;
}

void sms_register_msg_event(void(*callback)(char* number, char* msg)) {
	sms_msg_event = callback;
}

int sms_get_error(void) {
    return sms_error_code;
}

static void sms_read_message_proc(int index) {
	SIM_MSG_T msg;
	do {
		sms_error_code = sAPI_SmsReadMsg(1, index, smsMsgQRes);
		if (sms_error_code != SC_SMS_SUCESS)
			break;
		
		memset(&msg, 0, sizeof(msg));
        sAPI_MsgQRecv(smsMsgQRes, &msg, SMS_RECV_TIME_OUT);

		sms_error_code = msg.arg2;
		if (sms_error_code != SC_SMS_SUCESS) {
			logw("Sms read text [%d]: id: %d, code: %d, %s", index, msg.arg1, sms_error_code, (const char*)(msg.arg3));
			ufree(msg.arg3);
			break;
		}
		
		char* content = NULL;
		char number[32] = {0};

		content = strstr((const char*)(msg.arg3), "\"REC UNREAD\",");
		if (content != NULL) {
			content += 13;
			split_string(content, number, 0, strlen((const char*)content), 32, ',');
			content = strstr((const char*)content, "\r\n");
		}
		if ((content != NULL) && (sms_msg_event != NULL)) {
			content += 2;
			sms_msg_event(number, content);
		}
		else {
			logi("Sms read text [%d]: %s", index, (const char*)(msg.arg3));
		}
		ufree(msg.arg3);

		sms_error_code = sAPI_SmsDelOneMsg(index, smsMsgQRes);
		if (sms_error_code == SC_SMS_SUCESS) {
			sms_error_code = sAPI_MsgQRecv(smsMsgQRes, &msg, SMS_RECV_TIME_OUT);
			logi("Sms delete [%d] -> %d", index, sms_error_code);
			ufree(msg.arg3);
		}
	} while (0);
}

static void sms_full_message_proc(void) {
	SIM_MSG_T msg;
	if (sAPI_SmsDelAllMsg(smsMsgQRes) == SC_SMS_SUCESS) {
		sAPI_MsgQRecv(smsMsgQRes, &msg, SMS_RECV_TIME_OUT);
		logi("Sms delete all -> %d", msg.arg2);
		ufree(msg.arg3);
	}
}

static void sms_on_urc_proc(int flag, void* data) {
	if (smsMsgQ != NULL)
	{
		SIM_MSG_T msg = {MSG_ID_SMS, flag, 0, NULL};
		if (data != NULL) {
			int size = strlen((const char*)data);
			msg.arg3 = ualloc(size + 1);
			umemcpy(msg.arg3, data, size);
			((char*)(msg.arg3))[size] = 0;
		}
		sAPI_MsgQSend(smsMsgQ, &msg);
	}
}

static void sTask_SmsProc(void *ptr) {
	char* temp;
	SIM_MSG_T msg;

	while (1)
	{
		if (sAPI_MsgQRecv(smsMsgQ, &msg, SC_SUSPEND) != SC_SUCCESS)
            continue;

        if (MSG_ID_SMS != msg.msg_id) {
			if (msg.arg3 != NULL) {
				ufree(msg.arg3);
            	msg.arg3 = NULL;
			}
            continue;
        }

		switch (msg.arg1)
		{
			case SC_URC_NEW_MSG_IND:
				temp = strstr((const char*)(msg.arg3), "\"SM\",");
				if (temp != NULL) {
					temp += 5;
					sms_read_message_proc(u_atoi(temp));
				}
				break;

			case SC_URC_FLASH_MSG:
				logw("Sms flash");
				break;
			
			case SC_URC_STATUS_REPORT:
				logw("Sms report -> %s", (const char*)(msg.arg3));
				break;

			case SC_URC_SMSDWON:
				logw("SMS DONE");
				break;

			case SC_URC_SMSFULL:
				sms_full_message_proc();
				break;

			default:
				logw("Sms unknow event");
				break;
		}

		if (msg.arg3 != NULL) {
			ufree(msg.arg3);
            msg.arg3 = NULL;
		}
	}
}