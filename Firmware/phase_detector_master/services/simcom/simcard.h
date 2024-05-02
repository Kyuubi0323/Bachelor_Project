#ifndef __SIMCARD_H__
#define __SIMCARD_H__

typedef struct simcard_t simcard_t;

typedef struct simcard_p {
    int(*cpin)(simcard_t*);
    const char*(*imsi)(simcard_t*);
    const char*(*iccid)(simcard_t*);
} simcard_p;

struct simcard_t {
    const simcard_p* proc;
    simcom_t* simcom;
    char* imsi;
    char* iccid;
};

simcard_t* simcard_create(simcom_t* simcom);
int simcard_get_cpin(simcard_t* simcard);
const char* simcard_get_imsi(simcard_t* simcard);
const char* simcard_get_iccid(simcard_t* simcard);
int simcard_get_error(void);

#endif