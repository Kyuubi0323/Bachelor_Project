#ifndef __SMS_H__
#define __SMS_H__

typedef struct sms_t sms_t;

typedef struct sms_p {
	void(*dispose)(sms_t*);
	int(*send_text)(sms_t*, const char*, const char*);
	char*(*read_text)(sms_t*, int, uint8_t*, int);
} sms_p;

struct sms_t {
	const sms_p* proc;
	simcom_t* simcom;
};

sms_t* sms_create(simcom_t* simcom);
void sms_dispose(sms_t* sms);
char* sms_read_text(sms_t* sms, int index, uint8_t* buffer, int size);
int sms_send_text(sms_t* sms, const char* number, const char* message);
int sms_delete(sms_t* sms, int index);
void sms_register_msg_event(void(*callback)(char* number, char* msg));

int sms_get_error(void);

#endif