/**
  ******************************************************************************
  * @file    simcom_tcpip.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of internal tcp\ip stack.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef _SIMCOM_TCPIP_H_
#define _SIMCOM_TCPIP_H_
#include "simcom_os.h"

#define SC_FD_SETSIZE      128

#define SC_SOL_SOCKET      0xfff    /* options for socket level */
#define SC_SO_ERROR        0x1007


#define SC_ECONNRESET      104
#define SC_ECONNABORTED    103  /* Software caused connection abort */

#define SC_AF_INET         2
#define SC_AF_INET6        10
#define SC_INADDR_ANY      ((INT32)0x00000000)

/* Socket protocol types (TCP/UDP/RAW) */
#define SC_SOCK_STREAM     1
#define SC_SOCK_DGRAM      2
#define SC_SOCK_RAW        3

#define SC_INVALID_SOCKET  (0xFFFFFFFFL)
#define SC_SOCKET_ERROR    (0xFFFFFFFFL)

#define SC_SO_ACCEPTCONN   0x0002 /* socket has had listen() */
#define SC_SO_REUSEADDR    0x0004 /* Allow local address reuse */
#define SC_SO_KEEPALIVE    0x0008 /* keep connections alive */

#define SC_TCP_NODELAY         0x01    /* don't delay send to coalesce packets */
#define SC_TCP_KEEPALIVE       0x02    /* send KEEPALIVE probes when idle for pcb->keep_idle milliseconds */
#define SC_TCP_KEEPIDLE        0x03    /* set pcb->keep_idle  - Same as TCP_KEEPALIVE, but use seconds for get/setsockopt */
#define SC_TCP_KEEPINTVL       0x04    /* set pcb->keep_intvl - Use seconds for get/setsockopt */
#define SC_TCP_KEEPCNT         0x05    /* set pcb->keep_cnt   - Use number of probes sent for get/setsockopt */
#define SC_TCP_TIMEROUT        0x06    /* set pcb->TCP_TIMEROUT   - Use number of probes sent for get/setsockopt */
#define SC_TCP_MSS_VALUE       0x07    /* set pcb->mss   - Use number of probes only for get/setsockopt */
#define SC_TCP_RTO_VALUE       0x08    /* set pcb->rto 	- Use number of probes only for get/setsockopt */
#define SC_TCP_TXB_UNACK       0x09    /* set (pcb->snd_lbb - pcb->lastack) - how much byte remaind in buffer for unacked */
#define SC_TCP_TXB_REST        0x0A    /* set (pcb->snd_buf) - how much byte remaind in tx buffer can writen by app */
#define SC_TCP_TXB_ACKED       0x0B    /* set (pcb->acked_sum) - the number of accumulation of acked */
#define SC_TCP_CLOSE_TIMEROUT  0x0C    /* set (pcb->close_timeout) - the timerout set of tcp close all */
#define SC_TCP_RST_TRIGGER     0x0D    /* set reset man trigger - call tcp_reset_man_trigger() */
#define SC_TCP_INFO            0x0E    /* get tcp_info */

/*
 * Option flags per-socket.
 */
#define SC_SO_DEBUG        0x0001 /* Unimplemented: turn on debugging info recording */
#define SC_SO_ACCEPTCONN   0x0002 /* socket has had listen() */
#define SC_SO_REUSEADDR    0x0004 /* Allow local address reuse */
#define SC_SO_KEEPALIVE    0x0008 /* keep connections alive */
#define SC_SO_DONTROUTE    0x0010 /* Unimplemented: just use interface addresses */
#define SC_SO_BROADCAST    0x0020 /* permit to send and to receive broadcast messages (see IP_SOF_BROADCAST option) */
#define SC_SO_USELOOPBACK  0x0040 /* Unimplemented: bypass hardware when possible */
#define SC_SO_LINGER       0x0080 /* linger on close if data present */
#define SC_SO_DONTLINGER   ((int)(~SC_SO_LINGER))
#define SC_SO_OOBINLINE    0x0100 /* Unimplemented: leave received OOB data in line */
#define SC_SO_REUSEPORT    0x0200 /* Unimplemented: allow local address & port reuse */
#define SC_SO_IPSEC        0x0400 /* denote ipsec using */

