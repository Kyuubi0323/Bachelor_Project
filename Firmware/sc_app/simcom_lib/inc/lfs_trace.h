#ifndef	LFS_TRACE_H
#define LFS_TRACE_H
/*===========================================================================

    LFS trace Header File

DESCRIPTION
  This header file contains all the definitions necessary for
  clients to interface with the Dial module.

Copyright (c) 2011 by MARVELL INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who         what, where, why
--------   ------     -------------------------------------------------------
11/16/08   zhoujin      Create lfs trace header file

===========================================================================*/

//#include "utils.h"
//#include "utilities.h"

/*===========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/

/* -------------------------------------------------------------------
** FATSYS trace debug.
** -------------------------------------------------------------------
*/

#define FATSYS_TRACE(fmt,args...)                                           \
{                                                                           \
    if (LFS_TRACE_ENABLE)                                                   \
    {                                                                       \
        CPUartLogPrintf("lfs: "fmt, ##args);                                \
    }                                                                       \
}

/* -------------------------------------------------------------------
** LFS trace debug.
** -------------------------------------------------------------------
*/

#define LFS_TRACE(fmt,args...)                                              \
{                                                                           \
    if (LFS_TRACE_ENABLE)                                                   \
    {                                                                       \
        CPUartLogPrintf("lfs: "fmt, ##args);                                \
    }                                                                       \
}

/* -------------------------------------------------------------------
** FAT system ASSERT.
** -------------------------------------------------------------------
*/

#define FAT_ASSERT(cOND) { if (!(cOND))  { utilsAssertFail(#cOND, __FILE__,   __LINE__, 1);}}

/* -------------------------------------------------------------------
** LFS debug flag.
** -------------------------------------------------------------------
*/
#define LFS_TRACE_ENABLE     LfsTraceEnable

/*===========================================================================

            EXTERN DECLARATIONS FOR MODULE

===========================================================================*/
extern BOOL LfsTraceEnable;

/*===========================================================================

                        EXTERN FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

                          INTERNAL FUNCTION DECLARATIONS

===========================================================================*/

#endif /* LFS_TRACE_H */
