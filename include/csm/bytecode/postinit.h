#ifndef CSM_INCLUDE_CSM_BYTECODE_POSTINIT_H_
#define CSM_INCLUDE_CSM_BYTECODE_POSTINIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"
#include "csm/bytecode/format.h"

int csm_bc_module_postinit(csm_bc_module* m);

int csm_bc_method_postinit(csm_bc_method* m);

int csm_bc_object_postinit(csm_bc_object* o);

#ifdef __cplusplus
}
#endif
#endif
