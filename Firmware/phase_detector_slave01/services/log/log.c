#include "log.h"

#ifdef _DEBUG

#if (LOG_SIZE_DEFAULT > 0)
static char log_buff_mem[LOG_SIZE_DEFAULT] = {0};
char* LOG_BUFF = log_buff_mem;
int LOG_SIZE = LOG_SIZE_DEFAULT;
#else
char* LOG_BUFF = null;
int LOG_SIZE = 0;
#endif

void ICACHE_FLASH_ATTR log_init(int size)
{
#if (LOG_SIZE_DEFAULT == 0)
    if ((size > 0) && (LOG_BUFF == null)) {
        LOG_SIZE = size;
        LOG_BUFF = (char*)ualloc(size);
        umemset((void*)LOG_BUFF, 0, size);
    }
#endif
    if (LOG_BUFF == null) {
        uprintf("<<<<<<<<<<<< ERROR ALLOCATE MEMORY LOG >>>>>>>>>>>>>\r\n");
        sysDump();
    }
}

void ICACHE_FLASH_ATTR log_deinit(void)
{
#if (LOG_SIZE_DEFAULT == 0)
    if (LOG_BUFF != null) {
        ufree(LOG_BUFF);
        LOG_SIZE = 0;
    }
#endif
}

#endif