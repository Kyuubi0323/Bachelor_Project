/**
  ******************************************************************************
  * @file    socket.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of socket.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef _SOCKETS_H_
#define _SOCKETS_H_
#include "simcom_api.h"
#define  LWIP_IPV6 1

typedef UINT8       u8_t;
typedef UINT16      u16_t;
typedef UINT32      u32_t;

#define uint16_t    unsigned short

typedef u32_t socklen_t;

struct in_addr {
  u32_t s_addr;
};

struct in6_addr {
  union {
    u8_t  u8_addr[16];
    u8_t  u16_addr[8];
    u32_t u32_addr[4];
  } un;
#define s6_addr  un.u8_addr
#define s6_addr16  un.u16_addr
#define s6_addr32 un.u32_addr
};

/* members are in network byte order */
struct sockaddr_in {
  u8_t sin_len;
  u8_t sin_family;
  u16_t sin_port;
  struct in_addr sin_addr;
#define SIN_ZERO_LEN 8
  char sin_zero[SIN_ZERO_LEN];
};


#if LWIP_IPV6
struct sockaddr_in6 {
  u8_t sin6_len;             /* length of this structure */
  u8_t sin6_family;          /* AF_INET6                 */
  u16_t sin6_port;           /* Transport layer port #   */
  u32_t sin6_flowinfo;       /* IPv6 flow information    */
  struct in6_addr sin6_addr; /* IPv6 address             */
};
#endif /* LWIP_IPV6 */


struct sockaddr {
  u8_t sa_len;
  u8_t sa_family;
#if LWIP_IPV6
  u8_t sa_data[22];
#else /* LWIP_IPV6 */
  u8_t sa_data[14];
#endif /* LWIP_IPV6 */
};

/* Socket protocol types (TCP/UDP/RAW) */
#define SOCK_STREAM     1
#define SOCK_DGRAM      2
#define SOCK_RAW        3


#define  ERROK         0  /* err ok set, no err happen */
#define  EPERM            1  /* Operation not permitted */
#define  ENOENT           2  /* No such file or directory */
#define  ESRCH            3  /* No such process */
#define  EINTR            4  /* Interrupted system call */
#define  EIO              5  /* I/O error */
#define  ENXIO            6  /* No such device or address */
#define  E2BIG            7  /* Arg list too long */
#define  ENOEXEC          8  /* Exec format error */
#define  EBADF            9  /* Bad file number */
#define  ECHILD          10  /* No child processes */
#define  EAGAIN          11  /* Try again */
#define  ENOMEM          12  /* Out of memory */
#define  EACCES          13  /* Permission denied */
#define  EFAULT          14  /* Bad address */
#define  ENOTBLK         15  /* Block device required */
#define  EBUSY           16  /* Device or resource busy */
#define  EEXIST          17  /* File exists */
#define  EXDEV           18  /* Cross-device link */
#define  ENODEV          19  /* No such device */
#define  ENOTDIR         20  /* Not a directory */
#define  EISDIR          21  /* Is a directory */
#define  EINVAL          22  /* Invalid argument */
#define  ENFILE          23  /* File table overflow */
#define  EMFILE          24  /* Too many open files */
#define  ENOTTY          25  /* Not a typewriter */
#define  ETXTBSY         26  /* Text file busy */
#define  EFBIG           27  /* File too large */
#define  ENOSPC          28  /* No space left on device */
#define  ESPIPE          29  /* Illegal seek */
#define  EROFS           30  /* Read-only file system */
#define  EMLINK          31  /* Too many links */
#define  EPIPE           32  /* Broken pipe */
#define  EDOM            33  /* Math argument out of domain of func */
#define  ERANGE          34  /* Math result not representable */
#define  EDEADLK         35  /* Resource deadlock would occur */
#define  ENAMETOOLONG    36  /* File name too long */
#define  ENOLCK          37  /* No record locks available */
#define  ENOSYS          38  /* Function not implemented */
#define  ENOTEMPTY       39  /* Directory not empty */
#define  ELOOP           40  /* Too many symbolic links encountered */
#define  EWOULDBLOCK     EAGAIN  /* Operation would block */
#define  ENOMSG          42  /* No message of desired type */
#define  EIDRM           43  /* Identifier removed */
#define  ECHRNG          44  /* Channel number out of range */
#define  EL2NSYNC        45  /* Level 2 not synchronized */
#define  EL3HLT          46  /* Level 3 halted */
#define  EL3RST          47  /* Level 3 reset */
#define  ELNRNG          48  /* Link number out of range */
#define  EUNATCH         49  /* Protocol driver not attached */
#define  ENOCSI          50  /* No CSI structure available */
#define  EL2HLT          51  /* Level 2 halted */
#define  EBADE           52  /* Invalid exchange */
#define  EBADR           53  /* Invalid request descriptor */
#define  EXFULL          54  /* Exchange full */
#define  ENOANO          55  /* No anode */
#define  EBADRQC         56  /* Invalid request code */
#define  EBADSLT         57  /* Invalid slot */

