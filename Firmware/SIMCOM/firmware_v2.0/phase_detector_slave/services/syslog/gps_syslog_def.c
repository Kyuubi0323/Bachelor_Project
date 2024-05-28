#include "time.h"
#include "sdk.h"
#include "log.h"
#include "utils.h"
#include "gps_syslog_def.h"

#ifdef _DEBUG 
void printSysLog(uint32_t address, uint32_t ts, uint32_t log) {
    struct tm* sysdt = u_gmtime((const time_t*)&ts);
    uint8_t cause = (uint8_t)(log >> 24);
    uint16_t value = (uint16_t)(log >> 8);
    uint8_t extra = (uint8_t)log;
    log_printf("syslog at %08X ", address, log);
    log_printf("%02d/%02d/%04d ", sysdt->tm_mday, sysdt->tm_mon + 1, sysdt->tm_year + 2000);
    log_printf("%02d:%02d:%02d ", sysdt->tm_hour, sysdt->tm_min, sysdt->tm_sec);
    if (cause == 0)
        log_printf("syslog init\r\n");
    else if (cause == 1) {
        if (extra == 0)
            log_printf("reset up reason %d\r\n", (int)value);
        else if (extra == 1)
            log_printf("reset down reason %d\r\n", (int)value);
        else if (extra == 2)
            log_printf("reset rtc reason %d\r\n", (int)value);
        else if (extra == 3)
            log_printf("device init error %02X\r\n", value);
        else
            log_printf("reset unknow reason %d\r\n", (int)value);
    }
        
    else if (cause == 2) {
        if (value == 0)
            log_printf("mmi test start\r\n");
        else if (value == 1)
            log_printf("mmi test success\r\n");
        else if (value == 2)
            log_printf("mmi test failed\r\n");
        else
            log_printf("mmi test unknow\r\n");
    }
    else if (cause == 3) {
        if (extra == 0)
            log_printf("detect low voltage %d\r\n", (int)value);
        else if (extra == 1)
            log_printf("device start %d\r\n", (int)value);
        else if (extra == 2)
            log_printf("box off by voltage %d\r\n", (int)value);
        else if (extra == 3)
            log_printf("sim voltage %d\r\n", (int)value);
        else if (extra == 4)
            log_printf("power change %d\r\n", (int)value);
        else if (extra == 5)
            log_printf("device start by speed %d\r\n", (int)value);
        else if (extra == 6)
            log_printf("device start by power %d\r\n", (int)value);
        else
            log_printf("voltage %d\r\n", (int)value);
    }
    else if (cause == 4) {
        if (extra == 1)
            log_printf("device sleep by %d\r\n", (int)value);
        else if (extra == 0) 
            log_printf("device wakeup by %d\r\n", (int)value);
        else
            log_printf("unknow power event %d\r\n", (int)value);
    }
    else if (cause == 5) {
        log_printf("bma sensor error\r\n");
    }
    else if (cause == 6) {
        log_printf("secure key %d\r\n", (int)value);
    }
    else if (cause == 7) {
        if (value == 0)
            log_printf("ota start %d\r\n", (int)extra);
        else if(value == 1)
            log_printf("ota stop %d\r\n", (int)extra);
        else
            log_printf("ota unknow %d\r\n", (int)extra);
    }
    else if (cause == 8) {
        if (value == 0) {
            if (extra == 0)
                log_printf("simcom init fail\r\n");
            else if (extra == 1)
                log_printf("simcom no response\r\n");
            else if (extra == 2)
                log_printf("newtork connect fail\r\n");
            else if (extra == 3)
                log_printf("ntp update fail\r\n");
            else if (extra == 10)
                log_printf("newtork fail by unknown\r\n");
            else if (extra == 11)
                log_printf("newtork fail by gatt \r\n");
            else if (extra == 12)
                log_printf("newtork fail by ip\r\n");
            else if (extra == 13)
                log_printf("newtork fail by bearer\r\n");
            else if (extra == 20)
                log_printf("sms fail by unknown\r\n");
            else if (extra == 21)
                log_printf("sms fail by sim pin\r\n");
            else if (extra == 22)
                log_printf("sms fail by sim reg\r\n");
            else
                log_printf("simcom unknow event\r\n");
        }
        else if (value == 1) {
            if (extra == 0)
                log_printf("sms init fail\r\n");
            else if (extra == 1)
                log_printf("sms cmd turn on\r\n");
            else if (extra == 2)
                log_printf("sms cmd turn off\r\n");
            else if (extra == 3)
                log_printf("sms cmd restore\r\n");
            else if (extra == 4)
                log_printf("sms cmd restart\r\n");
            else if (extra == 5)
                log_printf("sms cmd search\r\n");
            else if (extra == 6)
                log_printf("sms cmd set ota\r\n");
            else if (extra == 7)
                log_printf("sms cmd set auto\r\n");
            else if (extra == 8)
                log_printf("sms cmd set voltage\r\n");
            else if (extra == 9)
                log_printf("sms cmd set server\r\n");
            else if (extra == 10)
                log_printf("sms cmd set password\r\n");
            else if (extra == 11)
                log_printf("sms cmd set phone\r\n");
            else if (extra == 12)
                log_printf("factory reset device\r\n");
            else if (extra == 13)
                log_printf("sms brand name\r\n");
            else if (extra == 14)
                log_printf("sms sos\r\n");
            else if (extra == 254)
                log_printf("sms syntax invalid\r\n");
            else if (extra == 255)
                log_printf("sms cmd wrong phone\r\n");
            else
                log_printf("sms cmd unknow\r\n");
        }
        else if (value == 2) {
            if (extra == 0)
                log_printf("socket init error\r\n");
            else if (extra == 1)
                log_printf("socket send error\r\n");
            else
                log_printf("socket event\r\n");
        }
        else if (value == 3) {
            if (extra == 0)
                log_printf("gps init error\r\n");
            else if (extra == 1)
                log_printf("gps init success\r\n");
            else if (extra == 2)
                log_printf("gps error time\r\n");
            else
                log_printf("gps event\r\n");
        }
        else if (value == 4) {
            int rssi = (int)extra - 255;
            log_printf("rssi: %d\r\n", rssi);
        }
    }
    else if (cause == 9) {
        log_printf("temperature = %d\r\n", (int)value);
    }
    else if (cause == 10) {
        log_printf("Tracking task error, cpu = %d\r\n", (int)value);
    }
    else
        log_printf("unknow event\r\n");
        
}
#endif