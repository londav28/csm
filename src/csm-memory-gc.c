#include "csm/memory/gc.h"
#include "csm/memory/system.h"
#include "csm/machine/state.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/* May add more status flags later. */
#define CSM_GC_STATUS_MARKED    0b0000000000000001
#define CSM_GC_STATUS_PINNED    0b0000000000000010
#define CSM_GC_STATUS_STRING    0b0000000000000100
#define CSM_GC_STATUS_NINDEX    0b0000000000001000

#define CSM_ALIGN_PTR(amount, by) (amount + (amount % by))
#define CSM_ALIGN_PTR_8(amount) CSM_ALIGN_PTR(amount, 8)
#define CSM_ALIGN_PTR_16(amount) CSM_ALIGN_PTR(amount, 16)
#define CSM_ALIGH_PTR_32(amount) CSM_ALIGN_PTR(amount, 32)

typedef struct csm_traced_header {

    csm_gc_header hdr;
    /* TODO: Embed the mark into the last bit of the link pointer. */
    csm_u8 mark;
    csm_traced_header *next;
    csm_u64 size;

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
        printf("> %p : %lu\n", (void*) traced, traced->size);
        traced = traced->next;
    }

    return;
}

void csm_gc_config_default(csm_gc_config *out)
{
    (void) out;
    return;
}

void csm_gc_profile(csm_gc_profile *out)
{
    (void) out;
    return;
}

void *csm_gc_alloc(csm_gc_header hdr, csm_thread *t)
{
    size_t realsize = 0;
    csm_traced_header *header = NULL;
    void *result = NULL;

    /* TODO: Thread is used when we add callbacks! */
    (void) t;

    /* TODO: Catch overflow errors here? */
    realsize = CSM_ALIGN_PTR_8(hdr.bytes + sizeof(*header));

    /* TODO: Replace malloc with more primitive allocator later. */
    header = csm_sys_malloc(realsize);
    if (header == NULL) { return NULL; }

    /* Increment traced allocation stats. */
    traced_count += 1;
    traced_bytes += realsize;

    /* Populate the intrusive list entry. */
    header->hdr = hdr;
    header->mark = 0;
    header->next = traced_start;
    header->size = realsize;

    /* Update intrusive list head. */
    traced_start = header;

    result = (header + 1);

    return result;
}

