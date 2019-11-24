#ifndef CSM_INCLUDE_CSM_TYPES_H_
#define CSM_INCLUDE_CSM_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#define CSM_FALSE   0x00
#define CSM_TRUE    0x01

/* Base types, for now assumes existence of <stdint.h>! */
typedef uint8_t     csm_u8;
typedef uint16_t    csm_u16;
typedef uint32_t    csm_u32;
typedef uint64_t    csm_u64;
typedef int8_t      csm_i8;
typedef int16_t     csm_i16;
typedef int32_t     csm_i32;
typedef int64_t     csm_i64;
typedef float       csm_f32;
typedef double      csm_f64;

#ifdef __cplusplus
}
#endif
#endif
