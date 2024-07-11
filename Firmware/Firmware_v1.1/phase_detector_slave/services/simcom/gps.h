#ifndef __GPS_H__
#define __GPS_H__

typedef struct gps_t gps_t;

typedef struct date_time_t {
	uint32_t ts;
	uint32_t tick;
} date_time_t;

typedef struct location_t {
	double latitude;
	double longitude;
	double speed;
	uint8_t ns;
	uint8_t ew;
	uint8_t padding;
	uint8_t reserve;
} location_t;

typedef struct gps_p {
	void(*dispose)(gps_t*);
	int(*get_data)(gps_t*);
} gps_p;

struct gps_t {
	const gps_p* proc;
	simcom_t* simcom;
	location_t* location;
	date_time_t* datetime;
};

gps_t* gps_create_standalone(simcom_t* simcom);
gps_t* gps_create_assist(simcom_t* simcom, const char* apn);
void gps_dispose(gps_t* object);
int gps_get_data(gps_t* object);
void gps_register_data_event(void(*callback)(location_t* location));

int gps_get_error(void);

#endif