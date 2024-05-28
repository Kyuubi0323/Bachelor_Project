#include "sdk.h"
#include "log.h"
#include "app_param.h"

#define LED_POWER_PIN SC_MODULE_GPIO_10
#define LED_STATUS_PIN SC_MODULE_GPIO_10

#define USER_MAIN_STACK_SIZE 8192 // 8k

typedef struct
{
    void (*app_entry)(void *arg);
} appRegItem_t;
#define _appRegTable_attr_ __attribute__((unused, section(".appRegTable")))

Gpio_t ledStatus = {0};

static sMsgQRef appMain_msgq;
static sTaskRef appMain_task;
static sTimerRef appMain_timer;

static UINT8 userMainStack[USER_MAIN_STACK_SIZE];

extern void initAppUart(void);
extern void initAppSimcom(void);
void uart_config(void)
{
    SCuartConfiguration uart2Config;
    uart2Config.BaudRate = SC_UART_BAUD_115200;
    uart2Config.DataBits = SC_UART_WORD_LEN_8;
    uart2Config.ParityBit = SC_UART_NO_PARITY_BITS;
    uart2Config.StopBits = SC_UART_ONE_STOP_BIT;
    if (sAPI_UartSetConfig(LOG_UART, &uart2Config) == SC_UART_RETURN_CODE_ERROR)
        loge("%s: Configure UART %d failure!!", LOG_UART, __func__);
        
}

static void sTask_userMain(void *argv)
{
    SIM_MSG_T msg = {0, 0, 0, NULL};
    if (SC_SUCCESS != sAPI_MsgQCreate(&appMain_msgq, "appMain_msgq", sizeof(SIM_MSG_T), 12, SC_FIFO))
        loge("Main queue create");
    if (SC_SUCCESS != sAPI_TimerCreate(&appMain_timer))
        loge("Main timer create");
    while (1)
    {
        msleep(100);
    }
}

static void appMain(void *argv)
{
    /* Do not comment the initialization code below */
    unsigned long *apiTable = (unsigned long *)argv;
    if (apiTable != NULL)
    {
        get_sAPI(apiTable);
        sAPI_Debug("sc_Init successfull...");
    }
    SIMComVersion info = {0};
    sAPI_SysGetVersion(&info);
    uart_config();
    /*End of initialization*/
    log_init(512);
    log_printf(BMS_INFO);
    log_printf(BMS_INFO_FM, FIRMWARE_VERSION, HARDWARE_VERSION, __DATE__, __TIME__, sAPI_GetFreeSize("c:"));
    logi("Main app running -> Phase Detector Slave");
    logi("SDK version: %s", info.SDK_Version);
    
    Gpio_create(&ledStatus, LED_STATUS_PIN);
    Gpio_config(&ledStatus, SC_GPIO_OUT_PIN, SC_GPIO_PULL_DISABLE, 0);
    if (sAPI_TaskCreate(&appMain_task, userMainStack, USER_MAIN_STACK_SIZE, 150, "userMain", sTask_userMain, NULL) != SC_SUCCESS)
        loge("mainTask create fail");
    initAppUart();
    initAppSimcom();
}

appRegItem_t helloworldapp_entry _appRegTable_attr_ = {.app_entry = appMain};