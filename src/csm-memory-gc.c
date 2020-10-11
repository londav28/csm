#include "csm/memory/gc.h"
#include "csm/memory/system.h"
#include "csm/machine/state.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define CSM_ALIGN_PTR(amount__, by__) ((amount__) + ((amount__) % by__))
#define CSM_ALIGN_PTR_8(amount__) CSM_ALIGN_PTR((amount__), 8)
#define CSM_ALIGN_PTR_16(amount__) CSM_ALIGN_PTR((amount__), 16)
#define CSM_ALIGH_PTR_32(amount__) CSM_ALIGN_PTR((amount__), 32)

#define CSM_GC_HDR_TAG_MASK (((csm_u64) 0xFF) << 56)
#define CSM_GC_MAX_ALLOC_SIZE (~((csm_u64) 0) ^ CSM_GC_HDR_TAG_MASK)
#define CSM_GC_HDR_GET_TAG(w__) (csm_u8 (w__ & CSM_GC_HDR_TAG_MASK))
#define CSM_GC_HDR_GET_SIZE(w__) (((csm_u64) w__) & ~CSM_GC_HDR_TAG_MASK)

#define CSM_GC_HDR_SET_TAG(wp__, t__)                                       \
    do {                                                                    \
        csm_u64 clear_tag_bits = (*wp__ & ~CSM_GC_HDR_TAG_MASK);            \
        csm_u64 shifted_tag = (((csm_u64) t__) << 56);                      \
        *wp__ = clear_tag_bits | shifted_tag;                               \
    } while (0)

#define CSM_GC_HDR_SET_SIZE(wp__, s__)                                      \
    do {                                                                    \
        csm_u64 clear_size_bits = (*wp__  & CSM_GC_HDR_TAG_MASK);           \
        csm_u64 shifted_size = (s__ & ~CSM_GC_HDR_TAG_MASK);                \
        *wp__ = clear_size_bits | shifted_size;                             \
    } while (0)
        
typedef struct csm_traced_header {

    /* TODO: Embed the mark bit into the last bit of the link pointer. */
    struct csm_traced_header *next;
    csm_u64 tagsize;

} csm_traced_header;

/* Intrusive list of traced allocations. */
static csm_traced_header *traced_start = NULL;
csm_u64 traced_count = 0;
csm_u64 traced_bytes = 0;

void csm_gc_stats(void)
{
    csm_traced_header *traced;

    /* TODO: Non portable format specifier usage here. */
    printf(
                "-- %lu traced allocations (%lu bytes)\n",
                traced_count,
                traced_bytes
    );

    traced = traced_start;

    while (traced && traced_count > 0) {
        csm_u64 size = CSM_GC_HDR_GET_SIZE(traced->tagsize);
        printf("> %p : %lu\n", (void*) traced, size);
        traced = traced->next;
    }

    return;
}

void csm_gc_config_default(csm_gc_config *out)
{
    (void) out;
    return;
}

void csm_gc_profile(csm_gc_prof *out)
{
    (void) out;
    return;
}

void *csm_gc_alloc(csm_u8 tag, csm_u64 bytes, csm_thread *t)
{
    /* TODO: Verify that tag is valid. */
    csm_traced_header *header = NULL;
    void *result = NULL;
    size_t realbytes = 0;

    /* TODO: Thread is used when we add callbacks! */
    (void) t;

    /* TODO: Catch overflow errors here? */
    realbytes = CSM_ALIGN_PTR_8(bytes + sizeof(*header));

    assert(realbytes < CSM_GC_MAX_ALLOC_SIZE);

    /* TODO: Replace malloc with more primitive allocator later. */
    header = csm_sys_malloc(realbytes);
    if (header == NULL) {
        return NULL;
    }

    /* Increment traced allocation stats. */
    traced_count += 1;
    traced_bytes += realbytes;

    /* Populate the intrusive list entry. */
    header->next = traced_start;

    /* Set the tag and size words. */
    CSM_GC_HDR_SET_SIZE(&header->tagsize, realbytes);
    CSM_GC_HDR_SET_TAG(&header->tagsize, tag);

    /* Update intrusive list head. */
    traced_start = header;

    /* Shift to get payload. */
    result = (header + 1);

    return result;
}

