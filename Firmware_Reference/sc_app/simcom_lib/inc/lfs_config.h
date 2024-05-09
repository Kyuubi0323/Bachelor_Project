#ifndef _LFS_CONFIG_H_
#define _LFS_CONFIG_H_

// System includes
#include <string.h>
//#include "UART.h"
//#include "utils.h"
#include "lfs_trace.h"

// Macros, may be replaced by system specific wrappers. Arguments to these
// macros must not have side-effects as the macros can be removed for a smaller
// code footprint

// Logging functions
#ifndef LFS_NO_DEBUG
#define LFS_DEBUG(fmt, ...) \
    CPUartLogPrintf("lfs debug:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define LFS_DEBUG(fmt, ...)
#endif

#ifndef LFS_NO_WARN
#define LFS_WARN(fmt, ...) \
    CPUartLogPrintf("lfs warn:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define LFS_WARN(fmt, ...)
#endif

#ifndef LFS_NO_ERROR
#define LFS_ERROR(fmt, ...) \
    CPUartLogPrintf("lfs error:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define LFS_ERROR(fmt, ...)
#endif

// Runtime assertions
#ifndef LFS_NO_ASSERT
#define LFS_ASSERT(test) ASSERT(test)
#else
#define LFS_ASSERT(test)
#endif

// Builtin functions, these may be replaced by more efficient
// toolchain-specific implementations. LFS_NO_INTRINSICS falls back to a more
// expensive basic C implementation for debugging purposes

// Min/max functions for unsigned 32-bit numbers
static inline UINT32 lfs_max(UINT32 a, UINT32 b) {
    return (a > b) ? a : b;
}

static inline UINT32 lfs_min(UINT32 a, UINT32 b) {
    return (a < b) ? a : b;
}

// Find the next smallest power of 2 less than or equal to a
static inline UINT32 lfs_npw2(UINT32 a) {
#if 0
    return 32 - __builtin_clz(a-1);
#else
    UINT32 r = 0;
    UINT32 s;
    a -= 1;
    s = (a > 0xffff) << 4; a >>= s; r |= s;
    s = (a > 0xff  ) << 3; a >>= s; r |= s;
    s = (a > 0xf   ) << 2; a >>= s; r |= s;
    s = (a > 0x3   ) << 1; a >>= s; r |= s;
    return (r | (a >> 1)) + 1;
#endif
}

// Count the number of trailing binary zeros in a
// lfs_ctz(0) may be undefined
static inline UINT32 lfs_ctz(UINT32 a) {
#if 0
    return __builtin_ctz(a);
#else
    return lfs_npw2((a & -a) + 1) - 1;
#endif
}

// Count the number of binary ones in a
static inline UINT32 lfs_popc(UINT32 a) {
#if 0
    return __builtin_popcount(a);
#else
    a = a - ((a >> 1) & 0x55555555);
    a = (a & 0x33333333) + ((a >> 2) & 0x33333333);
    return (((a + (a >> 4)) & 0xf0f0f0f) * 0x1010101) >> 24;
#endif
}

// Find the sequence comparison of a and b, this is the distance
// between a and b ignoring overflow
static inline int lfs_scmp(UINT32 a, UINT32 b) {
    return (int)(unsigned)(a - b);
}

// Convert from 32-bit little-endian to native order
#if 0
static inline UINT32 lfs_fromle32(UINT32 a) {
#if !defined(LFS_NO_INTRINSICS) && ( \
    (defined(  BYTE_ORDER  ) &&   BYTE_ORDER   ==   ORDER_LITTLE_ENDIAN  ) || \
    (defined(__BYTE_ORDER  ) && __BYTE_ORDER   == __ORDER_LITTLE_ENDIAN  ) || \
    (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__))
    return a;
#elif !defined(LFS_NO_INTRINSICS) && ( \
    (defined(  BYTE_ORDER  ) &&   BYTE_ORDER   ==   ORDER_BIG_ENDIAN  ) || \
    (defined(__BYTE_ORDER  ) && __BYTE_ORDER   == __ORDER_BIG_ENDIAN  ) || \
    (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__))
    return __builtin_bswap32(a);
#else
    return (((UINT8*)&a)[0] <<  0) |
           (((UINT8*)&a)[1] <<  8) |
           (((UINT8*)&a)[2] << 16) |
           (((UINT8*)&a)[3] << 24);
#endif
}

// Convert to 32-bit little-endian from native order
static inline UINT32 lfs_tole32(UINT32 a) {
    return lfs_fromle32(a);
}
#else
#define lfs_fromle32(a) a
#define lfs_tole32(a) a
#endif


// Calculate CRC-32 with polynomial = 0x04c11db7
void lfs_crc(UINT32 *crc, const void *buffer, size_t size);

// Allocate memory, only used if buffers are not provided to littlefs
#if 0
static inline void *lfs_malloc(size_t size) {
#ifndef LFS_NO_MALLOC
    return rt_malloc(size);
#else
    return NULL;
#endif
}

// Deallocate memory, only used if buffers are not provided to littlefs
static inline void lfs_free(void *p) {
#ifndef LFS_NO_MALLOC
    rt_free(p);
#endif
}
#else
#define lfs_malloc malloc
#define lfs_free free

extern void *malloc( size_t Size );
extern void free( void *p );
#endif
#endif
