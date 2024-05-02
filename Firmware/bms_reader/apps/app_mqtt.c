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
    cJSON *cellVol = cJSON_CreateArray();
    if (!values || !root || !cellVol)
        return;
    cJSON_AddNumberToObject(root, "timestamp", GetTimestamp());
    cJSON_AddNumberToObject(root, "id", simcom->id);
    cJSON_AddStringToObject(root, "method", "status");
    cJSON_AddBoolToObject(values, "state", bmsInfo.state);
    cJSON_AddStringToObject(values, "bms_id", bmsInfo.id);
    cJSON_AddNumberToObject(values, "cell_quantity", bmsInfo.cells);
    cJSON_AddNumberToObject(values, "voltage", bmsInfo.vol);
    cJSON_AddNumberToObject(values, "soc", bmsInfo.soc);
    cJSON_AddNumberToObject(values, "capacity", bmsInfo.cap);
    cJSON_AddNumberToObject(values, "output_current", bmsInfo.i_out);
    cJSON_AddNumberToObject(values, "temp_env", bmsInfo.envTemp);
    cJSON_AddNumberToObject(values, "temp_board", bmsInfo.boardTemp);
    cJSON_AddNumberToObject(values, "temp_cells", bmsInfo.cellsTemp);
    for (int i = 0; i < MAX_CELL_NUMBER; i++) {
        cJSON_AddItemToArray(cellVol, cJSON_CreateNumber(bmsInfo.cellsVol[i]));
    }
    cJSON_AddItemToObject(values, "cells_voltage", cellVol);
    cJSON_AddItemToObject(root, "values", values);
    char *payload = cJSON_Print(root);
    // logi("%s", payload);
    if (mqtt->connected)
        mqtt->proc->pub(mqtt, mqtt_res_topic, payload, strlen(payload), 0);
    cJSON_Delete(root);
    if (payload != NULL)
        free(payload);    
}