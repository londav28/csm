#include "csm/map.h"
#include "csm/memory/malloc.h"
#include <assert.h>

/* Just randomly picked 61 as my starting prime. */
#define CSM_MAP_START_SIZE      61

struct csm_map_node_ {

    struct csm_map_node_    *next;
    void                    *data;

};

/* It's ok if this isn't that performant. Only called on table resize. */
static csm_i64 next_prime_(csm_i64 after)
{
    int64_t n;

    assert(after >= 0);
    if (after < 3) { return 3; }
    n = after;

    if ((n % 2) == 0) { n++; }

    while (TRUE) {
        int64_t i;
        int is_prime = TRUE;

        for (i = 3; i < (n/2); i++) {
            if ((n % i) == 0) {
                is_prime = FALSE;
                break;
            }
        }

        if (is_prime && n > after) { return n; }

        n += 2;
    }

    return 0;
}


struct csm_map *
csm_map_create(csm_hash hash, csm_equals equals)
{
    struct csm_map* result;

    result = csm_malloc(sizeof(*result));
    if (result == NULL) {
        goto _unwind_0;
    }

    (void) hash;
    (void) equals;

    int64_t a = next_prime_(1);
    (void) a;



_unwind_0:
    return NULL;
}


void
csm_map_destroy(struct csm_map *m)
{
    (void) m;
    return;
}


void *
csm_map_get(struct csm_map *m, void *k)
{
    (void) m;
    (void) k;
    return NULL;
}


void *
csm_map_put(struct csm_map *m, void *k, void *v)
{
    (void) m;
    (void) k;
    (void) v;
    return NULL;
}


void *
csm_map_remove(struct csm_map *m, void *k)
{
    (void) m;
    (void) k;
    return NULL;
}


size_t
csm_map_size(struct csm_map *m)
{
    (void) m;
    return 0;
}


void
csm_map_clear(struct csm_map *m)
{
    (void) m;
    return;
}
