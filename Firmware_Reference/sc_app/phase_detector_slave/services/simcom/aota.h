#ifndef __AOTA_H__
#define __AOTA_H__

typedef struct aota_t aota_t;

typedef struct aota_p {
    int(*start)(aota_t*, const char*);
    int(*start_local)(aota_t*, const char*);
} aota_p;

struct aota_t {
    const aota_p* proc;
    simcom_t* simcom;
    uint32_t crc;
};

aota_t* aota_create(simcom_t* simcom);
int aota_start(aota_t* ota, const char* url);
int aota_start_local(aota_t* ota, const char* fpath);

int aota_get_error(void);

#endif