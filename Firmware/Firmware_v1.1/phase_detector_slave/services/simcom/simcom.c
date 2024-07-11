#include "sdk.h"
#include "log.h"
#include "simcom.h"
#include "system.h"
#define SIMCOM_TASK_STACK_SIZE      8192
#define SIMCOM_TASK_PRIORITY        150



static char sc_imei[16] = {0};
static char sc_version[40] = {0};
static simcom_t sc_object = {0};

static int simcom_error_code = 0;

static sMsgQRef gUrcMsgQueue = NULL;
static sTaskRef gUrcProcessTask = NULL;
static uint8_t gUrcTaskStack[SIMCOM_TASK_STACK_SIZE];

static simcom_p simcom_proc = {
};

static void(*sc_sim_event)(int status) = NULL;
static void(*sc_nwk_event)(int status) = NULL;
static void(*sc_ntp_event)(int status) = NULL;
static void(*sc_gps_event)(int src, char* msg) = NULL;
static void(*sc_sms_event)(int flag, void* msg) = NULL;
static void(*sc_call_event)(void) = NULL;
static void(*sc_http_event)(void* data) = NULL;
static void(*sc_mqtt_event)(void* data) = NULL;

static void sTask_UrcProc(void *ptr);

simcom_t* simcom_create(Gpio_t* status) {
    simcom_t* object = NULL;

    do {
        if (gUrcMsgQueue == NULL) {
            if (sAPI_MsgQCreate(&gUrcMsgQueue, "Urc Queue", sizeof(SIM_MSG_T), 30, SC_FIFO) != SC_SUCCESS) {
                loge("Urc Queue create");
                gUrcMsgQueue = NULL;
                break;
            }
        }

        if (gUrcProcessTask == NULL) {
            if (sAPI_TaskCreate(&gUrcProcessTask, gUrcTaskStack, SIMCOM_TASK_STACK_SIZE, SIMCOM_TASK_PRIORITY, \
                (char*)"Urc Task" , sTask_UrcProc, NULL) != SC_SUCCESS) {
                loge("Urc Task create!");
                gUrcProcessTask = NULL;
                break;
            }
        }
        simcom_get_imei(&sc_object);
        if (sc_object.id == 0)
            break;
        object = &sc_object;
        object->sleep = 0;
        object->version = NULL;
        object->status = status;
        object->msgq = gUrcMsgQueue;
        object->proc = &simcom_proc;
        logi("Simcom create success");
    } while (0); 
	return object;
}

const char* simcom_get_imei(simcom_t* object) {
    if ((object->imei == NULL) && (sAPI_SysGetImei(sc_imei) == 0)) {
        sc_imei[15] = 0;
        object->imei = sc_imei;
        simcom_get_id(object);
        logi("Simcom IMEI: %s, ID: %d", sc_imei, (int)object->id);
    }
    return object->imei;
}

const char* simcom_get_version(simcom_t* object) {
    if (object->version == NULL) {
        SIMComVersion simcominfo;
        sAPI_SysGetVersion (&simcominfo);
        umemset(sc_version, 0, 40);
        umemcpy(sc_version, simcominfo.version_tagstr, strlen(simcominfo.version_tagstr));
        object->version = sc_version;
    }
    return object->version;
}

const char* simcom_get_cus_version(simcom_t* object) {
    return NULL;
}

const char* simcom_get_rf_version(simcom_t* object) {
    return NULL;
}

uint32_t simcom_get_id(simcom_t* object) {
    int i;
    if ((object->id == 0) && (object->imei != NULL)) {
        for (i = 6; i < 15; i++)
            object->id = (object->id * 10) + ((uint8_t)(object->imei[i]) - 0x30);
    }
    return object->id;
}

int simcom_get_error(void) {
    return simcom_error_code;
}

void simcom_register_simcard_event(void(*callback)(int status)) {
    sc_sim_event = callback;
}

void simcom_register_nwk_event(void(*callback)(int status)) {
    sc_nwk_event = callback;
}

void simcom_register_gps_event(void(*callback)(int src, char* msg)) {
    sc_gps_event = callback;
}

void simcom_register_ntp_event(void(*callback)(int status)) {
    sc_ntp_event = callback;
}

void simcom_register_sms_event(void(*callback)(int flag, void* data)) {
    sc_sms_event = callback;
}

void simcom_register_call_event(void(*callback)(void)) {
    sc_call_event = callback;
}

void simcom_register_http_event(void(*callback)(void* data)) {
    sc_http_event = callback;
}

void simcom_register_mqtt_event(void(*callback)(void* data)) {
    sc_mqtt_event = callback;
}

static void simcom_msg_remain_proc(SIM_MSG_T msg) {
    if (msg.msg_id == SC_SRV_NTP) {
        if (sc_ntp_event != NULL)
            sc_ntp_event(msg.arg1);
    }
    else if (msg.msg_id == SC_SRV_LBS) {
        if (sc_gps_event != NULL)
            sc_gps_event(255, (char*)msg.arg3);
    }
    else if ((msg.msg_id == SC_SRV_HTTPS) || (msg.msg_id == SRV_SELF)){
        if (sc_http_event != NULL)
            sc_http_event((void*)msg.arg3);
        ufree(((SChttpApiTrans*)(msg.arg3))->data);
    }
    else if((msg.msg_id == SC_SRV_MQTT) && (msg.arg3 != NULL)) {
        if (sc_mqtt_event != NULL)
            sc_mqtt_event((void*)msg.arg3);
        ufree(((SCmqttData*)(msg.arg3))->topic_P);
        ufree(((SCmqttData*)(msg.arg3))->payload_P);
    }
    else {
        logw("scMsg: id = %d, arg1 = %d", msg.msg_id, msg.arg1);
    }
}

static void sTask_UrcProc(void *ptr) {
    SIM_MSG_T msg = {0};

    if (sAPI_UrcRefRegister(gUrcMsgQueue, SC_MODULE_ALL) != 0) {
        sAPI_MsgQDelete(gUrcMsgQueue);
        loge("Urc Ref register");
        return;
    }

    while (1)
    {
        if (sAPI_MsgQRecv(gUrcMsgQueue, &msg, SC_SUSPEND) != SC_SUCCESS)
            continue;

        if (SRV_URC != msg.msg_id) {
            simcom_msg_remain_proc(msg);
            ufree(msg.arg3);
            msg.arg3 = NULL;
            continue;
        }

        switch (msg.arg1)
        {
            case SC_URC_SMS_MASK:
                if (sc_sms_event != NULL)
                    sc_sms_event((int)msg.arg2, (void*)msg.arg3);
                break;

            case SC_URC_NETSTATUE_MASK:
                if (sc_nwk_event != NULL)
                    sc_nwk_event(msg.arg2);
                break;

            case SC_URC_GNSS_MASK:
                if (sc_gps_event != NULL)
                    sc_gps_event(msg.arg2, (char*)msg.arg3);
                break;

            case SC_URC_CALL_MASK:
                if (sc_call_event != NULL)
                    sc_call_event();
                break;
            case SC_URC_SIM_MASK:
                if (sc_sim_event != NULL)
                    sc_sim_event(msg.arg2);
                break;
                    

            default:
                // logi("Urc module:%d, urc:%d", msg.arg1, msg.arg2);
                break;
        }

        ufree(msg.arg3);
        msg.arg3 = NULL;
    }

    sAPI_UrcRefRelease(gUrcMsgQueue);
    sAPI_MsgQDelete(gUrcMsgQueue);
}