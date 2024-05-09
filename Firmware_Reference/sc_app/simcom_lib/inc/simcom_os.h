/**
  ******************************************************************************
  * @file    simcom_os.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of system APIs.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef _SIMCOM_OS_H
#define _SIMCOM_OS_H

//#include "simcom_common.h"

#ifndef UINT8
typedef unsigned char   UINT8;
#endif

#ifndef UINT16
typedef unsigned short  UINT16;
#endif

#ifndef UINT32
typedef unsigned int   UINT32;
#endif

#ifndef UINT64
typedef unsigned long long  UINT64;
#endif

#ifndef BOOL
typedef unsigned char   BOOL;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef INT8
typedef char            INT8;
#endif

#ifndef CHAR
typedef char            CHAR;
#endif


#ifndef INT16
typedef signed short    INT16;
#endif

#ifndef INT32
typedef int             INT32;
#endif

#ifndef INT64
typedef long long       INT64;
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef void*   sTaskRef;
typedef void*   sSemaRef;
typedef void*   sMutexRef;
typedef void*   sMsgQRef;
typedef void*   sTimerRef;
typedef void*   sFlagRef;

typedef struct sim_msg_cell
{
    UINT32 msg_id;
    int arg1;
    int arg2;
    void *arg3;
} SIM_MSG_T;

typedef enum
{
	SC_TASK_READY,
	SC_TASK_COMPLETED,
	SC_TASK_TERMINATED,
	SC_TASK_SUSPENDED,
	SC_TASK_SLEEP,
	SC_TASK_QUEUE_SUSP,
	SC_TASK_SEMAPHORE_SUSP,
	SC_TASK_EVENT_FLAG,
	SC_TASK_BLOCK_MEMORY,
	SC_TASK_MUTEX_SUSP,
	SC_TASK_STATE_UNKNOWN,
}SC_TASK_STATE;

typedef struct
{
    char                *task_name;                /* Pointer to thread's name     */
    unsigned int        task_priority;             /* Priority of thread (0-255)  */
    unsigned long        task_stack_def_val;             /* default vaule of  thread  */
    SC_TASK_STATE      task_state;                /* Thread's execution state     */
    unsigned long       task_stack_ptr;           /* Thread's stack pointer   */
    unsigned long       task_stack_start;         /* Stack starting address   */
    unsigned long       task_stack_end;           /* Stack ending address     */
    unsigned long       task_stack_size;           /* Stack size               */
    unsigned long       task_run_count;            /* Thread's run counter     */

} sTaskInfo;

typedef struct
{
    UINT32  status;         /* timer status SC_ENABLED, SC_DISABLED    */
    UINT32  expirations;    /* number of expirations for cyclic timers */
}sTimerStatus;

typedef struct
{
	int tv_sec;        /* seconds */
	int tv_usec;       /* and microseconds */
}sTimeval;

#define SC_MIN_STACK_SIZE      256
#define SC_ENABLE_INTERRUPTS   1
#define SC_DISABLE_INTERRUPTS  2
#define SC_SUSPEND             0xFFFFFFFF
#define SC_NO_SUSPEND          0
#define SC_FLAG_AND            5
#define SC_FLAG_AND_CLEAR      6
#define SC_FLAG_OR             7
#define SC_FLAG_OR_CLEAR       8
#define SC_FIXED               9
#define SC_VARIABLE            10
#define SC_FIFO                11
#define SC_PRIORITY            12
#define SC_GLOBAL              13
#define SC_OS_INDEPENDENT      14
#define SC_ENABLED             2
#define SC_DISABLED            3

#define SC_TIMER_DEAD          0
#define SC_TIMER_CREATED       1
#define SC_TIMER_ACTIVE        2
#define SC_TIMER_INACTIVE      3


#define SC_DEFAULT_TASK_PRIORITY    90
#define SC_DEFAULT_THREAD_STACKSIZE 4096
/*========================================================================
 *  SC Return Error Codes
 *========================================================================*/

typedef enum
{
    SC_SUCCESS = 0,        /* 0x0 -no errors                                        */
    SC_FAIL,               /* 0x1 -operation failed code                            */
    SC_TIMEOUT,            /* 0x2 -Timed out waiting for a resource                 */
    SC_NO_RESOURCES,       /* 0x3 -Internal OS resources expired                    */
    SC_INVALID_POINTER,    /* 0x4 -0 or out of range pointer value                  */
    SC_INVALID_REF,        /* 0x5 -invalid reference                                */
    SC_INVALID_DELETE,     /* 0x6 -deleting an unterminated task                    */
    SC_INVALID_PTR,        /* 0x7 -invalid memory pointer                           */
    SC_INVALID_MEMORY,     /* 0x8 -invalid memory pointer                           */
    SC_INVALID_SIZE,       /* 0x9 -out of range size argument                       */
    SC_INVALID_MODE,       /* 0xA, 10 -invalid mode                                 */
    SC_INVALID_PRIORITY,   /* 0xB, 11 -out of range task priority                   */
    SC_UNAVAILABLE,        /* 0xC, 12 -Service requested was unavailable or in use  */
    SC_POOL_EMPTY,         /* 0xD, 13 -no resources in resource pool                */
    SC_QUEUE_FULL,         /* 0xE, 14 -attempt to send to full messaging queue      */
    SC_QUEUE_EMPTY,        /* 0xF, 15 -no messages on the queue                     */
    SC_NO_MEMORY,          /* 0x10, 16 -no memory left                              */
    SC_DELETED,            /* 0x11, 17 -service was deleted                         */
    SC_SEM_DELETED,        /* 0x12, 18 -semaphore was deleted                       */
    SC_MUTEX_DELETED,      /* 0x13, 19 -mutex was deleted                           */
    SC_MSGQ_DELETED,       /* 0x14, 20 -msg Q was deleted                           */
    SC_MBOX_DELETED,       /* 0x15, 21 -mailbox Q was deleted                       */
    SC_FLAG_DELETED,       /* 0x16, 22 -flag was deleted                            */
    SC_INVALID_VECTOR,     /* 0x17, 23 -interrupt vector is invalid                 */
    SC_NO_TASKS,           /* 0x18, 24 -exceeded max # of tasks in the system       */
    SC_NO_FLAGS,           /* 0x19, 25 -exceeded max # of flags in the system       */
    SC_NO_SEMAPHORES,      /* 0x1A, 26 -exceeded max # of semaphores in the system  */
    SC_NO_MUTEXES,         /* 0x1B, 27 -exceeded max # of mutexes in the system     */
    SC_NO_QUEUES,          /* 0x1C, 28 -exceeded max # of msg queues in the system  */
    SC_NO_MBOXES,          /* 0x1D, 29 -exceeded max # of mbox queues in the system */
    SC_NO_TIMERS,          /* 0x1E, 30 -exceeded max # of timers in the system      */
    SC_NO_MEM_POOLS,       /* 0x1F, 31 -exceeded max # of mem pools in the system   */
    SC_NO_INTERRUPTS,      /* 0x20, 32 -exceeded max # of isr's in the system       */
    SC_FLAG_NOT_PRESENT,   /* 0x21, 33 -requested flag combination not present      */
    SC_UNSUPPORTED,        /* 0x22, 34 -service is not supported by the OS          */
    SC_NO_MEM_CELLS,       /* 0x23, 35 -no global memory cells                      */
    SC_DUPLICATE_NAME,     /* 0x24, 36 -duplicate global memory cell name           */
    SC_INVALID_PARM        /* 0x25, 37 -invalid parameter                           */
}SC_STATUS;

#endif
