#ifndef CSM_INCLUDE_CSM_MEMORY_BASE_H_
#define CSM_INCLUDE_CSM_MEMORY_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"

void *csm_sys_malloc(size_t bytes);

void csm_sys_free(void* ptr);

#ifdef __cplusplus
}
#endif
#endif
