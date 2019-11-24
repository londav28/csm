#ifndef CSM_INCLUDE_CSM_READPTR_H_
#define CSM_INCLUDE_CSM_READPTR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"
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

void csm_readptr_shift_rel(void** ptr, csm_i64 shift);

void csm_readptr_shift_abs(void** ptr, csm_u64 shift);

csm_u8 csm_readptr_u8(void** ptr);

csm_u16 csm_readptr_u16(void** ptr);

csm_u32 csm_readptr_u32(void** ptr);

csm_u64 csm_readptr_u64(void** ptr);

csm_i8 csm_readptr_i8(void** ptr);

csm_i16 csm_readptr_i16(void** ptr);

csm_i32 csm_readptr_i32(void** ptr);

csm_i64 csm_readptr_i64(void** ptr);

csm_f32 csm_readptr_f32(void** ptr);

csm_f64 csm_readptr_f64(void** ptr);

#ifdef __cplusplus
}
#endif
#endif
