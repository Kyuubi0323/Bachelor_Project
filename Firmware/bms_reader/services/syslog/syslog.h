#ifndef __SYSLOG_H__
#define __SYSLOG_H__

#define SYSLOG_EN

#if (defined SYSLOG_EN)
#include <stdint.h>

#define SYSLOG_ADDR_START       0
#define SYSLOG_ADDR_END         0x2000
#define SYSLOG_PAGE_MAGIC       0x1fff //FLASH_PAGE_SIZE - 1
#define SYSLOG_BLOCK_COUNT      1

void initSyslog(void);
void syslog(uint8_t cause, uint16_t value, uint8_t extra);
bool backupSyslogCurrent(void);

extern uint32_t syslogAddr;

#else
#define initSysLog(...)
#define syslog(...)
#endif

#endif