#define SC_SO_SNDBUF       0x1001  /* Unimplemented: send buffer size */
#define SC_SO_RCVBUF       0x1002  /* receive buffer size */
#define SC_SO_SNDLOWAT     0x1003  /* Unimplemented: send low-water mark */
#define SC_SO_RCVLOWAT     0x1004  /* Unimplemented: receive low-water mark */
#define SC_SO_SNDTIMEO     0x1005  /* Unimplemented: send timeout */
#define SC_SO_RCVTIMEO     0x1006  /* receive timeout */
#define SC_SO_ERROR        0x1007  /* get error status and clear */
#define SC_SO_TYPE         0x1008  /* get socket type */
#define SC_SO_CONTIMEO     0x1009  /* Unimplemented: connect timeout */
#define SC_SO_NO_CHECK     0x100a  /* don't create UDP checksum */
#define SC_SO_BINDTODEVICE 0x100b /* bind to device */
#define SC_SO_BIO          0x100c  /* set socket into blocking mode */
#define SC_SO_NBIO         0x100d  /* set socket into non-blocking mode */
#define SC_SO_NONBLOCK     0x100e  /* set/get socket blocking mode via optval param*/


/** 255.255.255.255 */
#define SC_IPADDR_NONE         ((UINT32)0xffffffffUL)

#define SC_IPPROTO_IP      0
#define SC_IPPROTO_TCP     6
#define SC_IPPROTO_UDP     17

/* Flags we can use with sAPI_TcpipSend and sAPI_TcpipRecv. */
#define SC_MSG_PEEK       0x01    /* Peeks at an incoming message */
#define SC_MSG_WAITALL    0x02    /* Unimplemented: Requests that the function block until the full amount of data requested can be returned */
#define SC_MSG_OOB        0x04    /* Unimplemented: Requests out-of-band data. The significance and semantics of out-of-band data are protocol-specific */
#define SC_MSG_DONTWAIT   0x08    /* Nonblocking i/o for this operation only */
#define SC_MSG_MORE       0x10    /* Sender will send more */

