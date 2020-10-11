#ifndef CSM_INCLUDE_CSM_NATIVE_SETUP_H_
#define CSM_INCLUDE_CSM_NATIVE_SETUP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"
#include "csm/bytecode/format.h"
#include "csm/machine/state.h"

/* Load all native module definitions in the "csm" namespace. */
void csm_native_modules_setup(void);

/* Unload all native module definitions in the "csm" namespace. */
void csm_native_modules_teardown(void);

#ifdef __cplusplus
}
#endif
#endif
