#ifndef __MQTT_H__
#define __MQTT_H__

typedef struct mqtt_t mqtt_t;

typedef struct mqtt_p {
    void(*dispose)(mqtt_t*);
    int(*uconnect)(mqtt_t*, const char*, const char*, const char*);
    int(*udisconnect)(mqtt_t*);
    // int(*enable_ssl)(mqtt_t*, const char*, int);
    int(*pub)(mqtt_t*, const char*, char*, int, int);
    int(*sub)(mqtt_t*, const char*, int);
} mqtt_p;

struct mqtt_t {
    const mqtt_p* proc;
    simcom_t* simcom;
    bool connected;
    int error;
};

mqtt_t* mqtt_create(simcom_t* simcom);
void mqtt_dispose(mqtt_t* mqtt);
int mqtt_connect(mqtt_t* mqtt, const char* server, const char* user, const char* pwd);
int mqtt_disconnect(mqtt_t* mqtt);
// int mqtt_enable_ssl(mqtt_t* mqtt, const char* certfile, int ssl_ver);
int mqtt_publish(mqtt_t* mqtt, const char* topic, char* message, int size, int qos);
int mqtt_subscribe(mqtt_t* mqtt, const char* topic, int qos);
int mqtt_lwt(mqtt_t* mqtt, const char* topic);
void mqtt_register_data_event(void(*callback)(void* data, int size));
void mqtt_register_lost_event(void(*callback)(int index, int cause));

int mqtt_get_error(void);

#endif