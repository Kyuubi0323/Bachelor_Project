#ifndef __HTTP_H__
#define __HTTP_H__

typedef struct http_t http_t;

typedef struct http_p {
    int(*hclose)(http_t*);
    int(*hget)(http_t*, const char*, uint8_t*, int);
} http_p;

struct http_t {
    const http_p* proc;
    simcom_t* simcom;
};

http_t* http_create(simcom_t* simcom);
int http_close(http_t* http);
int http_get(http_t* http, const char* url, uint8_t* buffer, int size);

int http_get_error(void);
int http_get_status(void); // http code error

#endif