/*LWIP_PROVIDE_ERRNO*/
#define  SC_ERROK            0  /* err ok set, no err happen */
#define  SC_EPERM            1  /* Operation not permitted */
#define  SC_ENOENT           2  /* No such file or directory */
#define  SC_ESRCH            3  /* No such process */
#define  SC_EINTR            4  /* Interrupted system call */
#define  SC_EIO              5  /* I/O error */
#define  SC_ENXIO            6  /* No such device or address */
#define  SC_E2BIG            7  /* Arg list too long */
#define  SC_ENOEXEC          8  /* Exec format error */
#define  SC_EBADF            9  /* Bad file number */
#define  SC_ECHILD          10  /* No child processes */
#define  SC_EAGAIN          11  /* Try again */
#define  SC_ENOMEM          12  /* Out of memory */
#define  SC_EACCES          13  /* Permission denied */
#define  SC_EFAULT          14  /* Bad address */
#define  SC_ENOTBLK         15  /* Block device required */
#define  SC_EBUSY           16  /* Device or resource busy */
#define  SC_EEXIST          17  /* File exists */
#define  SC_EXDEV           18  /* Cross-device link */
#define  SC_ENODEV          19  /* No such device */
#define  SC_ENOTDIR         20  /* Not a directory */
#define  SC_EISDIR          21  /* Is a directory */
#define  SC_EINVAL          22  /* Invalid argument */
#define  SC_ENFILE          23  /* File table overflow */
#define  SC_EMFILE          24  /* Too many open files */
#define  SC_ENOTTY          25  /* Not a typewriter */
#define  SC_ETXTBSY         26  /* Text file busy */
#define  SC_EFBIG           27  /* File too large */
#define  SC_ENOSPC          28  /* No space left on device */
#define  SC_ESPIPE          29  /* Illegal seek */
#define  SC_EROFS           30  /* Read-only file system */
#define  SC_EMLINK          31  /* Too many links */
#define  SC_EPIPE           32  /* Broken pipe */
#define  SC_LWIPEDOM        33  /* Math argument out of domain of func */
#define  SC_LWIPERANGE      34  /* Math result not representable */
#define  SC_EDEADLK         35  /* Resource deadlock would occur */
#define  SC_ENAMETOOLONG    36  /* File name too long */
#define  SC_ENOLCK          37  /* No record locks available */
#define  SC_ENOSYS          38  /* Function not implemented */
#define  SC_ENOTEMPTY       39  /* Directory not empty */
#define  SC_ELOOP           40  /* Too many symbolic links encountered */
#define  SC_EWOULDBLOCK     SC_EAGAIN  /* Operation would block */
#define  SC_ENOMSG          42  /* No message of desired type */
#define  SC_EIDRM           43  /* Identifier removed */
#define  SC_ECHRNG          44  /* Channel number out of range */
#define  SC_EL2NSYNC        45  /* Level 2 not synchronized */
#define  SC_EL3HLT          46  /* Level 3 halted */
#define  SC_EL3RST          47  /* Level 3 reset */
#define  SC_ELNRNG          48  /* Link number out of range */
#define  SC_EUNATCH         49  /* Protocol driver not attached */
#define  SC_ENOCSI          50  /* No CSI structure available */
#define  SC_EL2HLT          51  /* Level 2 halted */
#define  SC_EBADE           52  /* Invalid exchange */
#define  SC_EBADR           53  /* Invalid request descriptor */
#define  SC_EXFULL          54  /* Exchange full */
#define  SC_ENOANO          55  /* No anode */
#define  SC_EBADRQC         56  /* Invalid request code */
#define  SC_EBADSLT         57  /* Invalid slot */
#define  SC_EDEADLOCK       SC_EDEADLK
#define  SC_EBFONT          59  /* Bad font file format */
#define  SC_ENOSTR          60  /* Device not a stream */
#define  SC_ENODATA         61  /* No data available */
#define  SC_ETIME           62  /* Timer expired */
#define  SC_ENOSR           63  /* Out of streams resources */
#define  SC_ENONET          64  /* Machine is not on the network */
#define  SC_ENOPKG          65  /* Package not installed */
#define  SC_EREMOTE         66  /* Object is remote */
#define  SC_ENOLINK         67  /* Link has been severed */
#define  SC_EADV            68  /* Advertise error */
#define  SC_ESRMNT          69  /* Srmount error */
#define  SC_ECOMM           70  /* Communication error on send */
#define  SC_EPROTO          71  /* Protocol error */
#define  SC_EMULTIHOP       72  /* Multihop attempted */
#define  SC_EDOTDOT         73  /* RFS specific error */
#define  SC_EBADMSG         74  /* Not a data message */
#define  SC_EOVERFLOW       75  /* Value too large for defined data type */
#define  SC_ENOTUNIQ        76  /* Name not unique on network */
#define  SC_EBADFD          77  /* File descriptor in bad state */
#define  SC_EREMCHG         78  /* Remote address changed */
#define  SC_ELIBACC         79  /* Can not access a needed shared library */
#define  SC_ELIBBAD         80  /* Accessing a corrupted shared library */
#define  SC_ELIBSCN         81  /* .lib section in a.out corrupted */
#define  SC_ELIBMAX         82  /* Attempting to link in too many shared libraries */
#define  SC_ELIBEXEC        83  /* Cannot exec a shared library directly */
#define  SC_LWIPEILSEQ      84  /* Illegal byte sequence */
#define  SC_ERESTART        85  /* Interrupted system call should be restarted */
#define  SC_ESTRPIPE        86  /* Streams pipe error */
#define  SC_EUSERS          87  /* Too many users */
#define  SC_ENOTSOCK        88  /* Socket operation on non-socket */
#define  SC_EDESTADDRREQ    89  /* Destination address required */
#define  SC_EMSGSIZE        90  /* Message too long */
#define  SC_EPROTOTYPE      91  /* Protocol wrong type for socket */
#define  SC_ENOPROTOOPT     92  /* Protocol not available */
#define  SC_EPROTONOSUPPORT 93  /* Protocol not supported */
#define  SC_ESOCKTNOSUPPORT 94  /* Socket type not supported */
#define  SC_EOPNOTSUPP      95  /* Operation not supported on transport endpoint */
#define  SC_EPFNOSUPPORT    96  /* Protocol family not supported */
#define  SC_EAFNOSUPPORT    97  /* Address family not supported by protocol */
#define  SC_EADDRINUSE      98  /* Address already in use */
#define  SC_EADDRNOTAVAIL   99  /* Cannot assign requested address */
#define  SC_ENETDOWN       100  /* Network is down */
#define  SC_ENETUNREACH    101  /* Network is unreachable */
#define  SC_ENETRESET      102  /* Network dropped connection because of reset */
#define  SC_ECONNABORTED   103  /* Software caused connection abort */
#define  SC_ECONNRESET     104  /* Connection reset by peer */
#define  SC_ENOBUFS        105  /* No buffer space available */
#define  SC_EISCONN        106  /* Transport endpoint is already connected */
#define  SC_ENOTCONN       107  /* Transport endpoint is not connected */
#define  SC_ESHUTDOWN      108  /* Cannot send after transport endpoint shutdown */
#define  SC_ETOOMANYREFS   109  /* Too many references: cannot splice */
#define  SC_ETIMEDOUT      110  /* Connection timed out */
#define  SC_ECONNREFUSED   111  /* Connection refused */
#define  SC_EHOSTDOWN      112  /* Host is down */
#define  SC_EHOSTUNREACH   113  /* No route to host */
#define  SC_EALREADY       114  /* Operation already in progress */
#define  SC_EINPROGRESS    115  /* Operation now in progress */
#define  SC_ESTALE         116  /* Stale NFS file handle */
#define  SC_EUCLEAN        117  /* Structure needs cleaning */
#define  SC_ENOTNAM        118  /* Not a XENIX named type file */
#define  SC_ENAVAIL        119  /* No XENIX semaphores available */
#define  SC_EISNAM         120  /* Is a named type file */
#define  SC_EREMOTEIO      121  /* Remote I/O error */
#define  SC_EDQUOT         122  /* Quota exceeded */
#define  SC_ENOMEDIUM      123  /* No medium found */
#define  SC_EMEDIUMTYPE    124  /* Wrong medium type */

