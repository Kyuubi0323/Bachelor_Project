#include "sdk.h"
#include "log.h"
#include "simcom.h"
#include "mqtt.h"
#include "fs.h"


#define MQTT_CERT \
"-----BEGIN CERTIFICATE-----\n\
MIIEBzCCAu+gAwIBAgIUCPAWewRYT5FcKbwbxEMjY8aMNDUwDQYJKoZIhvcNAQEL\n\
BQAwgZExCzAJBgNVBAYTAlZOMQ4wDAYDVQQIDAVIYW5vaTEOMAwGA1UEBwwFSGFu\n\
b2kxETAPBgNVBAoMCERpZ290ZWNoMREwDwYDVQQLDAhEaWdvdGVjaDEXMBUGA1UE\n\
AwwOKi5kaWdvdGVjaC5uZXQxIzAhBgkqhkiG9w0BCQEWFGNvbnRhY3RAZGlnb3Rl\n\
Y2gubmV0MCAXDTIyMDExNDA1NTAwNloYDzIwNzYxMjMxMDU1MDA2WjCBkTELMAkG\n\
A1UEBhMCVk4xDjAMBgNVBAgMBUhhbm9pMQ4wDAYDVQQHDAVIYW5vaTERMA8GA1UE\n\
CgwIRGlnb3RlY2gxETAPBgNVBAsMCERpZ290ZWNoMRcwFQYDVQQDDA4qLmRpZ290\n\
ZWNoLm5ldDEjMCEGCSqGSIb3DQEJARYUY29udGFjdEBkaWdvdGVjaC5uZXQwggEi\n\
MA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDX/oU3/rVbOk7fgGdTz63tyQvi\n\
xFunDbmELBu3JuAq9P5djmjbNo7N/QhfyaORlmEAV/I65QdYL40eRY4kT10uJR27\n\
X65kFly4B/G+IBkuVe6whC+Vnhau4K+YY/5C+P8Fz5tUY+E1zgsdHdLTz+H3iut1\n\
tHZnQ9OKLyCENvQFJcj4vtqt/hFXr/DxfP1H+IjNfNUpCNUS1Wkif1dItWOg6I5D\n\
9gNpAxMH100rvb80PLTrReUhZejTJc4Ns+3sCowMSHAOC01Fa4EBLmbxIb0l0e64\n\
X1BHGBlA7qbmm6oqh57bviv6W4wF2EYOUM6FTs9tSczfU2jg6LF+o1zVhHvNAgMB\n\
AAGjUzBRMB0GA1UdDgQWBBR/4sv6sz1cH5vLrVGbFXAc8LUc2zAfBgNVHSMEGDAW\n\
gBR/4sv6sz1cH5vLrVGbFXAc8LUc2zAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3\n\
DQEBCwUAA4IBAQAgzDZ5pfahLWEUt6QD9IATHCPLmOTR9O+zreknsMALg0dU8Gsz\n\
WPbp9G11Uo7l/rP30cF8oq4it9bjskjwSb0wWcxUS+wUPAZp8HGgp7so84bgDHeC\n\
SZDyvO+Xw4TnNqhQxvukuYTlvTDe1CCuKaP/R4FqJL9RJFrptCEwrJNpGOWjH5sF\n\
amxn2CZojvrC9C58v1QbadJLeUXdMgyDHW7bURurxpdrKi/Q2wQcPeXxexC2JF7z\n\
v74Cpv8pZWCOEBaLqCjKTOPSBdUB1L6KcrLNpUAtVNn2T8UuU+jlBMErgF1RLbg6\n\
4XO0jqUek0e3zhVlgzleOzgS4UUYscBU2NqW\n\
-----END CERTIFICATE-----"


#define USER_MQTT_RESULT_TASK       (SC_MQTT_RESULT_MAX + 1)
#define USER_MQTT_RESULT_QUEUE      (SC_MQTT_RESULT_MAX + 2)

#define MQTT_TASK_STACK_SIZE        4096
#define MQTT_TASK_PRIORITY          150

static char *TAG = "MQTT";

static mqtt_p mqtt_proc = {
    mqtt_dispose,
    mqtt_connect,
    mqtt_disconnect,
    // mqtt_enable_ssl,
    mqtt_publish,
    mqtt_subscribe
};
static mqtt_t mqtt_object = {0};
static int mqtt_error_code = 0;
static void(*mqtt_events)(void* data, int size) = NULL;

//static int mqtt_enable_ssl(void);

static void mqtt_process_data(void* message);

mqtt_t* mqtt_create(simcom_t* simcom) {
    mqtt_t* object = NULL;

    simcom_register_mqtt_event(mqtt_process_data);

    do {
        mqtt_error_code = sAPI_MqttStart(-1);
        if (mqtt_error_code != SC_MQTT_RESULT_SUCCESS)
            break;
        logi("Mqtt start");

        mqtt_error_code = sAPI_MqttAccq(SC_MQTT_OP_SET, NULL, 0, simcom->imei, 0, simcom->msgq);
        if (mqtt_error_code != SC_MQTT_RESULT_SUCCESS)
            break;
        logi("Mqtt acquire");

        mqtt_error_code = sAPI_MqttCfg(0, NULL, 0, 0, 0);
        if (mqtt_error_code != SC_MQTT_RESULT_SUCCESS)
            break;
        logi("Mqtt config");
        object = &mqtt_object;
        object->simcom = simcom;
        object->proc = &mqtt_proc;
        object->connected = false;
        object->error = 0;
    } while (0);
    return object;
}

