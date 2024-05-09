#include "sdk.h"
#include "time.h"
#include "system.h"
#include "log.h"
#include "utils.h"
#include "simcom.h"
#include "gps.h"

#ifndef FEATURE_SIMCOM_GPS_OVERSEAS
#define SC_GNSS_MODE_GPS_GLONASS_GALILEO_SBAS_QZSS -1
#endif

date_time_t gps_dt = {0};
location_t gps_location = {0};

static gps_p gps_proc = {
		gps_dispose,
		gps_get_data
};
static gps_t gps_object = {0};

static bool gps_get_lbs = false;
static int gps_error_code = 0;
static void(*gps_data_events)(location_t* location) = NULL;

static void on_gps_data(int src, char* data);
static inline void gps_get_time(void);

gps_t* gps_create_standalone(simcom_t* simcom) {
    gps_t* object = NULL;
	do {
        if (sAPI_GnssPowerStatusSet(SC_GNSS_POWER_ON) != SC_GNSS_RETURN_CODE_OK)
            break;
        logi("Gps power on");

        if (sAPI_GnssModeSet(SC_GNSS_MODE_GPS_GLONASS_GALILEO_SBAS_QZSS) != SC_GNSS_RETURN_CODE_OK)
            break;
        logi("Gps config mode: %d", SC_GNSS_MODE_GPS_GLONASS_GALILEO_SBAS_QZSS);

        if (sAPI_GnssInfoGet(3) != SC_GNSS_RETURN_CODE_OK)
            break;
        
        gps_error_code = 0;
        object = &gps_object;
		object->location = NULL;
		object->simcom = simcom;
		object->proc = &gps_proc;
        logi("Gps standalone create success");
	} while (0);

    simcom_register_gps_event(on_gps_data);
	return object;
}

gps_t* gps_create_assist(simcom_t* simcom, const char* apn) {
    gps_t* object = NULL;
	do {
        if (sAPI_GnssPowerStatusSet(SC_GNSS_POWER_ON) != SC_GNSS_RETURN_CODE_OK)
            break;
        logi("Gps power on");

        if (sAPI_GnssStartMode(SC_GNSS_START_HOT) != SC_GNSS_RETURN_CODE_OK)
            break;
        logi("Gps start hot mode");

        logi("Gps baudrate: %d", sAPI_GnssBaudRateGet());
        logi("Gps nmearate: %d", sAPI_GnssNmeaRateGet());

        if (sAPI_GnssModeSet(SC_GNSS_MODE_GPS_BDS_GALILEO_SBAS_QZSS) != SC_GNSS_RETURN_CODE_OK)
            break;
        logi("Gps config mode: %d", SC_GNSS_MODE_GPS_BDS_GALILEO_SBAS_QZSS);

        if (sAPI_GnssAgpsSeviceOpen() != SC_GNSS_RETURN_CODE_OK)
            break;
        logi("Gps assist enable");

        if (sAPI_GnssInfoGet(3) != SC_GNSS_RETURN_CODE_OK)
            break;

        if ((gps_location.latitude == 0) && (gps_location.longitude == 0))
            sAPI_LocGet(0, simcom->msgq, 1);

        gps_error_code = 0;
        object = &gps_object;
		object->simcom = simcom;
		object->proc = &gps_proc;
        object->datetime = &gps_dt;
        object->location = &gps_location;
        logi("Gps assist create success");
	} while (0);

    gps_get_time();
    simcom_register_gps_event(on_gps_data);
	return object;
}

void gps_dispose(gps_t* object) {
    gps_error_code = sAPI_GnssInfoGet(0);
    sAPI_GnssPowerStatusSet(SC_GNSS_POWER_OFF);
}

int gps_get_data(gps_t* object) { 
    if (!gps_get_lbs && (gps_location.latitude == 0) && (gps_location.longitude == 0)) {
        gps_get_lbs = true;
        sAPI_LocGet(0, object->simcom->msgq, 1);
    }
    return -1;
}

void gps_register_data_event(void(*callback)(location_t* location)) {
    gps_data_events = callback;
}

int gps_get_error(void) {
    return gps_error_code;
}

