#ifndef CSM_MAP_H_
#define CSM_MAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>


struct csm_map;


typedef uint64_t (*csm_hash)(void *data);


typedef int (*csm_equals)(void *one, void *two);


struct csm_map *
csm_map_create(csm_hash hash, csm_equals equals);


void
csm_map_destroy(struct csm_map *m);


void *
csm_map_get(struct csm_map *m, void *k);


void *
csm_map_put(struct csm_map *m, void *k, void *v);


void *
csm_map_remove(struct csm_map *m, void *k);


size_t
csm_map_size(struct csm_map *m);


void
csm_map_clear(struct csm_map *m);


#ifdef __cplusplus
}
#endif

#endif