#include "sdk.h"
#include "log.h"
#include "app_param.h"
#include "simcom.h"
#include "network.h"
#include "simcard.h"
#include "mqtt.h"
#include "cJSON.h"

#define DEV_INIT_COUNT 5
#define SIM_INIT_COUNT 5
#define NWK_INIT_COUNT 20
#define MQT_INIT_COUNT 5

#define LED_BLINK_ON_INIT 100       // 500 ms
#define LED_BLINK_OFF_INIT 100      // 500 ms
#define LED_BLINK_ON_SIM_ERROR 40   // 200 ms
#define LED_BLINK_OFF_SIM_ERROR 40  // 200 ms
#define LED_BLINK_ON_NWK_ERROR 200  // 1000 ms
#define LED_BLINK_OFF_NWK_ERROR 200 // 1000 ms

#define MQTT_HOST       "tcp://38.105.209.36:1883"
#define MQTT_USER       "unused"
#define MQTT_PASS       "unused"
#define MQTT_REF_TOPIC  "gateway/52438020/response"


simcom_t *simcom = NULL;
network_t *network = NULL;
simcard_t *simcard = NULL;
mqtt_t *mqtt = NULL;

sMsgQRef appSimcom_msgq = NULL;
static sTimerRef appSimcom_timer = NULL;
static sTaskRef appSimcom_task = NULL;
static UINT8 appSimcom_stack[8192];

static int dev_init_count = DEV_INIT_COUNT;
static int sim_init_count = SIM_INIT_COUNT;
static int nwk_init_count = NWK_INIT_COUNT;
static int mqt_init_count = MQT_INIT_COUNT;
//0915423686
char mqtt_req_topic[60];
char mqtt_res_topic[60];

// Function blink led
extern Gpio_t ledStatus;
static int ledOntime = 0;
static int ledOfftime = 0;
static sTimerRef ledTimer = NULL;

extern int uartSend(UINT8 *data, UINT32 size);
extern UINT8 Cal_CheckSumU8(UINT8 *buf, UINT16 len);

extern void updateExStatus(void);
extern void updateStatus(void);
static void onLedBlink(UINT32 value)
{
    int initime = (value != 0) ? ledOfftime : ledOntime;
    Gpio_setState(simcom->status, !value);
    sAPI_TimerStart(ledTimer, initime, 0, onLedBlink, !value);
}

static void blinkLedStatus(int ontime, int offtime)
{
    ledOntime = ontime;
    ledOfftime = offtime;
    sAPI_TimerStop(ledTimer);
    Gpio_setState(simcom->status, 1);
    sAPI_TimerStart(ledTimer, ledOntime, 0, onLedBlink, 1);
}

static void setLedStatus(bool state)
{
    sAPI_TimerStop(ledTimer);
    Gpio_setState(simcom->status, state);
}

static void initSimcard(void)
{
    simcard = simcard_create(simcom);
    if (simcard == NULL)
    {
        sim_init_count--;
        if (sim_init_count > 0)
            msleep(1000);
    }
    if (sim_init_count == 0)
    {
        loge("Simcard error code -> %d", simcard_get_error());
        blinkLedStatus(LED_BLINK_ON_SIM_ERROR, LED_BLINK_OFF_SIM_ERROR);
        // syslog(8, 5, simcard_get_error());
    }
}

static inline void initNetwork(void)
{
    network->proc->nconnect(network, "v-internet", NULL, NULL);
    if (!network->connected)
    {
        nwk_init_count--;
        if (nwk_init_count > 0)
            msleep(1000);
    }

    if (nwk_init_count == 0)
    {
        loge("Network error code -> %d", network->error);
        blinkLedStatus(LED_BLINK_ON_NWK_ERROR, LED_BLINK_OFF_NWK_ERROR);
        // syslog(8, 6, network->error);
    }
}

void sendRefDataToMCU(uint32_t ts, uint16_t l1, uint16_t l2, uint16_t l3, BOOL seq) {
    // uint8_t data[32] = {0};
	// data[0] = 0xAA;
	// data[1] = 0x01;
	// data[2] = 0x00;
	// data[3] = 0x00;
	// data[4] = 11;
	// *(uint32_t*)&data[5] = ts;
	// *(uint16_t*)&data[9] = l1;
	// *(uint16_t*)&data[11] = l2;
	// *(uint16_t*)&data[13] = l3;
	// data[15] = seq;
	// data[16] = Cal_CheckSumU8(data, 16);
	// data[17] = 0x55;
    char Data[64] = {0};
    usprintf(Data,"$,%ld,%d,%d,%d,%s,\r\n", ts, l1, l2, l3, (seq) ? "ACB" : "ABC");
    uartSend((uint8_t*)Data, strlen(Data));
}

