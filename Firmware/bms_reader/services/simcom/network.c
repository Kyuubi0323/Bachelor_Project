#include "sdk.h"
#include "log.h"
#include "syslog.h"
#include "simcom.h"
#include "network.h"

static network_p network_proc = {
		network_connect,
		network_disconnect,
		network_get_rssi
};
static network_t nwk_object = {0};

static int ntp_update_status = 0;
static void(*ntp_update_event)(int status) = NULL;

static void on_ntp_update(int status);
static void network_debug_info(void);

network_t* network_create(simcom_t* simcom) {
    network_t* object = &nwk_object;
	object->proc = &network_proc;
    object->connected = false;
	object->simcom = simcom;
    object->cfun = NWK_CFUN_1;

    if (network_get_cfun(NULL) != NWK_CFUN_1)
        sAPI_NetworkSetCfun(NWK_CFUN_1);

    ntp_update_status = -1;
    simcom_register_ntp_event(on_ntp_update);
	return object;
}

int network_disconnect(network_t* object) {
    object->connected = false;
    return 0;
}

int network_connect(network_t* object, const char* apn, const char* user, const char* pwd) {
    int creg;
    uint8_t cfun = 0;
    sAPI_NetworkInit();
    do {
        if (sAPI_NetworkGetCfun(&cfun) != SC_NET_SUCCESS)
            break;
        logi("Network cfun: %d", (int)cfun);
        if (sAPI_NetworkSetCops(0, 1,"45204",7) != SC_NET_SUCCESS)
            loge("Network set ops");
        if (sAPI_NetworkSetCnmp(2) != SC_NET_SUCCESS)
            loge("Network set nmp");
        if (sAPI_NetworkGetCreg(&creg) != SC_NET_SUCCESS)
            break;
        logi("Network creg: %d", creg);
        if ((creg != 1) && (creg != 5)) {
            break;
        }
        if (sAPI_NetworkSetCgdcont(1, "IP", (char*)apn) != SC_NET_SUCCESS)
            break;
        logi("Network config APN: %s", apn);

        if (sAPI_NetworkSetCgact(1, 1) != SC_NET_SUCCESS)
            break;
        logi("Network active PDP");
        
        if (!network_get_ip(object))
            break;

        network_debug_info();

        if (sAPI_NtpUpdate(SC_NTP_OP_SET, "time.digotech.net", 0, NULL) != SC_NTP_OK)
            break;
        if (sAPI_NtpUpdate(SC_NTP_OP_EXC, NULL, 0, (sMsgQRef)(object->simcom->msgq)) != SC_NTP_OK)
            break;
        object->connected = true;
        logi("Network connected");
    } while (0); 
    return !(object->connected);
}

int network_get_rssi(network_t* object) { 
    uint8_t csq;
    int rssi = -1;
    if (sAPI_NetworkGetCsq(&csq) == SC_NET_SUCCESS)
    {
        if (csq == 0)
            rssi = -113;
        else if (csq == 1)
            rssi = -111;
        else if ((csq >= 2) && (csq <= 30))
            rssi = (csq * 2 - 113);
        else if (csq == 31)
            rssi = -51;
    }
    if (ntp_update_status > 0) {
        if (sAPI_NtpUpdate(SC_NTP_OP_SET, "time.google.com", 0, NULL) == SC_NTP_OK)
            if (sAPI_NtpUpdate(SC_NTP_OP_EXC, NULL, 0, (sMsgQRef)(object->simcom->msgq)) == SC_NTP_OK)
                ntp_update_status = -1;
        logi("Ntp reconfig -> %d", ntp_update_status);
    }
    return rssi;
}

int network_get_cfun(network_t* object) {
    uint8_t cfun;
    if (sAPI_NetworkGetCfun(&cfun) == SC_NET_SUCCESS) {
        nwk_object.cfun = (int)cfun;
        return nwk_object.cfun;
    }
    return -1;
}

int network_set_cfun(network_t* object, int cfun) {
    if (sAPI_NetworkSetCfun(cfun) == SC_NET_SUCCESS) {
        nwk_object.cfun = cfun;
        return 0;
    }
    return -1;
}

uint32_t network_get_ip(network_t* object) {
    uint32_t ip = 0;
    SCcgpaddrParm cgpaddrParm;
    umemset(&cgpaddrParm, 0, sizeof(cgpaddrParm));
    if (sAPI_NetworkGetCgpaddr(1, &cgpaddrParm) == SC_NET_SUCCESS) {
        ip = sAPI_TcpipInet_addr(cgpaddrParm.ipv4addr);
        umemcpy(object->ip, cgpaddrParm.ipv4addr, sizeof(cgpaddrParm.ipv4addr));
        logi("Network IP: %s (%u)" ,object->ip, ip);
    }
    return ip;
}

void network_register_ntp_event(void(*callback)(int status)) {
    ntp_update_event = callback;
}

static void on_ntp_update(int status) {
    ntp_update_status = status;
    if (status > 0)
        syslog(8, 0, 3);
    else if (ntp_update_event != NULL)
        ntp_update_event(status);
    logi("Ntp update status: %d", status);
    SCsysTime_t now = {0};
    sAPI_GetSysLocalTime(&now);
    logi("Current date time now :%02d/%02d/%02d  %02d:%02d:%02d", now.tm_mday, now.tm_mon, now.tm_year, now.tm_hour, now.tm_min, now.tm_sec);
}

static void network_debug_info(void) {
#ifdef _DEBUG
        int cnmp = 0;
        char cops[100] = {0};
        SCcpsiParm cpsi = {0};

        logi("Network rssi: %d", network_get_rssi(NULL));

        if(sAPI_NetworkGetCpsi(&cpsi) == SC_NET_SUCCESS)
            logi("Network %s, MM=%s, GSM Band: %s,LTE Band: %s", cpsi.networkmode, cpsi.Mnc_Mcc, cpsi.GSMBandStr, cpsi.LTEBandStr);

        if (sAPI_NetworkGetCnmp(&cnmp) == SC_NET_SUCCESS)
            logi("Network cnmp: %d", cnmp);

        if (sAPI_NetworkGetCops(cops) == SC_NET_SUCCESS)
            logi("Network cops: %s", cops);
#endif
}
