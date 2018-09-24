#ifndef CSM_HANDLERS_SPECIAL_H_
#define CSM_HANDLERS_SPECIAL_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "csm/types.h"
#include "csm/machine/state.h"


/* Terminate the current thread witn an unexpected cause. */
struct csm_unpacked_op
csm_special_terminate_unexpected(struct csm_thread* t);

/* Expects bad array size on stack. */
struct csm_unpacked_op
csm_special_bad_array_size(struct csm_thread* t);

/* Expects bad array index on stack. */
struct csm_unpacked_op
csm_special_bad_array_load(struct csm_thread* t);

/* Expects bad array index on stack. */
struct csm_unpacked_op
csm_special_bad_array_store(struct csm_thread* t);

/* Expects (push dividend, push divisor) on stack. */
struct csm_unpacked_op
csm_special_divide_by_zero(struct csm_thread* t);

/* Expects string constant index on stack. */
struct csm_unpacked_op
csm_special_no_method(struct csm_thread* t);

/* Expects string constant index on stack. */
struct csm_unpacked_op
csm_special_no_object(struct csm_thread* t);

/* Expects string constant index on stack. */
struct csm_unpacked_op
csm_special_callstack_overflow(struct csm_thread* t);

/* Expects allocation size on stack. */
struct csm_unpacked_op
csm_special_memory_exhaustion(struct csm_thread* t);

/* Marks the end of normal execution. */
struct csm_unpacked_op
csm_special_eox(struct csm_thread* t);


#ifdef __cplusplus
}
#endif

#endif