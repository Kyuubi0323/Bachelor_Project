#ifndef __FTP_H__
#define __FTP_H__

typedef struct ftp_t ftp_t;

typedef struct ftp_p {
    void(*dispose)(ftp_t*);
    int(*uconnect)(ftp_t*, const char*, int, const char*, const char*);
    int(*upload)(ftp_t*, const char*, const char* file);
} ftp_p;

struct ftp_t {
    const ftp_p* proc;
    simcom_t* simcom;
};

ftp_t* ftp_create(simcom_t* simcom);
void ftp_dispose(ftp_t* ftp);
int ftp_connect(ftp_t* ftp, const char* server, int port, const char* user, const char* password);
int ftp_upload(ftp_t* ftp, const char* path, const char* file);

int ftp_get_error(void);

#endif