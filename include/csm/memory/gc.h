#ifndef CSM_MEMORY_GC_H_
#define CSM_MEMORY_GC_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "csm/types.h"


struct csm_thread;
struct csm_descriptor;
struct csm_gc_config;
struct csm_gc_profile;
struct csm_gc_info;


/* Callback for collection routine. */
typedef void (*csm_gc_callback)(struct csm_thread *t);


/* Config GC options. */
struct csm_gc_config {
    int dummy;
};


/* Detailed GC collection details. */
struct csm_gc_profile {
    int dummy;
};


enum csm_gc_tag {

    CSM_GC_TAG_OBJECT_BC = 0,
    CSM_GC_TAG_OBJECT_NATIVE,
    CSM_GC_TAG_METHOD_BC,
    CSM_GC_TAG_METHOD_NATIVE,
    CSM_GC_TAG_ARRAY,
    CSM_GC_TAG_UNKNOWN

};


struct csm_gc_header {

    uint32_t* string;
    uint16_t tag;
    size_t bytes;

};

    
/**___________________________________________________________________________
 * MAIN INTERFRACE
 */


/* Used for debugging ONLY! */
void
csm_gc_stats(void);


void
csm_gc_config_default(struct csm_gc_config* out);


void
csm_gc_setup(struct csm_gc_config* conf);


void
csm_gc_cleanup(void);


void
csm_gc_profile(struct csm_gc_profile* out);


/* If thread is NULL, the allocation is unowned and cannot be collected! */
void *
csm_gc_alloc(struct csm_gc_header hdr, struct csm_thread* t);


void
csm_gc_minimize(void);


/* TODO: Fine-tuned control over allocations. */
void
csm_gc_collect(void);


void
csm_gc_pin_on(void* object);


void
csm_gc_pin_off(void* object);


void
csm_gc_enable(void);


void
csm_gc_disable(void);


uint64_t
csm_gc_heap_size(void);



#ifdef __cplusplus
}
#endif

#endif
