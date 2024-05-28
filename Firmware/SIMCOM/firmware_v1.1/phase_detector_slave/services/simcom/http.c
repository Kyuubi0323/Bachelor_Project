#include "sdk.h"
#include "log.h"
#include "system.h"
#include "simcom.h"
#include "http.h"

static http_p http_proc = {
    http_close,
    http_get
};
static http_t http_object = {0};

// static int http_buffer_size = 0;
// static uint8_t* http_buffer = NULL;

static int http_error_code = 0;
static int http_status_code = 0;

// static void on_http_action(void* data);
// static void on_http_read(void* data);

static sMsgQRef httpMsgQ = NULL;

http_t* http_create(simcom_t* simcom) {
    http_t* object = NULL;
    do {
        http_error_code = SC_SUCCESS;
        if (httpMsgQ == NULL)
            http_error_code = sAPI_MsgQCreate(&httpMsgQ, "Http Queue", sizeof(SIM_MSG_T), 8, SC_FIFO);
        if (http_error_code != SC_SUCCESS)
            break;
        http_error_code = sAPI_HttpInit(1, httpMsgQ);
        if (http_error_code == SC_HTTPS_SUCCESS) {
            object = &http_object;
            object->proc = &http_proc;
            object->simcom = simcom;
        }
    } while (0);
    
    logi("Http init %d", http_error_code);
    return object;
}

int http_close(http_t* http) {
    http_error_code = sAPI_HttpTerm();
    if (httpMsgQ != NULL) {
        sAPI_MsgQDelete(httpMsgQ);
        httpMsgQ = NULL;
    }
    logi("Http close %d", http_error_code);
    return http_error_code;
}

int http_get(http_t* http, const char* url, uint8_t* buffer, int size) {
    int reval = -1;
    uint32_t tick = 0;
    SChttpApiTrans *data = NULL;
    SIM_MSG_T msg = {SIM_MSG_INIT, 0, -1, NULL};

    do {
        http_status_code = 0;
        http_error_code = sAPI_HttpPara("URL", (char*)url);
        if (http_error_code != SC_HTTPS_SUCCESS)
            break;
        logi("Http url: %s", url);

        http_error_code = sAPI_HttpAction(0);
        if (http_error_code != SC_HTTPS_SUCCESS)
            break;

        tick = getSysTickCount();
        while ((uint32_t)(getSysTickCount() - tick) < 10000) {
            http_error_code = sAPI_MsgQRecv(httpMsgQ, &msg, SC_NO_SUSPEND);
            if (http_error_code != SC_SUCCESS)
                osleep(20);
            else
                break;
        }
        if (http_error_code != SC_SUCCESS)
            break;

        data = (SChttpApiTrans *)(msg.arg3);
        http_status_code = data->status_code;
        logi("Http action -> %d", http_status_code);

        ufree(data->data);
        ufree(data);

        if (http_status_code != 200)
            break;

        http_error_code = sAPI_HttpRead(1, 0, size);
        if (http_error_code != SC_HTTPS_SUCCESS)
            break;
        
        tick = getSysTickCount();
        while ((uint32_t)(getSysTickCount() - tick) < 10000) {
            http_error_code = sAPI_MsgQRecv(httpMsgQ, &msg, SC_NO_SUSPEND);
            if (http_error_code != SC_SUCCESS)
                osleep(20);
            else
                break;
        }
        if (http_error_code != SC_SUCCESS)
            break;
        
        data = (SChttpApiTrans *)(msg.arg3);
        http_status_code = data->status_code;
        logi("Http read -> status %d, size %d", http_status_code, data->dataLen);

        umemset(buffer, 0, size);
        umemcpy(buffer, data->data, data->dataLen);

        ufree(data->data);
        ufree(data);
        reval = 0;

        // http_buffer = buffer;
        // http_buffer_size = size;
        // http_error_code = sAPI_HttpAction(0);


        // SIM_MSG_T msgQ_data_recv = {SIM_MSG_INIT, 0, -1, NULL};
        // sAPI_MsgQRecv(httpMsgQ, &msgQ_data_recv, SC_SUSPEND);
        // SChttpApiTrans *sub_data = (SChttpApiTrans *)(msgQ_data_recv.arg3);
        // on_http_action(msgQ_data_recv.arg3);
        // ufree(sub_data->data);
        // ufree(sub_data);

        // logi("Http action: status = %d, len = %d", http_status_code, http_buffer_size);
        // if (http_status_code != 200)
        //     break;
        
        // reval = http_buffer_size;
        // http_buffer_size = size;
        // umemset((void*)buffer, 0, size);
        // http_error_code = sAPI_HttpRead(1, 0, reval);

        // sAPI_MsgQRecv(httpMsgQ, &msgQ_data_recv, SC_SUSPEND);
        // sub_data = (SChttpApiTrans *)(msgQ_data_recv.arg3);
        // on_http_read(msgQ_data_recv.arg3);
        // ufree(sub_data->data);
        // ufree(sub_data);

        // logi("Http read: len = %d", http_buffer_size);
    } while (0);

    // http_buffer = NULL;
    // simcom_register_http_event(NULL);
    return reval;
}

int http_get_error(void) {
    return http_error_code;
}

int http_get_status(void) {
    return http_status_code;
}

// static void on_http_action(void* data) {
//     SChttpApiTrans *result = (SChttpApiTrans*)data;
//     http_status_code = result->status_code;
//     if (result->action_content_len >= http_buffer_size)
//         http_buffer_size = result->action_content_len - 1;
//     else
//         http_buffer_size = result->action_content_len;
// }

// static void on_http_read(void* data) {
//     SChttpApiTrans *result = (SChttpApiTrans*)data;
//     if (result->dataLen >= http_buffer_size)
//         http_buffer_size = result->dataLen - 1;
//     else
//         http_buffer_size = result->dataLen;
//     umemcpy((void*)http_buffer, result->data, http_buffer_size);
// }