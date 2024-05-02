#ifndef __NETWORK_H__
#define __NETWORK_H__

#define NWK_CFUN_0      0 // minimum mode
#define NWK_CFUN_1      1 // normal mode
#define NWK_CFUN_4      4 // flight mode

typedef struct network_t network_t;

typedef struct network_p {
	int(*nconnect)(network_t*, const char*, const char*, const char*);
	int(*disconnect)(network_t*);
	int(*rssi)(network_t*);
} network_p;

struct network_t {
	const network_p* proc;
	simcom_t* simcom;
	bool connected;
	int cfun;
    int error;
	char ip[32];
};

network_t* network_create(simcom_t* simcom);
int network_disconnect(network_t* object);
int network_connect(network_t* object, const char* apn, const char* user, const char* pwd);
int network_get_rssi(network_t* object);
int network_get_cfun(network_t* object);
int network_set_cfun(network_t* object, int cfun);
uint32_t network_get_ip(network_t* object);
void network_register_ntp_event(void(*callback)(int status));

#endif