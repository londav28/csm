#ifndef CSM_INCLUDE_CSM_MACHINE_START_H_
#define CSM_INCLUDE_CSM_MACHINE_START_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/bytecode/format.h"
#include "csm/machine/state.h"

int csm_start_basic(csm_machine *m, csm_bc_module *mod);

#ifdef __cplusplus
}
#endif
#endif