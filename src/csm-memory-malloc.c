#include "csm/memory/malloc.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


// TODO: Remove if we no longer need debug dump?
#include <stdio.h>


#define CSM_ALIGN(amount, by) (amount + (amount % by))
#define CSM_ALIGN_8(amount) CSM_ALIGN(amount, 8)
#define CSM_ALIGN_16(amount) CSM_ALIGN(amount, 16)


// TODO: Expose this later?
struct csm_untracked_header {

    struct csm_untracked_header* next;
    struct csm_untracked_header* prev;
    uint64_t size;

};


// Intrusive list of untracked allocations.
static struct csm_untracked_header* untracked_start;
uint64_t untracked_count;
uint64_t untracked_bytes;



void
csm_malloc_stats(void)
{
    struct csm_untracked_header* untracked;

    // Non portable format specifier usage here.
    printf(
                "-- %lu untracked allocations (%lu bytes)\n",
                untracked_count,
                untracked_bytes
    );

    untracked = untracked_start;

    while (untracked && untracked_count > 0) {
        printf("> %p : %lu\n", (void*) untracked, untracked->size);
        untracked = untracked->next;
    }
}


// TODO: This and "csm_gc_alloc" share similar code, any way to abstract?
void* csm_malloc(size_t bytes)
{
    size_t realsize;
    struct csm_untracked_header* header;
    void* result;

    // TODO: Catch overflow errors here?
    realsize = CSM_ALIGN_8(bytes + sizeof(*header));

    // TODO: Replace malloc with our own internal implementation.
    header = malloc(realsize);
    if (header == NULL) { return NULL; }

    // Increment untracked allocation stats.
    untracked_count += 1;
    untracked_bytes += realsize;

    // For untracked allocations we make use of double links.
    header->next = untracked_start;
    header->prev = NULL;
    header->size = realsize;

    // Update intrusive list head.
    if (untracked_start) { untracked_start->prev = header; }
    untracked_start = header;

    result = (header + 1);

    return result;
}


void csm_free(void* object)
{
    struct csm_untracked_header* header;

    header = ((struct csm_untracked_header*) object) - 1;

    // Adjust allocation statis.
    untracked_count -= 1;
    untracked_bytes -= header->size;

    // Remove header from list by relinking adjacent nodes.
    if (header->prev) { header->prev->next = header->next; }
    if (header->next) { header->next->prev = header->prev; }

    // TODO: Replace free with our own internal implementation.
    free(header);

    return;
}