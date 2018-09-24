#ifndef CSM_BYTECODE_POSTINIT_H_
#define CSM_BYTECODE_POSTINIT_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


struct csm_bc_method;
struct csm_bc_object;
struct csm_bc_tlabel;


void
csm_bc_method_post_flags(struct csm_bc_method* m);


uint32_t
csm_bc_method_post_paramc(struct csm_bc_method* m);


struct csm_bc_tlabel*
csm_bc_method_post_params(struct csm_bc_method* m);


uint32_t
csm_bc_method_post_insc(struct csm_bc_method* m);


uint32_t
csm_bc_object_post_fieldc(struct csm_bc_object* o);


struct cm_bc_tlabel*
csm_bc_object_post_fields(struct csm_bc_object* o);


#ifdef __cplusplus
}
#endif

#endif