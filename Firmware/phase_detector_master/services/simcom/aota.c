#include "sdk.h"
#include "log.h"
#include "simcom.h"
#include "aota.h"

static aota_p aota_proc = {
    aota_start,
    aota_start_local
};
static aota_t aota_object = {0};
static int aota_error_code = 0;

aota_t* aota_create(simcom_t* simcom) {
    aota_object.proc = &aota_proc;
    aota_object.simcom = simcom;
    aota_object.crc = 0;
    return &aota_object;
}

int aota_start(aota_t* ota, const char* url) {
    SCAppDownloadPram param;
    SCAppPackageInfo info;

    param.mod = SC_APP_DOWNLOAD_HTTP_MOD;
    param.recvtimeout = 30000;
    param.url = (INT8*)url;
    aota_error_code = 0;
    logi("App ota start");

    do {
        aota_error_code = sAPI_AppDownload(&param);
        if (aota_error_code != SC_APP_DOWNLOAD_SUCESSED) {
            loge("App ota download fail, code %d", aota_error_code);
            break;
        }

        aota_error_code = sAPI_AppPackageCrc(&info);
        ota->crc = info.crcValue;
        logi("App ota crc, code %d, crc %u", aota_error_code, ota->crc);
    } while (0);
    return aota_error_code;
}

int aota_start_local(aota_t* ota, const char* fpath) {
    return -1;
}

int aota_get_error(void) {
    return aota_error_code;
}