#define  EDEADLOCK       EDEADLK

#define  EBFONT          59  /* Bad font file format */
#define  ENOSTR          60  /* Device not a stream */
#define  ENODATA         61  /* No data available */
#define  ETIME           62  /* Timer expired */
#define  ENOSR           63  /* Out of streams resources */
#define  ENONET          64  /* Machine is not on the network */
#define  ENOPKG          65  /* Package not installed */
#define  EREMOTE         66  /* Object is remote */
#define  ENOLINK         67  /* Link has been severed */
#define  EADV            68  /* Advertise error */
#define  ESRMNT          69  /* Srmount error */
#define  ECOMM           70  /* Communication error on send */
#define  EPROTO          71  /* Protocol error */
#define  EMULTIHOP       72  /* Multihop attempted */
#define  EDOTDOT         73  /* RFS specific error */
#define  EBADMSG         74  /* Not a data message */
#define  EOVERFLOW       75  /* Value too large for defined data type */
#define  ENOTUNIQ        76  /* Name not unique on network */
#define  EBADFD          77  /* File descriptor in bad state */
#define  EREMCHG         78  /* Remote address changed */
#define  ELIBACC         79  /* Can not access a needed shared library */
#define  ELIBBAD         80  /* Accessing a corrupted shared library */
#define  ELIBSCN         81  /* .lib section in a.out corrupted */
#define  ELIBMAX         82  /* Attempting to link in too many shared libraries */
#define  ELIBEXEC        83  /* Cannot exec a shared library directly */
#define  EILSEQ          84  /* Illegal byte sequence */
#define  ERESTART        85  /* Interrupted system call should be restarted */
#define  ESTRPIPE        86  /* Streams pipe error */
#define  EUSERS          87  /* Too many users */
#define  ENOTSOCK        88  /* Socket operation on non-socket */
#define  EDESTADDRREQ    89  /* Destination address required */
#define  EMSGSIZE        90  /* Message too long */
#define  EPROTOTYPE      91  /* Protocol wrong type for socket */
#define  ENOPROTOOPT     92  /* Protocol not available */
#define  EPROTONOSUPPORT 93  /* Protocol not supported */
#define  ESOCKTNOSUPPORT 94  /* Socket type not supported */
#define  EOPNOTSUPP      95  /* Operation not supported on transport endpoint */
#define  EPFNOSUPPORT    96  /* Protocol family not supported */
#define  EAFNOSUPPORT    97  /* Address family not supported by protocol */
#define  EADDRINUSE      98  /* Address already in use */
#define  EADDRNOTAVAIL   99  /* Cannot assign requested address */
#define  ENETDOWN       100  /* Network is down */
#define  ENETUNREACH    101  /* Network is unreachable */
#define  ENETRESET      102  /* Network dropped connection because of reset */
#define  ECONNABORTED   103  /* Software caused connection abort */
#define  ECONNRESET     104  /* Connection reset by peer */
#define  ENOBUFS        105  /* No buffer space available */
#define  EISCONN        106  /* Transport endpoint is already connected */
#define  ENOTCONN       107  /* Transport endpoint is not connected */
#define  ESHUTDOWN      108  /* Cannot send after transport endpoint shutdown */
#define  ETOOMANYREFS   109  /* Too many references: cannot splice */
#define  ETIMEDOUT      110  /* Connection timed out */
#define  ECONNREFUSED   111  /* Connection refused */
#define  EHOSTDOWN      112  /* Host is down */
#define  EHOSTUNREACH   113  /* No route to host */
#define  EALREADY       114  /* Operation already in progress */
#define  EINPROGRESS    115  /* Operation now in progress */
#define  ESTALE         116  /* Stale NFS file handle */
#define  EUCLEAN        117  /* Structure needs cleaning */
#define  ENOTNAM        118  /* Not a XENIX named type file */
#define  ENAVAIL        119  /* No XENIX semaphores available */
#define  EISNAM         120  /* Is a named type file */
#define  EREMOTEIO      121  /* Remote I/O error */
#define  EDQUOT         122  /* Quota exceeded */