#define SC_TCPIP_SUCCESS      0
#define SC_TCPIP_FAIL        -1


typedef struct 
{
	INT32 tv_sec;        /* seconds */
	INT32 tv_usec;       /* and microseconds */
}SCtimeval;

enum SCnetType
{
	TCPIP_PDP_IPV4V6,
	TCPIP_PDP_IPV4,
	TCPIP_PDP_IPV6,

	TCPIP_PDP_INVALID
};


struct SCipInfo
{
  enum SCnetType type;
  UINT32 ip4;
  UINT32 ip6[4];
};

typedef struct{
  UINT32 s_addr;
}SCinAddr;

typedef struct  {
  UINT8 sin_len;
  UINT8 sin_family;
  UINT16 sin_port;
  SCinAddr sin_addr;
#define SC_SIN_ZERO_LEN 8
  INT8 sin_zero[SC_SIN_ZERO_LEN];
}SCsockAddrIn;



typedef struct  {
  UINT8 fd_bits [(SC_FD_SETSIZE * 2 + 7)/8];
} SCfdSet;


 typedef struct  {
    INT8  *h_name;      /* Official name of the host. */
    INT8 **h_aliases;   /* A pointer to an array of pointers to alternative host names,
                           terminated by a null pointer. */
    INT32    h_addrtype;  /* Address type. */
    INT32    h_length;    /* The length, in bytes, of the address. */
    INT8 **h_addr_list; /* A pointer to an array of pointers to network addresses (in
                           network byte order) for the host, terminated by a null pointer. */
#define h_addr h_addr_list[0] /* for backward compatibility */
}SChostent;




