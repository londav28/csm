#ifndef CSM_NATIVE_MODULE_GC_H_
#define CSM_NATIVE_MODULE_GC_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "csm/types.h"
#include "csm/machine/state.h"


/* Search tables. */
extern struct csm_native_method* csm_native_mem_methods[];
extern struct csm_native_object* csm_native_mem_objects[];


/* Call before execution. */
void csm_native_mem_setup(void);
void csm_native_mem_teardown(void);


/**___________________________________________________________________________
 * METHODS
 */


/* Attempt to minimize heap overhead as much as possible. */
void
csm_native_mem_gc_minimize(struct csm_thread* t);


/* Perform a full collection, reclaiming all unused memory. */
void
csm_native_mem_gc_collect(struct csm_thread* t);


/* Pin an object to the heap, preventing the GC from collecting it. */
void
csm_native_mem_gc_pin_on(struct csm_thread* t);


/* Release a pinned object, allowing the GC to collect it again. */
void
csm_native_mem_gc_pin_off(struct csm_thread* t);


/* The GC will only collect if memory exhaustion occurs. */
void
csm_native_mem_gc_disable(struct csm_thread* t);


/* If disabled, the GC may now collect at will again. */
void
csm_native_mem_gc_enable(struct csm_thread* t);


/* Display GC memory allocation stats. */
void
csm_native_mem_gc_stats(struct csm_thread* t);


#ifdef __cplusplus
}
#endif

#endif