#define  ENOMEDIUM      123  /* No medium found */
#define  EMEDIUMTYPE    124  /* Wrong medium type */



/*
 * Option flags per-socket. These must match the SOF_ flags in ip.h (checked in init.c)
 */
#define SO_DEBUG        0x0001 /* Unimplemented: turn on debugging info recording */
#define SO_ACCEPTCONN   0x0002 /* socket has had listen() */
#define SO_REUSEADDR    0x0004 /* Allow local address reuse */
#define SO_KEEPALIVE    0x0008 /* keep connections alive */
#define SO_DONTROUTE    0x0010 /* Unimplemented: just use interface addresses */
#define SO_BROADCAST    0x0020 /* permit to send and to receive broadcast messages (see IP_SOF_BROADCAST option) */
#define SO_USELOOPBACK  0x0040 /* Unimplemented: bypass hardware when possible */
#define SO_LINGER       0x0080 /* linger on close if data present */
#define SO_DONTLINGER   ((int)(~SO_LINGER))
#define SO_OOBINLINE    0x0100 /* Unimplemented: leave received OOB data in line */
#define SO_REUSEPORT    0x0200 /* Unimplemented: allow local address & port reuse */
#define SO_IPSEC        0x0400 /* denote ipsec using */
#define SO_FLOWCTRL     0x0800 /* denote flow control using */

#define SO_SNDBUF       0x1001  /* Unimplemented: send buffer size */
#define SO_RCVBUF       0x1002  /* receive buffer size */
#define SO_SNDLOWAT     0x1003  /* Unimplemented: send low-water mark */
#define SO_RCVLOWAT     0x1004  /* Unimplemented: receive low-water mark */
#define SO_SNDTIMEO     0x1005  /* Unimplemented: send timeout */
#define SO_RCVTIMEO     0x1006  /* receive timeout */
#define SO_ERROR        0x1007  /* get error status and clear */
#define SO_TYPE         0x1008  /* get socket type */
#define SO_CONTIMEO     0x1009  /* Unimplemented: connect timeout */
#define SO_NO_CHECK     0x100a  /* don't create UDP checksum */
#define SO_BINDTODEVICE 0x100b /* bind to device */
#define SO_BIO          0x100c  /* set socket into blocking mode */
#define SO_NBIO         0x100d  /* set socket into non-blocking mode */
#define SO_NONBLOCK     0x100e  /* set/get socket blocking mode via optval param*/

/*
 * Structure used for manipulating linger option.
 */
struct linger {
   int l_onoff;                /* option on/off */
   int l_linger;               /* linger time , second as unit*/
};

enum tcp_state {
  CLOSED      = 0,
  LISTEN      = 1,
  SYN_SENT    = 2,
  SYN_RCVD    = 3,
  ESTABLISHED = 4,
  FIN_WAIT_1  = 5,
  FIN_WAIT_2  = 6,
  CLOSE_WAIT  = 7,
  CLOSING     = 8,
  LAST_ACK    = 9,
  TIME_WAIT   = 10
};

/*
 * Structure used for get tcp_info.
 */
struct tcp_info {
    u8_t tcpi_state;      //tcp_pcb state: SYN_SENT, ESTABLISHED, etc, see tcp_state enum define in tcp.h
    u8_t tcpi_nrtx;       //tcp_pcb nrtx: number of retransmissions
    u8_t tcpi_dupacks;    //tcp_pcb dupacks: number of dupacks
    u8_t tcpi_last_timer; //tcp_pcb last_timer
};


#define  SOL_SOCKET  0xfff    /* options for socket level */

#define AF_UNSPEC       0
#define AF_INET         2
#if LWIP_IPV6
#define AF_INET6        10
#else /* LWIP_IPV6 */
#define AF_INET6        AF_UNSPEC
#endif /* LWIP_IPV6 */
#define PF_INET         AF_INET
#define PF_INET6        AF_INET6
#define PF_UNSPEC       AF_UNSPEC

#define IPPROTO_IP      0
#define IPPROTO_TCP     6
#define IPPROTO_UDP     17
#if LWIP_IPV6
#define IPPROTO_IPV6    41
#endif /* LWIP_IPV6 */
#define IPPROTO_UDPLITE 136