void mqtt_dispose(mqtt_t* mqtt) {
    mqtt_error_code = sAPI_MqttDisConnect(SC_MQTT_OP_SET, NULL, 0, 60);
    mqtt_error_code += sAPI_MqttRel(0);
    mqtt_error_code += sAPI_MqttStop();
    logi("Mqtt dispose: %d", mqtt_error_code);
}

int mqtt_connect(mqtt_t* mqtt, const char* server, const char* user, const char* pwd) {
    mqtt_error_code = sAPI_MqttConnect(SC_MQTT_OP_SET, NULL, 0, (char*)server, 60, 0, (char*)user, (char*)pwd);
    if (mqtt_error_code == SC_MQTT_RESULT_SUCCESS)
        mqtt->connected = true;
    logi("Mqtt connect: %d", mqtt_error_code);
    return mqtt_error_code;
}

int mqtt_disconnect(mqtt_t* mqtt) {
    mqtt->connected = false;
    mqtt_error_code = sAPI_MqttDisConnect(SC_MQTT_OP_SET, NULL, 0, 60);
    logi("Mqtt disconnect: %d", mqtt_error_code);
    return mqtt_error_code;
}

int mqtt_publish(mqtt_t* mqtt, const char* topic, char* message, int size, int qos) {
    do {
        mqtt_error_code = sAPI_MqttTopic(0, (char*)topic, ustrlen(topic));
        if (mqtt_error_code != SC_MQTT_RESULT_SUCCESS)
            break;
        
        mqtt_error_code = sAPI_MqttPayload(0, message, size);
        if (mqtt_error_code != SC_MQTT_RESULT_SUCCESS)
            break;
        
        mqtt_error_code = sAPI_MqttPub(0, 0, 60, 0, 0);
    } while (0);

    if (mqtt_error_code != 0)
        mqtt->error++;

    logi("Mqtt pub: %d -> (%s) %s", mqtt_error_code, topic, (const char*)message);
    return mqtt_error_code;
}

int mqtt_subscribe(mqtt_t* mqtt, const char* topic, int qos) {
    mqtt_error_code = sAPI_MqttSub(0, (char*)topic, ustrlen(topic), qos, 0);
    logi("Mqtt sub: %d -> %s", mqtt_error_code, topic);
    return mqtt_error_code;
}

void mqtt_register_data_event(void(*callback)(void* data, int size)) {
    mqtt_events = callback;
}

void mqtt_register_lost_event(void(*callback)(int index, int cause)) {
    if (callback != NULL)
        sAPI_MqttConnLostCb(callback);
}

int mqtt_get_error(void) {
    return mqtt_error_code;
}

// static void mqtt_lost_event(int index, int cause) {
//     logi("Mqtt lost event %d -> cause %d", index, cause);
// }

static void mqtt_process_data(void* message) {
    SCmqttData* data = (SCmqttData*)(message);

    // logi("Mqtt-> index: [%d]; topic: [%s]", data->client_index, data->topic_P);
    // logi("Mqtt-> payload: %s", data->payload_P);

    if (mqtt_object.connected && (mqtt_events != NULL))
        mqtt_events((void*)data->payload_P, data->payload_len);
}

// void insert_cert()
// {
//     void* fd = NULL;

//     fd = fs_open("c:/User/digo_ca.crt", FS_READ_ONLY);
//     if(fd == NULL)
//     {
//         fd = fs_open("c:/User/digo_ca.crt", FS_WRITE_ONLY);
//         fs_write(fd, MQTT_CERT, strlen(MQTT_CERT));

//     }

//     // loge("Write certificate successfully !!!");
//     fs_close(fd);


// }

// static int mqtt_enable_ssl(void)
// {
//     void* fd = NULL;
//     // int reval = -1;
//     int result  = -1;
//     char cert[4096] = {0};
//     // simcom_t* simcom = mqtt->simcom;

//     // fs_delete("c:/digo_ca.crt");
//     // fs_delete("c:/digo_ca.pem");

//     do
//     {
        

//         fd = fs_open("c:/digo_ca.pem", FS_WRITE_ONLY);
//         if(fd != NULL)
//         {
//             if(fs_write(fd, MQTT_CERT, strlen(MQTT_CERT)) < 0)
//             {
//                 fs_close(fd);
//                 loge("Write FS fail !!!");

//                 return -2;
//             }
//             else{
//                 loge("Write certificate successfully !!!");
//                 fs_close(fd);
//             }
//         }
//         else{
//             loge("Open Cert Fail !!!");
//             return -1;
//         }

        

//         // fd = fs_open("c:/digo_ca.pem", FS_READ_ONLY);
//         // fs_read(fd, (void*)cert, 4096);
//         // logi("Cert was inserted : %s", cert);
//         // fs_close(fd);

//         result = sAPI_SslSetContextIdMsg("sslversion",0,"4");
//         logi("sslver result : %d", result);

//         result = sAPI_SslSetContextIdMsg("authmode",0,"1");
//         logi("authmode result : %d", result);

//         result = sAPI_SslSetContextIdMsg("cacert",0,"c:/digo_ca.pem");
//         logi("cacert result : %d", result);

//     }while (0);

//     return 0;

// }