static void on_gps_data(int src, char* data) {
    int index;
    double temp;
    char buffer[30];
    SC_lbs_info_t* lbs;
    int size = strlen((const char*)data);

    gps_location.speed = 0;
    
    if ((src == SC_URC_GNSS_INFO) && (size > 10)) {
        data[size - 1] = 0;
        logi("Gps data : %s", data);

        do {
            if (size < 50)
                logw("Check -1");
                break;
            
            if (strstr((data), "+CGNSSINFO:") == NULL)
                logw("Check 0");
                break;
            data += 11;
            size -= 11;

            index = split_string(data, buffer, 0, size, 30, ',');
            if (index < 0)
                logw("Check 1");
                break;
            
            index = split_string(data, buffer, index, size, 30, ',');
            if (index < 0)
                logw("Check 2");
                break;
            // logi("Gps satellite: %s", (const char*)buffer);

            index = split_string(data, buffer, index, size, 30, ',');
            if (index < 0)
                logw("Check 3");
                break;
            // logi("Gnss satellite: %s", (const char*)buffer);

            index = split_string(data, buffer, index, size, 30, ',');
            if (index < 0)
                logw("Check 4");
                break;
            // logi("Beidou satellite: %s", (const char*)buffer);

            index = split_string(data, buffer, index, size, 30, ',');
            if (index < 0)
                logw("Check 5");
                break;
            // logi("Gps ???: %s", (const char*)buffer);

            index = split_string(data, buffer, index, size, 30, ',');
            if (index < 0)
                logw("Check 6");
                break;
            temp = u_atof(buffer) / 100;
            gps_location.latitude = (int)temp;
            temp = (temp - gps_location.latitude) * 100;
            gps_location.latitude += (temp / 60);
            logi("Gps latitude: %s", (const char*)buffer);

            index = split_string(data, buffer, index, size, 30, ',');
            if (index < 0)
                break;
            gps_location.ns = buffer[0];
            // logi("Gps N/S: %s", (const char*)buffer);

            index = split_string(data, buffer, index, size, 30, ',');
            if (index < 0)
                break;
            temp = u_atof(buffer) / 100;
            gps_location.longitude = (int)temp;
            temp = (temp - gps_location.longitude) * 100;
            gps_location.longitude += (temp / 60);
            logi("Gps longitude: %s", (const char*)buffer);

            index = split_string(data, buffer, index, size, 30, ',');
            if (index < 0)
                break;
            gps_location.ew = buffer[0];
            // logi("Gps E/W: %s", (const char*)buffer);

            index = split_string(data, buffer, index, size, 30, ',');
            if (index < 0)
                break;
            // logi("Gps date: %s", (const char*)buffer);

            index = split_string(data, buffer, index, size, 30, ',');
            if (index < 0)
                break;
            // logi("Gps time: %s", (const char*)buffer);

            index = split_string(data, buffer, index, size, 30, ',');
            if (index < 0)
                break;
            // logi("Gps altitude: %s", (const char*)buffer);

            index = split_string(data, buffer, index, size, 30, ',');
            if (index < 0)
                break;
            gps_object.location->speed = u_atof((char*)buffer);
            logi("Gps speed: %s", (const char*)buffer);

            gps_location.padding = (gps_location.latitude != 0) & (gps_location.longitude != 0);
        } while (0);
    }
    else if (src == 255) {
        gps_get_lbs = false;
        lbs = (SC_lbs_info_t*)data;
        if (lbs->u8ErrorCode == 0) {
            gps_location.latitude = ((double)(lbs->u32Lat)) / 1000000;
            gps_location.longitude = ((double)(lbs->u32Lng)) / 1000000;
        }
        logw("Gps lbs data: %d, latitude: %u, longitude: %u", lbs->u8ErrorCode, lbs->u32Lat, lbs->u32Lng);
    }
    gps_get_time();

    if (gps_data_events != NULL)
        gps_data_events(&gps_location);
}

static inline void gps_get_time(void) {
    struct tm dt;
    SCsysTime_t sdt;

    umemset(&sdt,0,sizeof(sdt));
    sAPI_GetSysLocalTime(&sdt);
    
    dt.tm_sec = sdt.tm_sec;
    dt.tm_min = sdt.tm_min;
    dt.tm_hour = sdt.tm_hour; 
    dt.tm_mday = sdt.tm_mday;
    dt.tm_mon = sdt.tm_mon - 1;
    dt.tm_year = sdt.tm_year - 1900;
    gps_dt.ts = (uint32_t)mktime(&dt) + 7 * 3600 - 946684800; // convert to 2000
    gps_dt.tick = getSysTickCount();
    logi("[Ntp] %02d/%02d/%d %02d:%02d:%02d", sdt.tm_mday, sdt.tm_mon, sdt.tm_year, sdt.tm_hour, sdt.tm_min, sdt.tm_sec);
}