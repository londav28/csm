#ifndef CSM_INCLUDE_CSM_MEMORY_GC_H_
#define CSM_INCLUDE_CSM_MEMORY_GC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"
#include "csm/machine/state.h"

struct csm_gc_config;
struct csm_gc_profile;
struct csm_gc_header;

/* Callback for collection routine. */
typedef void (*csm_gc_callback)(csm_thread *t);

/* Config GC options. */
typedef struct csm_gc_config {

    int dummy;

} csm_gc_config;

/* Detailed GC collection details. */
typedef struct csm_gc_prof {

    int dummy;

} csm_gc_prof;

typedef enum csm_gc_tag {

    CSM_GC_TAG_OBJECT_BC = 0,
    CSM_GC_TAG_OBJECT_NATIVE,
    CSM_GC_TAG_ARRAY,
    CSM_GC_TAG_UNKNOWN

} csm_gc_tag;

typedef struct csm_gc_header {
    csm_u32 tag;
    size_t bytes;
} csm_gc_header;

void csm_gc_stats(void);

void csm_gc_config_default(csm_gc_config *out);

void csm_gc_setup(csm_gc_config *conf);

void csm_gc_cleanup(void);

void csm_gc_profile(csm_gc_prof *out);

void *csm_gc_alloc(csm_u8 tag, csm_u64 bytes, csm_thread *t);

void csm_gc_minimize(void);

void csm_gc_collect(void);

void csm_gc_pin_on(void *object);

void csm_gc_pin_off(void *object);

void csm_gc_enable(void);

void csm_gc_disable(void);

csm_u64 csm_gc_heap_size(void);

csm_u64 csm_gc_arena_size(csm_thread *t);

#ifdef __cplusplus
}
#endif
#endif

