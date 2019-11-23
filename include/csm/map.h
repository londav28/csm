#ifndef CSM_INCLUDE_CSM_MAP_H_
#define CSM_INCLUDE_CSM_MAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"

struct csm_map_node_;

typedef struct {

    struct csm_map_node_ **table;
    size_t tablesize;
    csm_f64 lf;
    size_t size;
    csm_hash hash;
    csm_equals equals;

} csm_map;

typedef csm_u64 (*csm_hash)(void *data);

typedef int (*csm_equals)(void *one, void *two);

csm_map *csm_map_create(csm_hash hash, csm_equals equals);

int csm_map_init(csm_hash hash, csm_equals equals);

void csm_map_destroy(csm_map *m);

void *csm_map_get(csm_map *m, void *k);

void *csm_map_put(csm_map *m, void *k, void *v);

void *csm_map_remove(csm_map *m, void *k);

size_t csm_map_size(csm_map *m);

void csm_map_clear(csm_map *m);

#ifdef __cplusplus
}
#endif
#endif
