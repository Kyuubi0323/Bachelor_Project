#ifndef __LOG_H__
#define __LOG_H__

#include "sdk.h"
#ifdef _DEBUG

#define LOG_SIZE_DEFAULT    512
#define LOG_UART        SC_UART2
#define KNRM  "\x1B[0m "
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define log_print(type, fmt, args...) do { \
	usnprintf(LOG_BUFF, LOG_SIZE - 1, "%s[%08d][%s:%03d] " fmt KNRM "\r\n", type, sysTickCnt(), __FILENAME__, __LINE__, ##args); \
    sAPI_UartWrite(LOG_UART, (uint8_t*)LOG_BUFF, strlen(LOG_BUFF)); \
    sAPI_UsbVcomWrite((uint8_t*)LOG_BUFF, strlen(LOG_BUFF)); \
} while (0)

#define log_printf(fmt, args...) do { \
	usnprintf(LOG_BUFF, LOG_SIZE - 1, fmt, ##args); \
    sAPI_UartWrite(LOG_UART, (uint8_t*)LOG_BUFF, strlen(LOG_BUFF)); \
} while (0)


#define logi(fmt, args...) log_print("\x1B[32m[I]", fmt, ##args)
#define logw(fmt, args...) log_print("\x1B[33m[W]", fmt, ##args)
#define loge(fmt, args...) log_print("\x1B[31m[E]", fmt, ##args)

extern char* LOG_BUFF;
extern int LOG_SIZE;

void log_init(int size);
void log_deinit(void);
#else
#define logi(...)
#define logw(...)
#define loge(...)
#define log_printf(...)
#define log_deinit(...)
#define log_init(...)
#endif // __LOG_H__

#endif /* LOG_H */