/* ${RENESAS_LICENSE_CLAUSE_PLACEHOLDER} */

#ifndef _RWNX_LE_UTIL_H
#define _RWNX_LE_UTIL_H

#include "rwnx_types.h"

static inline uint16_t swap16(uint16_t x)
{
    return (uint16_t)((x << 8) | (x >> 8));
}

static inline uint32_t swap32(uint32_t x)
{
    return ((x << 24) & 0xFF000000) |
           ((x << 8)  & 0x00FF0000) |
           ((x >> 8)  & 0x0000FF00) |
           ((x >> 24) & 0x000000FF);
}

#define cpu_to_le64(x) ((uint64_t)(x))
#define le64_to_cpu(x) ((uint64_t)(x))
#define cpu_to_le32(x) ((uint32_t)(x))
#define le32_to_cpu(x) ((uint32_t)(x))
#define cpu_to_le16(x) ((uint16_t)(x))
#define le16_to_cpu(x) ((uint16_t)(x))

#define cpu_to_be16(x) swap16((uint16_t)(x))
#define be16_to_cpu(x) swap16((uint16_t)(x))
#define cpu_to_be32(x) swap32((uint32_t)(x))
#define be32_to_cpu(x) swap32((uint32_t)(x))

#endif /* _RWNX_LE_UTIL_H */
