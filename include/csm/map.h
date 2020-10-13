#ifndef CSM_INCLUDE_CSM_MAP_H_
#define CSM_INCLUDE_CSM_MAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"

typedef csm_u64 (*csm_hash)(csm_cell data);

typedef int (*csm_equals)(csm_cell one, csm_cell two);

struct csm_map_node;

/* A simple implementation of a chaining hash table. */
typedef struct csm_map {

    struct csm_map_node **table;
    csm_u64 tablesize;
    csm_u64 prime;
    csm_u64 size;
    csm_hash hasher;
    csm_equals equals;

} csm_map;

int csm_map_init(csm_map *out, csm_hash hash, csm_equals equals);

void csm_map_fini(csm_map *m);

/* Returns true and writes old value to "out" if found. */
int csm_map_get(csm_map *m, csm_cell k, csm_cell *out);

/* Returns true and writes old value to "out" if replaced. */
int csm_map_put(csm_map *m, csm_cell k, csm_cell v, csm_cell *old);

/* Returns true and writes old value to "out" if removed. */
int csm_map_remove(csm_map *m, csm_cell k, csm_cell *out);

csm_u64 csm_map_size(csm_map *m);

void csm_map_clear(csm_map *m);

extern int csm_scalar_equals(csm_cell one, csm_cell two);

extern csm_u64 csm_scalar_hash(csm_cell k);

extern csm_u64 csm_string_hash(csm_cell k);

extern int csm_string_equals(csm_cell one, csm_cell two);

extern csm_u64 csm_scalar_fasthash64(csm_cell k);

extern csm_u64 csm_scalar_fasthash32(csm_cell k);

extern csm_u64 csm_scalar_djb2(csm_cell k);

extern csm_u64 csm_scalar_fnv1a64(csm_cell k);

#ifdef __cplusplus
}
#endif
#endif
