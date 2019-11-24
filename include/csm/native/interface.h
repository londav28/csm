#ifndef CSM_INCLUDE_CSM_MACHINE_RESOLUTION_H_
#define CSM_INCLUDE_CSM_MACHINE_RESOLUTION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"
#include "csm/bytecode/format.h"
#include "csm/machine/state.h"

extern const char* csm_native_symbol_prefix;

/* Load all native module definitions. */
void csm_native_modules_setup(void);

/* Unload all native module definitions. */
void csm_native_modules_teardown(void);

/* The prefix for native names is "csm". */
int csm_method_has_reserved_prefix(csm_bc_string* str);

/* Absolute slowmode bridge for native/bytecode methods. */
csm_descriptor csm_method_bridge_snail(csm_u32 idx, csm_thread *t);

/* Absolute slowmode bridge for native/bytecode objects. */
csm_descriptor csm_object_bridge_snail(csm_u32 idx, csm_thread *t);

#ifdef __cplusplus
}
#endif
#endif
