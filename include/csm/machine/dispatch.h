#ifndef CSM_INCLUDE_CSM_MACHINE_DISPATCH_H_
#define CSM_INCLUDE_CSM_MACHINE_DISPATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/bytecode/format.h"
#include "csm/machine/state.h"

int csm_dispatch_basic(csm_thread *t, csm_bc_method *m);

#ifdef __cplusplus
}
#endif
#endif