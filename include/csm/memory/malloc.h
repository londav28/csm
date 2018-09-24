#ifndef CSM_MEMORY_UNTRACKED_H_
#define CSM_MEMORY_UNTRACKED_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>


void*
csm_malloc(size_t bytes);


void
csm_free(void *object);


void
csm_malloc_stats(void);


#ifdef __cplusplus
}
#endif

#endif