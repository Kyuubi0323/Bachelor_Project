/*
 * The little filesystem
 *
 * Copyright (c) 2019, ASR Limited. All rights reserved.
 *
 */
#ifndef LFS_API_H
#define LFS_API_H

#include "lfs.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef RT_DEF_LFS_DRIVERS
    #define RT_DEF_LFS_DRIVERS 1
#endif

#if (RT_DEF_LFS_DRIVERS < 1)
    #error "#define RT_DEF_LFS_DRIVERS must > 0"
#endif

#ifndef LFS_READ_SIZE
    #define LFS_READ_SIZE 4096
#endif

#ifndef LFS_PROG_SIZE
    #define LFS_PROG_SIZE 4096
#endif

#ifndef LFS_BLOCK_SIZE
    #define LFS_BLOCK_SIZE 4096
#endif

#ifndef LFS_LOOKAHEAD
    #define LFS_LOOKAHEAD 512
#endif

#ifndef LFS_HEAD_ID
    #define LFS_HEAD_ID 0x4853464C
#endif

#ifndef LFS_TAIL_ID
    #define LFS_TAIL_ID 0x5453464C
#endif

#ifndef LFS_FILE_ID
    #define LFS_FILE_ID 0x454C4946
#endif

#ifndef LFS_DIR_ID
    #define LFS_DIR_ID 0x44524944
#endif

#ifndef LFS_CKSUM_ID
    #define LFS_CKSUM_ID 0x4D534B43
#endif

#ifndef LFS_BLOCK_CYCLES
    #define LFS_BLOCK_CYCLES 999
#endif

/* Wildcard for single character */
#define LFS_SCWILDCARD      '?'

/* Wildcard for multiple characters */
#define LFS_MCWILDCARD      '*'

/* Terminating character marks end of string */
#define LFS_EOS             '\0'

/* Updater check version */
#define UPDATER_CK_VER      "20190911"

/* Updater version size */
#define UPDATER_VER_SIZE    8

/* Updater version offset */
#define UPDATER_VER_OFFSET  0x280

/*
 * Description:
 * Maximum number of open files
 *
 * Remarks:
 * Each handle takes 36 bytes
 *
 */
#define LFS_NUM_HANDLES		8

#define LFS_FREE_HANDLE		0x00

#define LFS_BAD_HANDLE		-1

typedef enum lfs_fs_type
{
    LFS_CUST_FS     = 0x00,
    LFS_NVM_FS      = 0x01,
    LFS_BACKUP_FS   = 0x02,
    LFS_MAX_FS      = 0xFF
} lfs_fs_type;

typedef struct _dfs_lfs_s
{
    struct lfs lfs;
    struct lfs_config cfg;
} dfs_lfs_t;

typedef struct _dfs_lfs_fd_s
{
    struct lfs_file file;
    UINT32 ErrCode;
} dfs_lfs_fd_t;

typedef struct _dfs_lfs_dir_s
{
    struct lfs_dir dir;
} dfs_lfs_dir_t;

typedef struct lfs_flash_info
{
    lfs_fs_type FsType;
    UINT32 LfsStartAddress;
    UINT32 LfsEndAddress;
} lfs_flash_info;

typedef struct
{
    UINT32 HeadID;
    UINT32 oflag;
    union
    {
        dfs_lfs_fd_t LfsFd;
        dfs_lfs_dir_t DirFd;
    } u;
    UINT32 CkSum;
    UINT32 TailID;
} lfs_sHandle;

/*===========================================================================

             EXTERN DEFINITIONS AND DECLARATIONS FOR MODULE

 ===========================================================================*/

/*===========================================================================

                          INTERNAL FUNCTION DECLARATIONS

===========================================================================*/
/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_load_config                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function load LFS configuration                              */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void lfs_load_config(struct lfs_config* lfs_cfg);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_root_is_valid                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function check whether root directory is valid or not.       */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_root_is_valid(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_sys_init                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function initialize LFS system                               */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_sys_init(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_open                                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function Open or create a file for read/write.               */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_open(const char *filename, UINT32 oflag, UINT32 pmode);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_close                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function close a lfs file.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_close(int Handle);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_read                                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function read data from file.                                */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_read(int Handle, void* buf, UINT32 len);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_readEx                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function read data from file.                                */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_readEx(int Handle, void* buf, UINT32 len, UINT32 filepostoread);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_write                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function write data to file.                                 */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_write(int Handle, const void* buf, UINT32 len);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_ftell                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function get the file position.                              */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_ftell(int Handle);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_writeEx                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function write data to file.                                 */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_writeEx(int Handle, const void* buf, UINT32 len, UINT32 filePostowrite);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_lseek                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function Move file pointer to specified location.            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_lseek(int Handle, UINT32 offset, int whence);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_size                                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function get file size.                                      */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
UINT32 lfs_io_size(int Handle);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_eof                                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function to judge if it is the end of file                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_eof(int Handle);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_remove                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function remove a file from file system.                     */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_remove(const char* path);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_rename                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function rename a file in file system.                       */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_rename(const char* from, const char* to);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_Access                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function access a LFS file.                                  */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_Access(const char *path, UINT32 mode);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_statfs                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function get file statfs.                                    */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_statfs(UINT32 *Bytes);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_used                                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function get the used bytes of LFS.                          */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_used(UINT32 *Bytes);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_dir_open                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function Open or create a directory.                         */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_dir_open(const char *path);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_dir_read                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function read file from a directory.                         */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_dir_read(int Handle, struct lfs_info *info);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_io_dir_close                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function close a directory.                                  */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_dir_close(int Handle);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_dir_make                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function create a directory.                                 */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_io_dir_make(const char *path);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_fname_match                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function Checks to see if filenames match, including         */
/*      wildcards.                                                       */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int lfs_fname_match(const char *src, const char *dest);

/*****************************************************************************
* Function Name: lfs_io_findall()
*
* DESCRIPTION:
*    None.
*
* USAGE:
*    void lfs_io_findall(void)
*
* PARAMETERS:
*
* INPUTS:
*    None.
*

*
* OUTPUTS:
*
* RETURNS:
*  Returns TRUE if filename is valid; otherwise, it returns FALSE.
*
* HISTORY:
* 20.05.05 dmp Took from FDI5 from FDI_FILE.c:3875; and simplifyed.
*
***************************************************************************/
void lfs_io_findall(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_file_verify                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function verify file validity                                */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      block                                                            */
/*      buffer                                                           */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
BOOL lfs_file_verify(int Handle, BOOL set);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
