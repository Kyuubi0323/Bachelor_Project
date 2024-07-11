#ifndef __SDK_H__
#define __SDK_H__

#ifndef SIMCOM_A7678_V1_02 //avoid warning
#define SIMCOM_A7678_V1_02
#endif

#include "simcom_api.h"
#include "time.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "gpio.h"

// typedef BOOL            bool;
// typedef UINT8           uint8_t;
// typedef UINT16          uint16_t;
// typedef UINT32          uint32_t;
// typedef UINT64          uint64_t;
// typedef INT8            int8_t;
// typedef INT16           int16_t;
// typedef INT32           int32_t;
// typedef INT64           int64_t;

#define ualloc          sAPI_Malloc
#define ufree           sAPI_Free
#define usscanf         sAPI_sscanf
#define usprintf        sAPI_Sprintf
#define usnprintf       sAPI_Snprintf
#define umemset         memset
#define umemcpy         memcpy
#define umemcmp         memcmp
#define ustrlen         strlen
#define umsize          sizeof
#define msleep(t)       sAPI_TaskSleep((t) / 5)
#define osleep(t)       sAPI_TaskSleep(t)

#define sysTickCnt()    (sAPI_GetTicks() * 5)
#define sysDump         sAPI_EnableDump

#ifdef _DEBUG
#define uprintf sAPI_Debug
#else
#define uprintf(...)
#endif

#ifndef null
#define null 0
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

#define ICACHE_FLASH_ATTR

#endif /* __SDK_H__ */