void mqttDataCallback(void *data, int size) {
    // logi("Mqtt-> payload[%d]: %s", size, (uint8_t*)data);
    cJSON *root = cJSON_Parse((char*)data);
    cJSON *value = cJSON_GetObjectItem(root , "values");
    if (!value) {
        return;
    }
    cJSON *ts = cJSON_GetObjectItem(value, "ts");
    cJSON *l1 = cJSON_GetObjectItem(value, "l1");
    cJSON *l2 = cJSON_GetObjectItem(value, "l2");
    cJSON *l3 = cJSON_GetObjectItem(value, "l3");
    cJSON *seq = cJSON_GetObjectItem(value, "sequence");
    if (!ts || !l1 || !l2 || !l3|| !seq)
        return;
    logi("Timestamp %ld, Ref Phase A %d, Sequence %s", (uint32_t)ts->valuedouble, l1->valueint, (seq->valueint) ? "ACB" : "ABC");
    sendRefDataToMCU((uint32_t)ts->valuedouble, l1->valueint, l2->valueint, l3->valueint, seq->valueint);
    cJSON_Delete(root);
}

static inline void initMqtt(void) {
    if (mqtt == NULL) {
        mqtt = mqtt_create(simcom);
        if (mqtt == NULL) {
            mqt_init_count--;
            mqtt_dispose(NULL);
        }
        else
            mqt_init_count = MQT_INIT_COUNT;

        if (mqt_init_count == 0) {
            loge("Mqtt error code -> %d", mqtt_get_error());
            setLedStatus(0);
        }
    }
    mqtt_register_data_event(mqttDataCallback);
    if ((mqtt != NULL) && (mqtt->connected == false)) {
        mqtt->proc->uconnect(mqtt, (const char*)MQTT_HOST, (const char*)MQTT_PASS, (const char*)MQTT_PASS);
        if (mqtt->connected) {
            mqtt->proc->sub(mqtt, MQTT_REF_TOPIC, 0);
            updateExStatus();
            setLedStatus(1);
        }
        else
            mqt_init_count--;

        if (mqt_init_count == 0) {
            loge("Mqtt error code -> %d", mqtt_get_error());
            setLedStatus(0);
        }
    }
}

static void onNwkEvent(int status)
{
    if (status == SC_URC_NETACTED) {
        logi("Network status -> NETACTED");
    }
    else if (status == SC_URC_NETDIS) {
        logi("Network status -> NETDIS");
    }       
    else if (status == SC_URC_PDP_ACTIVE) {
        logi("Network status -> PDP_ACTIVE");
    }  
    else if (status == SC_URC_PDP_DEACT) {
        logi("Network status -> PDP_DEACT");
    }       
}

static void simcardEvent(int status)
{
    if (status == SC_URC_CPIN_READY) {
        logi("Simcard status -> READY");
    }
    else if (status == SC_URC_CPIN_REMOVED) {
        loge("Simcard status -> REMOVED");
    }
}

static inline void reinitFuncion(void) {
    if (mqtt != NULL) {
        mqtt_dispose(NULL);
        mqtt = NULL;
    }
    simcard = NULL;
    network->connected = false;
    sim_init_count = SIM_INIT_COUNT;
    nwk_init_count = NWK_INIT_COUNT;
    mqt_init_count = MQT_INIT_COUNT;
}
static void onNetLost(int index, int cause) {
    reinitFuncion();
    logw("Mqtt lost connection");
}



static void appSimcomTask(void *argv)
{
    simcom = simcom_create(&ledStatus);
    network = network_create(simcom);
    usprintf(mqtt_req_topic, "gateway/%d/request", simcom->id);
    usprintf(mqtt_res_topic, "gateway/%d/response", simcom->id);

    blinkLedStatus(LED_BLINK_ON_INIT, LED_BLINK_OFF_INIT);
    simcom_register_nwk_event(onNwkEvent);
    simcom_register_simcard_event(simcardEvent);
    mqtt_register_lost_event(onNetLost);
    UINT32 updateInfo = sysTickCnt();
    SIM_MSG_T msg = {0};
    while (1)
    {
        if ((simcard == NULL) && (sim_init_count > 0))
            initSimcard();
        if ((simcard != NULL) && !network->connected && (nwk_init_count > 0))
            initNetwork();
        if ((network->connected && ((mqtt == NULL) || ((mqtt != NULL) && (!mqtt->connected))) && (mqt_init_count > 0))) {
            initMqtt();
        }
        if ((!nwk_init_count || !mqt_init_count)) {
            logw("Reinitiazize all function");
            reinitFuncion();
        }
        if (sAPI_MsgQRecv(appSimcom_msgq, &msg, SC_NO_SUSPEND) == SC_SUCCESS){
            if (msg.msg_id == 1 && mqtt != NULL)
                updateStatus();
        }
        msleep(100);
    }
}

void initAppSimcom(void)
{
    if (sAPI_TimerCreate(&appSimcom_timer) != SC_SUCCESS)
    {
        loge("appSimcom_timer create");
    }
    if (sAPI_TimerCreate(&ledTimer) != SC_SUCCESS)
    {
        loge("ledTimer create");
    }
    if (sAPI_MsgQCreate(&appSimcom_msgq, "appSimcom_msgq", sizeof(SIM_MSG_T), 8, SC_FIFO) != SC_SUCCESS)
    {
        loge("appSimcom_msgq create");
    }
    if (sAPI_TaskCreate(&appSimcom_task, appSimcom_stack, 8192, 150, "appSimcom_task", appSimcomTask, NULL) != SC_SUCCESS)
    {
        loge("appSimcom_task create");
    }
}





