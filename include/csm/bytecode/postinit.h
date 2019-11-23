#ifndef CSM_INCLUDE_CSM_BYTECODE_POSTINIT_H_
#define CSM_INCLUDE_CSM_BYTECODE_POSTINIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"
#include "csm/bytecode/format.h"

void csm_bc_method_post_flags(csm_bc_method* m);

csm_u32 csm_bc_method_post_paramc(csm_bc_method* m);

csm_bc_tlabel *csm_bc_method_post_params(csm_bc_method* m);

csm_u32 csm_bc_method_post_insc(csm_bc_method* m);

uint32_t csm_bc_object_post_fieldc(csm_bc_object* o);

csm_bc_tlabel *csm_bc_object_post_fields(csm_bc_object* o);

#ifdef __cplusplus
}
#endif
#endif