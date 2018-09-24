#ifndef CSM_MEMSET_H_
#define CSM_MEMSET_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "csm/types.h"


void*
csm_memset(void* data, int c, size_t size);


void*
csm_memclear(void* data, size_t size);


#ifdef __cplusplus
}
#endif

#endif