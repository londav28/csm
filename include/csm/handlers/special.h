#ifndef CSM_INCLUDE_CSM_HANDLERS_SPECIAL_H_
#define CSM_INCLUDE_CSM_HANDLERS_SPECIAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"
#include "csm/machine/state.h"

/* Terminate the current thread witn an unexpected cause. */
csm_unpacked_op csm_special_terminate_unexpected(csm_thread* t);

/* Expects bad array size on stack. */
csm_unpacked_op csm_special_bad_array_size(csm_thread* t);

/* Expects bad array index on stack. */
csm_unpacked_op csm_special_bad_array_load(csm_thread* t);

/* Expects bad array index on stack. */
csm_unpacked_op csm_special_bad_array_store(csm_thread* t);

/* Expects (push dividend, push divisor) on stack. */
csm_unpacked_op csm_special_divide_by_zero(csm_thread* t);

/* Expects string constant index on stack. */
csm_unpacked_op csm_special_no_method(csm_thread* t);

/* Expects string constant index on stack. */
csm_unpacked_op csm_special_no_object(csm_thread* t);

/* Expects string constant index on stack. */
csm_unpacked_op csm_special_callstack_overflow(csm_thread* t);

/* Expects allocation size on stack. */
csm_unpacked_op csm_special_memory_exhaustion(csm_thread* t);

/* Marks the end of normal execution. */
csm_unpacked_op csm_special_eox(csm_thread* t);

#ifdef __cplusplus
}
#endif
#endif