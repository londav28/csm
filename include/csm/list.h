#ifndef CSM_LIST_H_
#define CSM_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>


struct csm_list;


struct csm_list *
csm_list_create(void);


void
csm_list_destroy(struct csm_list *list);





#ifdef __cplusplus
}
#endif

#endif