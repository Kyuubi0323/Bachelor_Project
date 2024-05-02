#ifndef __SIMCOM_H__
#define __SIMCOM_H__

#define MSG_ID_APP          0x1000		
#define MSG_ID_TRACK        0x1001		
#define MSG_ID_SMS          0x1002

typedef struct simcom_t simcom_t;

typedef struct simcom_p {
} simcom_p;

struct simcom_t {
	const simcom_p* proc;
	Gpio_t* status;
	char* version;
	char* imei;
	void* msgq;
	int sleep;
	uint32_t id;
};

simcom_t* simcom_create(Gpio_t* status);
const char* simcom_get_imei(simcom_t* object);
const char* simcom_get_version(simcom_t* object);
const char* simcom_get_cus_version(simcom_t* object);
const char* simcom_get_rf_version(simcom_t* object);
uint32_t simcom_get_id(simcom_t* object);

int simcom_get_error(void);

void simcom_register_simcard_event(void(*callback)(int status));
void simcom_register_nwk_event(void(*callback)(int status));
void simcom_register_gps_event(void(*callback)(int src, char* msg));
void simcom_register_ntp_event(void(*callback)(int status));
void simcom_register_sms_event(void(*callback)(int flag, void* data));
void simcom_register_call_event(void(*callback)(void));
void simcom_register_http_event(void(*callback)(void* data));
void simcom_register_mqtt_event(void(*callback)(void* data));

#endif