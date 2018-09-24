#ifndef CSM_READPTR_H_
#define CSM_READPTR_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "csm/stream.h"


/**
 * Similar to streams, but reads and increments a pointer directly. As such,
 * there are no safety checks. Meant for use by the machine when executing
 * bytecode.
 * 
 * If you are going to use this, then make sure your buffers have already
 * accounted for endianness.
 * 
 */


void
csm_readptr_shift_rel(void** ptr, int64_t shift);


void
csm_readptr_shift_abs(void** ptr, uint64_t shift);


uint8_t
csm_readptr_u8(void** ptr);


uint16_t
csm_readptr_u16(void** ptr);


uint32_t
csm_readptr_u32(void** ptr);


uint64_t
csm_readptr_u64(void** ptr);


int8_t
csm_readptr_i8(void** ptr);


int16_t
csm_readptr_i16(void** ptr);


int32_t
csm_readptr_i32(void** ptr);


int64_t
csm_readptr_i64(void** ptr);


float
csm_readptr_f32(void** ptr);


double
csm_readptr_f64(void** ptr);


#ifdef __cplusplus
}
#endif

#endif