/* Flags we can use with send and recv. */
#define MSG_PEEK       0x01    /* Peeks at an incoming message */
#define MSG_WAITALL    0x02    /* Unimplemented: Requests that the function block until the full amount of data requested can be returned */
#define MSG_OOB        0x04    /* Unimplemented: Requests out-of-band data. The significance and semantics of out-of-band data are protocol-specific */
#define MSG_DONTWAIT   0x08    /* Nonblocking i/o for this operation only */
#define MSG_MORE       0x10    /* Sender will send more */


/*
 * Options for level IPPROTO_TCP
 */
#define TCP_NODELAY         0x01    /* don't delay send to coalesce packets */
#define TCP_KEEPALIVE       0x02    /* send KEEPALIVE probes when idle for pcb->keep_idle milliseconds */
#define TCP_KEEPIDLE        0x03    /* set pcb->keep_idle  - Same as TCP_KEEPALIVE, but use seconds for get/setsockopt */
#define TCP_KEEPINTVL       0x04    /* set pcb->keep_intvl - Use seconds for get/setsockopt */
#define TCP_KEEPCNT         0x05    /* set pcb->keep_cnt   - Use number of probes sent for get/setsockopt */
#define TCP_TIMEROUT        0x06    /* set pcb->TCP_TIMEROUT   - Use number of probes sent for get/setsockopt */
#define TCP_MSS_VALUE       0x07    /* set pcb->mss   - Use number of probes only for get/setsockopt */
#define TCP_RTO_VALUE       0x08    /* set pcb->rto   - Use number of probes only for get/setsockopt */
#define TCP_TXB_UNACK       0x09    /* set (pcb->snd_lbb - pcb->lastack) - how much byte remaind in buffer for unacked */
#define TCP_TXB_REST        0x0A    /* set (pcb->snd_buf) - how much byte remaind in tx buffer can writen by app */
#define TCP_TXB_ACKED       0x0B    /* set (pcb->acked_sum) - the number of accumulation of acked */
#define TCP_CLOSE_TIMEROUT  0x0C    /* set (pcb->close_timeout) - the timerout set of tcp close all */
#define TCP_RST_TRIGGER     0x0D    /* set reset man trigger - call tcp_reset_man_trigger() */
#define TCP_INFO            0x0E    /* get tcp_info */


#define IOCPARM_MASK    0x7fU           /* parameters must be < 128 bytes */
#define IOC_VOID        0x20000000UL    /* no parameters */
#define IOC_OUT         0x40000000UL    /* copy out parameters */
#define IOC_IN          0x80000000UL    /* copy in parameters */
#define IOC_INOUT       (IOC_IN|IOC_OUT)
                                        /* 0x20000000 distinguishes new &
                                           old ioctl's */
#define IO(x,y)        (IOC_VOID|((x)<<8)|(y))

#define IOR(x,y,t)     (IOC_OUT|(((UINT32)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))

#define IOW(x,y,t)     (IOC_IN|(((UINT32)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))

#define FIONBIO        IOW('f', 126, UINT32) /* set/clear non-blocking i/o */


#define MEMP_NUM_NETCONN 128

#define NUM_SOCKETS         MEMP_NUM_NETCONN
#define NUM_EVENTS          MEMP_NUM_NETCONN
#define LWIP_SOCKET_OFFSET  (0) /*note: must set to 0*/
#define LWIP_EVENT_OFFSET   (NUM_SOCKETS + LWIP_SOCKET_OFFSET)

/* FD_SET used for lwip_select */
#ifndef FD_SET
#undef  FD_SETSIZE
/* Make FD_SETSIZE match NUM_SOCKETS in socket.c */
#define FD_SETSIZE    MEMP_NUM_NETCONN
#define FDSETSAFESET(n, code)  do {\
    if ((((int)(n) - LWIP_SOCKET_OFFSET) < (MEMP_NUM_NETCONN * 2)) \
       && (((int)(n) - LWIP_SOCKET_OFFSET) >= 0)) {code;}\
    } while(0)
#define FDSETSAFEGET(n, code) (((((int)(n) - LWIP_SOCKET_OFFSET) < (MEMP_NUM_NETCONN * 2)) \
       && (((int)(n) - LWIP_SOCKET_OFFSET) >= 0)) ? (code) : 0)