typedef struct{
  UINT8 sa_len;
  UINT8 sa_family;
#if LWIP_IPV6
  UINT8 sa_data[22];
#else /* LWIP_IPV6 */
  UINT8 sa_data[14];
#endif /* LWIP_IPV6 */
}SCsockAddr;

#define SC_IOCPARM_MASK    0x7fU           /* parameters must be < 128 bytes */
#define SC_IOC_VOID        0x20000000UL    /* no parameters */
#define SC_IOC_OUT         0x40000000UL    /* copy out parameters */
#define SC_IOC_IN          0x80000000UL    /* copy in parameters */
#define SC_IOC_INOUT       (SC_IOC_IN|SC_IOC_OUT)
                                        /* 0x20000000 distinguishes new &
                                           old ioctl's */
#define SC_IO(x,y)        (SC_IOC_VOID|((x)<<8)|(y))

#define SC_IOR(x,y,t)     (SC_IOC_OUT|(((UINT32)sizeof(t)&SC_IOCPARM_MASK)<<16)|((x)<<8)|(y))

#define SC_IOW(x,y,t)     (SC_IOC_IN|(((UINT32)sizeof(t)&SC_IOCPARM_MASK)<<16)|((x)<<8)|(y))

#define SC_FIONBIO     SC_IOW('f', 126, UINT32) /* set/clear non-blocking i/o */

#define SC_SOCKET_OFFSET  (0) /*note: must set to 0*/

#define SC_MEMP_NUM_NETCONN 128

#define SC_FDSETSAFESET(n, code)  do {\
    if ((((int)(n) - SC_SOCKET_OFFSET) < (SC_MEMP_NUM_NETCONN * 2)) \
       && (((int)(n) - SC_SOCKET_OFFSET) >= 0)) {code;}\
    } while(0)
    
#define SC_FDSETSAFEGET(n, code) (((((int)(n) - SC_SOCKET_OFFSET) < (SC_MEMP_NUM_NETCONN * 2)) \
       && (((int)(n) - SC_SOCKET_OFFSET) >= 0)) ? (code) : 0)   

#define SC_FD_SET(n, p)  SC_FDSETSAFESET(n, (p)->fd_bits[((n) - SC_SOCKET_OFFSET)/8] |=  (1 << (((n) - SC_SOCKET_OFFSET) & 7)))
#define SC_FD_CLR(n, p)  SC_FDSETSAFESET(n, (p)->fd_bits[((n) - SC_SOCKET_OFFSET)/8] &= ~(1 << (((n) - SC_SOCKET_OFFSET) & 7)))
#define SC_FD_ISSET(n,p) SC_FDSETSAFEGET(n, (p)->fd_bits[((n) - SC_SOCKET_OFFSET)/8] &   (1 << (((n) - SC_SOCKET_OFFSET) & 7)))
#define SC_FD_ZERO(p)    memset((void*)(p),0,sizeof(*(p)))


#define SC_SHUT_RD   0
#define SC_SHUT_WR   1
#define SC_SHUT_RDWR 2


enum socket_state
{
    SOCKET_NONE,
    SOCKET_WRITE,
    SOCKET_LISTEN,
    SOCKET_CONNECT,
    SOCKET_CLOSE
};

#endif



