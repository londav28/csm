#include "csm/map.h"
#include "csm/memory/malloc.h"
#include "csm/types.h"
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
    csm_i64 n = 0;

    assert(after >= 0);

    if (after < 3) { return 3; }
    n = after;

    if ((n % 2) == 0) { n++; }

    while (CSM_TRUE) {
        csm_i64 i = 0;
        int is_prime = CSM_TRUE;

        for (i = 3; i < (n/2); i++) {
            if ((n % i) == 0) {
                is_prime = CSM_FALSE;
                break;
            }
        }

        if (is_prime && n > after) { return n; }

        n += 2;
    }

    return 0;
}

int csm_map_init(csm_map* out, csm_hash hash, csm_equals equals)
{
    csm_i64 a = next_prime_(1);

    (void) a;
    (void) out;
    (void) hash;
    (void) equals;

    goto _unwind_0;

_unwind_0:
    return -1;
}

void csm_map_fini(csm_map *m)
{
    (void) m;
    return;
}

void *csm_map_get(csm_map *m, void *k)
{
    (void) m;
    (void) k;
    return NULL;
}

void *csm_map_put(csm_map *m, void *k, void *v)
{
    (void) m;
    (void) k;
    (void) v;
    return NULL;
}

void *csm_map_remove(csm_map *m, void *k)
{
    (void) m;
    (void) k;
    return NULL;
}

size_t csm_map_size(csm_map *m)
{
    (void) m;
    return 0;
}

void csm_map_clear(csm_map *m)
{
    (void) m;
    return;
}

