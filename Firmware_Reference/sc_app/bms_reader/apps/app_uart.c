#include "sdk.h"
#include "fifo.h"
#include "log.h"
#include "lwmb.h"
#include "types.h"

#define RS485_DE_RE_PIN         SC_MODULE_GPIO_5
#define RS485_SERIAL_BUF_SIZE   256
#define RS485_SERIAL_PORT       SC_UART

#define TOUT_2400               40
#define TOUT_9600               30
#define TOUT_115200             5

/* ----------------------- static functions ---------------------------------*/

static sMsgQRef appUart_msgq;
static sTaskRef appUart_task;
static sTimerRef appUart_timer;

static UINT8 appUart_stack[4096];
static UINT8 mbPoll_stack[4096];
static sTaskRef mbPoll_task;

static fifo_t sc_fifo = {0};
static UINT8 sc_buffer[RS485_SERIAL_BUF_SIZE] = {0};
static int rx_size = 0;
static Gpio_t rs485_dir = {0};

mb_t *modbus = NULL;
bmsInfo_t bmsInfo = {0};
int uartSend(UINT8 *data, UINT32 size) {
    // Gpio_setState(&rs485_dir, 0);
    int len = sAPI_UartWrite(RS485_SERIAL_PORT, data, size);
    // msleep(5);
    // Gpio_setState(&rs485_dir, 1);
    return len;
}
static void vPortSerialTask(void *pvParameters)
{
    SIM_MSG_T msg = {0, 0, 0, NULL};
    for (;;)
    {
        sAPI_MsgQRecv(appUart_msgq, &msg, SC_SUSPEND);
        if (msg.msg_id == 1)
        {
            if (msg.arg1 == 1)
            {
                // logi("Data receive:[%d]", rx_size);
                // for (int i = 0; i < rx_size; i++)
                // {
                //     sAPI_UartPrintf("%.2x", sc_buffer[i]);
                // }
                // sAPI_UartPrintf("\r\n");
                modbus->proc->parse(modbus, sc_buffer, rx_size);
                rx_size = 0;
                memset(sc_buffer, 0, sizeof(sc_buffer));
            }
        }
    }
    sAPI_TaskDelete(NULL);
}
UINT16 swapByte(UINT16 byte) {
    UINT8 L = byte & 0xFF;
    UINT8 H = byte >> 8;
    return L << 8 | H;
}
static void mbPollTask(void *param) {
    UINT16 readData[64] = {0};
    INT16 size = 0;
    
    while (1) {
        size = 0;
        mb_readholdingregisters(modbus, 1, 1000, 13, readData, &size);
        if (size > 0)
            break;
        msleep(1000);
    }
    for (int i = 0; i < size; i++)
        readData[i] = swapByte(readData[i]);
    readData[size] = 0;
    logi("bms id: %s", (CHAR*)readData);
    memcpy(bmsInfo.id, (CHAR*)readData, strlen((CHAR*)readData));
    int errCnt = 0;
    while(1) {
        size = 0;
        mb_readholdingregisters(modbus, 1, 0, 29, readData, &size);
        if (size  > 0) {
            // for (INT16 i = 0; i < size ; i++)
            //     sAPI_UartPrintf("%.4x, ", readData[i]);
            // sAPI_UartPrintf("\r\n");
            errCnt = 0;
            bmsInfo.state = 1;
            bmsInfo.vol = readData[0];
            bmsInfo.cells = readData[1];
            bmsInfo.soc = readData[2];
            bmsInfo.cap = readData[3];
            bmsInfo.i_out = readData[4];
            bmsInfo.envTemp = readData[6];
            bmsInfo.cellsTemp = readData[7];
            bmsInfo.boardTemp = readData[8];
            for (int i = 0; i < MAX_CELL_NUMBER; i++)
                bmsInfo.cellsVol[i] = readData[9+i];
        }
        else errCnt++;
        if (errCnt > 5) {
            umemset((void*)&bmsInfo, 0, sizeof(bmsInfo));
        }
        msleep(2000);
    }
}
static void portSerialTimerCallback(UINT32 arg)
{
    SIM_MSG_T msg = {1, 1, 0, NULL};
    if (appUart_msgq != NULL)
        sAPI_MsgQSend(appUart_msgq, &msg);
}
static void portSerialCallback(SC_Uart_Port_Number portNumber, void *para)
{
    int readLen = 0;
    UINT8 *uartdata = sAPI_Malloc(100);
    readLen = sAPI_UartRead(portNumber, uartdata, 100);
    umemcpy(&sc_buffer[rx_size], uartdata, readLen);
    rx_size += readLen;
    sTimerStatus timerStatus;
    sAPI_TimerGetStatus(appUart_timer, &timerStatus);
    if (timerStatus.status == SC_ENABLED) {
        sAPI_TimerStop(appUart_timer);
    }
    sAPI_TimerStart(appUart_timer, TOUT_9600, 0, portSerialTimerCallback, 0);
    sAPI_Free((void *)uartdata);
}

void initAppUart(void) {
    do
    {

        // Init Ring buffer
        CBUFFER_Init(&sc_fifo, sc_buffer, RS485_SERIAL_BUF_SIZE);
        // Init DE/RE Gpio
        Gpio_create(&rs485_dir, RS485_DE_RE_PIN);
        Gpio_config(&rs485_dir, SC_GPIO_OUT_PIN, SC_GPIO_PULL_DISABLE, 1);
        // Init Serial
        SCuartConfiguration uartConfig;
        uartConfig.BaudRate = 9600;
        uartConfig.DataBits = SC_UART_WORD_LEN_8;
        uartConfig.ParityBit = SC_UART_NO_PARITY_BITS;
        uartConfig.StopBits = SC_UART_ONE_STOP_BIT;
        if (sAPI_UartSetConfig(RS485_SERIAL_PORT, &uartConfig) == SC_UART_RETURN_CODE_ERROR)
        {
            loge("%s: Configure UART %d failure!!", (UINT8)RS485_SERIAL_PORT, __func__);
            break;
        }
        sAPI_UartRegisterCallback(RS485_SERIAL_PORT, portSerialCallback);
        if (SC_SUCCESS != sAPI_MsgQCreate(&appUart_msgq, "appUart_msgq", sizeof(SIM_MSG_T), 12, SC_FIFO))
        {
            loge("appUart_msgq create failure");
            break;
        }
        if (SC_SUCCESS != sAPI_TimerCreate(&appUart_timer))
        {
            loge("appUart_timer create failure");
            break;
        }
        modbus = mb_init();
        mb_register_send_event(modbus, uartSend);
        if (sAPI_TaskCreate(&appUart_task, appUart_stack, 4096, 150, "PortSerialTask", vPortSerialTask, NULL) != SC_SUCCESS)
        {
            loge("PortSerialTask create failure");
            break;
        }
        sAPI_TaskCreate(&mbPoll_task, mbPoll_stack, 4096, 150, "PortSerialTask", mbPollTask, NULL);
    } while (0); 
}