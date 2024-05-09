#include <time.h>
#include "sdk.h"
#include "log.h"
#include "flash.h"
#include "simcom.h"
#include "gps.h"
#include "system.h"
#include "fs.h"
#include "gps_syslog_def.h"
#include "syslog.h"

uint32_t syslogAddr = 0;

#if (defined SYSLOG_EN)
#define SYSLOG_ITEM_SIZE        8   //4 byte ts + cause + value + extra
#define SYSLOG_ITEM_FREE        0xffffffff
#define SYSLOG_PAGE_SIZE        0x2000

#define SYSLOG_FS_PATH          "c:/syslog"

extern simcom_t* simcom;
extern date_time_t gps_dt;

void initSyslog(void) {
    int reval;
    // void* dd = NULL;
    uint32_t temp[2];
    struct dirent info_file = {0};
    // struct dirent *info_dir = NULL;

    reval = sAPI_stat(SYSLOG_FS_PATH, &info_file);
    if (reval != 0) {
        reval = sAPI_mkdir(SYSLOG_FS_PATH, 0);
        logi("Syslog create folder -> %d", reval);
    }
#ifdef _DEBUG
    // else {
    //     dd = fs_openDir(SYSLOG_FS_PATH);
    //     if (dd != NULL) {
    //         while((info_dir = sAPI_readdir(dd)) != NULL)
    //             logi("[%s]-[%ld]-[%d]",info_dir->name, info_dir->size, info_dir->type);
    //         fs_closeDir(dd);
    //     }
    // }
#endif

    syslogAddr = SYSLOG_ADDR_START;
    while (syslogAddr < SYSLOG_ADDR_END) {
        flash_read(syslogAddr, (uint8_t*)temp, SYSLOG_ITEM_SIZE);
        if (temp[0] == SYSLOG_ITEM_FREE)
            break;
#ifdef _DEBUG 
        else
            printSysLog(syslogAddr, temp[1], temp[0]);
#endif
        syslogAddr += SYSLOG_ITEM_SIZE;
    }
    if (syslogAddr >= SYSLOG_ADDR_END)
        syslogAddr = SYSLOG_ADDR_START;
    if ((syslogAddr & SYSLOG_PAGE_MAGIC) == 0) {
        flash_erase(syslogAddr, SYSLOG_PAGE_SIZE);
        logw("erase address %08X", syslogAddr);
    }
    syslog(0, 0, 0);
}

void syslog(uint8_t cause, uint16_t value, uint8_t extra) {
    uint32_t data[2];
    if ((syslogAddr < SYSLOG_ADDR_START) || (syslogAddr >= SYSLOG_ADDR_END))
        initSyslog();
    
    data[0] = (uint8_t)cause;
    data[0] = (data[0] << 16) + (uint16_t)value;
    data[0] = (data[0] << 8) + (uint8_t)extra;
    data[1] = gps_dt.ts + (((uint32_t)(getSysTickCount() - gps_dt.tick)) / 1000);
    flash_write(syslogAddr, (uint8_t*)data, SYSLOG_ITEM_SIZE);
    logw("address = %08X, cause = %d, value = %d, extra = %d", syslogAddr, (int)cause, (int)value, (int)extra);
    
    syslogAddr += SYSLOG_ITEM_SIZE;
    if (syslogAddr >= SYSLOG_ADDR_END)
        syslogAddr = SYSLOG_ADDR_START;
    if ((syslogAddr & SYSLOG_PAGE_MAGIC) == 0) {
        flash_erase(syslogAddr, SYSLOG_PAGE_SIZE);
        logw("erase address %08X", syslogAddr);
    }
}

bool backupSyslogCurrent(void) {
    void* fd = NULL;
    char* buff = NULL;
    bool reval = false;
    char file[30] = {0};

    do {
        usprintf(file, "c:/%d.bin", (int)(simcom->id));
        fs_delete(file);

        fd = fs_open((const char*)file, FS_WRITE_ONLY);
        if (fd == NULL)
            break;

        buff = ualloc(4096);
        if (buff == NULL)
            break;

        if (flash_read(0, (uint8_t*)buff, 4096) != 0)
            break;
        
        if (fs_write(fd, (void*)buff, 4096) < 0)
            break;

        if (flash_read(4096, (uint8_t*)buff, 4096) != 0)
            break;
        
        if (fs_write(fd, (void*)buff, 4096) < 0)
            break;

        reval = true;
    } while (0);

    if (fd != NULL)
        fs_close(fd);
    if (buff != NULL)
        ufree(buff);

    return reval;
}
#endif
