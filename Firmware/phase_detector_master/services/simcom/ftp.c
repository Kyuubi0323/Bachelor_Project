#include "sdk.h"
#include "log.h"
#include "simcom.h"
#include "ftp.h"

static sMsgQRef ftpMsgQueue = NULL;
static int ftp_error_code = 0;

static ftp_p ftp_proc = {
    ftp_dispose,
    ftp_connect,
    ftp_upload
};
static ftp_t ftp_object = {0};

ftp_t* ftp_create(simcom_t* simcom) {
    SIM_MSG_T msg;
    ftp_t* object = NULL;
    
    do {
        if (ftpMsgQueue == NULL)
            ftp_error_code = sAPI_MsgQCreate(&ftpMsgQueue, "Ftp queue", sizeof(SIM_MSG_T), 4, SC_FIFO);
        if (ftp_error_code != SC_SUCCESS)
            break;

        sAPI_FtpsInit(SC_FTPS_USB, ftpMsgQueue);
        sAPI_MsgQRecv(ftpMsgQueue, &msg, SC_SUSPEND);

        if (msg.arg3 != NULL)
            ufree(msg.arg3);

        if (msg.arg2 != SC_FTPS_RESULT_OK) {
            sAPI_MsgQDelete(ftpMsgQueue);
            ftpMsgQueue = NULL;
            ftp_error_code = msg.arg2;
            loge("Ftp init");
            break;
        }

        object = &ftp_object;
        object->simcom = simcom;
        object->proc = &ftp_proc;
        logi("Ftp create successs");
    } while (0);
    return object;
}

void ftp_dispose(ftp_t* ftp) {
    SIM_MSG_T msg;
    sAPI_FtpsLogout();
    sAPI_FtpsDeInit(SC_FTPS_USB);
    sAPI_MsgQRecv(ftpMsgQueue, &msg, SC_SUSPEND);
    if (msg.arg3 != NULL)
        ufree(msg.arg3);
    sAPI_MsgQDelete(ftpMsgQueue);
    ftpMsgQueue = NULL;
    ftp_error_code = msg.arg2;
    logi("Ftp deinit -> %d", ftp_error_code);
}

int ftp_connect(ftp_t* ftp, const char* server, int port, const char* user, const char* password) {
    SIM_MSG_T msg;
    SCftpsLoginMsg ftpMsg = {0};

    ftpMsg.serverType = 0;
    ftpMsg.port = port;
    umemcpy(ftpMsg.host, server, strlen(server));
    umemcpy(ftpMsg.username, user, strlen(user));
    umemcpy(ftpMsg.password, password, strlen(password));

    sAPI_FtpsLogin(ftpMsg);
    sAPI_MsgQRecv(ftpMsgQueue, &msg, SC_SUSPEND);
    ftp_error_code = msg.arg2;
    if (msg.arg3 != NULL)
        ufree(msg.arg3);
    logi("Ftp connect -> %d", ftp_error_code);
    return ftp_error_code;
}

int ftp_upload(ftp_t* ftp, const char* path, const char* file) {
    SIM_MSG_T msg;
    ftp_error_code = 0;

    if (path != NULL) {
        sAPI_FtpsChangeDirectory((char*)path);
        sAPI_MsgQRecv(ftpMsgQueue, &msg, SC_SUSPEND);
        if (msg.arg3 != NULL) {
            ufree(msg.arg3);
            msg.arg3 = NULL;
        }
        ftp_error_code = msg.arg2;
    }

    if (ftp_error_code == 0) {
        sAPI_FtpsUploadFile((char*)file, SC_FTPS_FILE_FLASH, 0);
        sAPI_MsgQRecv(ftpMsgQueue, &msg, SC_SUSPEND);
        if (msg.arg3 != NULL)
            ufree(msg.arg3);
        ftp_error_code = msg.arg2;
    }

    logi("Ftp upload %s -> %d", file, ftp_error_code);
    return ftp_error_code;
}