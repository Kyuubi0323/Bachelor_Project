#include "sdk.h"
#include "simcom.h"
#include "network.h"
#include "mqtt.h"
#include "cJSON.h"
#include "log.h"
#include "types.h"
extern simcom_t *simcom;
extern network_t *network;
extern mqtt_t *mqtt;

extern char mqtt_res_topic[];

unsigned long GetTimestamp() {
    return sAPI_Time(NULL);
}
void updateExStatus(void) {
    cJSON *values = cJSON_CreateObject();
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "timestamp", GetTimestamp());
    cJSON_AddNumberToObject(root, "id", simcom->id);
    cJSON_AddStringToObject(root, "method", "extended_status");
    cJSON_AddStringToObject(values, "firmwareVersion", "1.0.1");
    cJSON_AddStringToObject(values, "hardwareVersion", "1.0.0");
    cJSON_AddStringToObject(values, "ip", network->ip);
    cJSON_AddStringToObject(values, "resetReason", "power on reset");
    cJSON_AddItemToObject(root, "values", values);
    char *payload = cJSON_Print(root);
    // logi("%s", payload);
    if (mqtt->connected)
        mqtt->proc->pub(mqtt, mqtt_res_topic, payload, strlen(payload), 0);
    cJSON_Delete(root);
    if (payload != NULL)
        free(payload);
}

void updateStatus(void) {
    cJSON *values = cJSON_CreateObject();
    cJSON *root = cJSON_CreateObject();
    if (!values || !root)
        return;
    cJSON_AddNumberToObject(root, "timestamp", GetTimestamp());
    cJSON_AddNumberToObject(root, "id", simcom->id);
    cJSON_AddStringToObject(root, "method", "status");
    cJSON_AddNumberToObject(values, "ts", phaseData.ts);
    char phaseret[2] = {phaseData.ret, '\0'};
    cJSON_AddStringToObject(values, "phase", phaseret);
    //cJSON_AddStringToObject(values, "phase", phaseData.ret);
    cJSON_AddItemToObject(root, "values", values);
    char *payload = cJSON_Print(root);
    if (mqtt->connected)
        mqtt->proc->pub(mqtt, mqtt_res_topic, payload, strlen(payload), 0);
    cJSON_Delete(root);
    if (payload != NULL)
        free(payload);    
}