#define FD_SET(n, p)  FDSETSAFESET(n, (p)->fd_bits[((n) - LWIP_SOCKET_OFFSET)/8] |=  (1 << (((n) - LWIP_SOCKET_OFFSET) & 7)))
#define FD_CLR(n, p)  FDSETSAFESET(n, (p)->fd_bits[((n) - LWIP_SOCKET_OFFSET)/8] &= ~(1 << (((n) - LWIP_SOCKET_OFFSET) & 7)))
#define FD_ISSET(n,p) FDSETSAFEGET(n, (p)->fd_bits[((n) - LWIP_SOCKET_OFFSET)/8] &   (1 << (((n) - LWIP_SOCKET_OFFSET) & 7)))
#define FD_ZERO(p)    memset((void*)(p),0,sizeof(*(p)))



typedef struct fd_set {
  unsigned char fd_bits [(FD_SETSIZE * 2 + 7)/8];
} fd_set;

#endif /* FD_SET */

#ifndef TIME_VAL_DEF
#define TIME_VAL_DEF
struct timeval
{
	int tv_sec;        /* seconds */
	int tv_usec;       /* and microseconds */
};
#endif /* LWIP_TIMEVAL_PRIVATE */

struct addrinfo {
    int               ai_flags;      /* Input flags. */
    int               ai_family;     /* Address family of socket. */
    int               ai_socktype;   /* Socket type. */
    int               ai_protocol;   /* Protocol of socket. */
    socklen_t         ai_addrlen;    /* Length of socket address. */
    struct sockaddr  *ai_addr;       /* Socket address of socket. */
    char             *ai_canonname;  /* Canonical name of service location. */
    struct addrinfo  *ai_next;       /* Pointer to next in list. */
};

/** A callback prototype to inform about events for a socket */
typedef void (* socket_callback)(int s, int evt, u16_t len);


#define socket(a, b, c)       sAPI_TcpipSocket(a, b, c)
#define bind(a,b,c)           sAPI_TcpipBind(a,(const SCsockAddr *)b,c)
#define listen(a,b)           sAPI_TcpipListen(a,b)

#define accept(a,b,c)         sAPI_TcpipAccept(a,(SCsockAddr *)b,c)
#define connect(a,b,c)        sAPI_TcpipConnect(a,(const SCsockAddr *)b,c)
#define send(a,b,c,d)         sAPI_TcpipSend(a,b,c,d)
#define recv(a,b,c,d)         sAPI_TcpipRecv(a,b,c,d)
#define sendto(a,b,c,d,e,f)   sAPI_TcpipSendto(a,b,c,d,(const SCsockAddr *)e,f)
#define recvfrom(a,b,c,d,e,f) sAPI_TcpipRecvfrom(a,b,c,d,(SCsockAddr *)e,f)
#define close(s)              sAPI_TcpipClose(s)
#define shutdown(a,b)         sAPI_TcpipShutdown(a,b)


#define getsockname(a,b,c)    sAPI_TcpipGetsockname(a,(SCsockAddr *)b,c)
#define getpeername(a,b,c)    sAPI_TcpipGetpeername(a,(SCsockAddr *)b,c)
#define setsockopt(a,b,c,d,e) sAPI_TcpipSetsockopt(a,b,c,d,e)
#define getsockopt(a,b,c,d,e) sAPI_TcpipGetsockopt(a,b,c,d,e)
#define ioctlsocket(a,b,c)    sAPI_TcpipIoctlsocket(a,b,c)

#define gethostbyname(name)   sAPI_TcpipGethostbyname(name)
#define select(a,b,c,d,e)     sAPI_TcpipSelect(a,(SCfdSet*)b,(SCfdSet*)c,(SCfdSet*)d,(SCtimeval*)e)

#define inet_addr(a)          sAPI_TcpipInet_addr(a)
#define htons(a)              sAPI_TcpipHtons(a)
#define ntohs(a)              sAPI_TcpipNtohs(a)
#define inet_ntoa(a)          sAPI_TcpipInet_ntoa(a)
#define inet_ntop(a,b,c,d)    sAPI_TcpipInet_ntop(a,b,c,d)
#define getsockerrno(a)      sAPI_TcpipGetSocketErrno(a)
#define htonl(a)             sAPI_TcpipHtonl(a)
#define getaddrinfo(a,b,c,d)       sAPI_TcpipGetaddrinfo(a,b,c,d)
#define getaddrinfo_with_pcid(a,b,c,d,e)             sAPI_TcpipGetaddrinfo_with_pcid(a,b,c,d,e)
#define freeaddrinfo(a)             sAPI_TcpipFreeaddrinfo(a)
#define socket_with_callback(a, b, c, d) sAPI_TcpipSocket_with_callback(a, b, c, d)

#endif


