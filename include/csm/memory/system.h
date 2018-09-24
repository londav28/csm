#ifndef CSM_MEMORY_BASE_H_
#define CSM_MEMORY_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>


void* csm_sys_malloc(size_t bytes);


void csm_sys_free(void* ptr);


#ifdef __cplusplus
}
#endif

#endif