#include "sdk.h"
#include "log.h"
#include "simcom.h"
#include "simcard.h"

static int simcard_error_code = 0;
static char simcard_imsi[32];
static char simcard_iccid[32];

static simcard_p simcard_proc = {
    simcard_get_cpin,
    simcard_get_imsi,
    simcard_get_iccid
};
static simcard_t simcard_object = {0};

simcard_t* simcard_create(simcom_t* simcom) {
    uint8_t cpin;
    simcard_t* object = NULL;

    do {
        if (sAPI_SimPinGet(&cpin) != SC_SIM_RETURN_SUCCESS)
            break;
        simcard_error_code = cpin;
        if (cpin != SC_SIM_READY)
            break;
        
        object = &simcard_object;
        object->proc = &simcard_proc;
        object->simcom = simcom;
        object->imsi = NULL;
        object->iccid = NULL;

        memset(simcard_imsi, 0, 32);
        if (sAPI_SysGetImsi(simcard_imsi) == SC_SIM_RETURN_SUCCESS) {
            object->imsi = simcard_imsi;
            logi("Simcard IMSI: %s", (const char*)simcard_imsi);
        }

        memset(simcard_iccid, 0, 32);
        if (sAPI_SysGetIccid(simcard_iccid) == SC_SIM_RETURN_SUCCESS) {
            object->iccid = simcard_iccid;
            logi("Simcard ICCID: %s", (const char*)simcard_iccid);
        }
    } while (0);
    return object;
}

int simcard_get_cpin(simcard_t* simcard) {
    uint8_t cpin;
    sAPI_SimPinGet(&cpin);
    return (int)cpin;
}

const char* simcard_get_imsi(simcard_t* simcard) {
    if (simcard->imsi == NULL) {
        if (sAPI_SysGetImsi(simcard_imsi) == SC_SIM_RETURN_SUCCESS)
            simcard->imsi = simcard_imsi;
    }
    return simcard->imsi;
}

const char* simcard_get_iccid(simcard_t* simcard) {
    if (simcard->iccid == NULL) {
        if (sAPI_SysGetIccid(simcard_iccid) == SC_SIM_RETURN_SUCCESS)
            simcard->iccid = simcard_iccid;
    }
    return simcard->iccid;
}

int simcard_get_error(void) {
    return simcard_error_code;
}