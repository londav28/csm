#include "csm/types.h"
#include "csm/map.h"
#include "csm/memory/malloc.h"
#include <assert.h>

const csm_u64 hi = 500000;

int main() {
    csm_map map;
    csm_map *m = &map;
    csm_u64 i = 0;

    csm_map_init(m, csm_scalar_fnv1a64, csm_scalar_equals);

    for (i = 0; i < hi; i++) {
        csm_cell k;
        csm_cell v;

        k.as.i64 = i;
        v.as.i64 = i * 2;

        if (csm_map_put(m, k, v, NULL)) {
            assert("Key replaced?" == NULL);
        }
    }

    assert(csm_map_size(m) == hi);

    for (i = 0; i < hi; i++) {
        csm_cell k;
        csm_cell v;

        k.as.i64 = i;

        if (csm_map_remove(m, k, &v)) {
            assert(v.as.u64 == (i * 2));
        } else {
            assert("Removal failure!" == 0);
        }
    }

    assert(csm_map_size(m) == 0);

    for (i = 0; i < hi; i++) {
        csm_cell k;
        csm_cell v;

        k.as.i64 = i;
        v.as.i64 = i * 2;

        if (csm_map_put(m, k, v, NULL)) {
            assert("Key replaced?" == NULL);
        }
    }

    assert(csm_map_size(m) == hi);

    for (i = 0; i < hi; i++) {
        csm_cell k;
        csm_cell v;
        csm_cell a;

        k.as.i64 = i;
        v.as.i64 = i * 4;

        if (csm_map_put(m, k, v, &a)) {
            assert(a.as.u64 == (i * 2));
        } else {
            assert("Key not replaced?" == NULL);
        }
    }

    assert(csm_map_size(m) == hi);

    for (i = 0; i < hi; i++) {
        csm_cell k;
        csm_cell a;

        k.as.i64 = i;

        if (!csm_map_get(m, k, &a)) {
            assert("Lookup failure!" == NULL);
        } else {
            assert(a.as.u64 == (i * 4));
        }
    }

    assert(csm_map_size(m) == hi);

    csm_map_clear(m);

    assert(csm_map_size(m) == 0);

    for (i = 0; i < hi; i++) {
        csm_cell k;
        csm_cell v;
        csm_cell a;

        k.as.i64 = i;
        v.as.i64 = i * 2;

        if (csm_map_put(m, k, v, NULL)) {
            assert("Key replaced?" == NULL);
        }

        assert(csm_map_get(m, k, &a));
        assert(a.as.i64 == v.as.i64);
    }

    csm_map_fini(m);

    csm_malloc_stats();

    return 0;
}

