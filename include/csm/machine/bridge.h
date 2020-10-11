#ifndef CSM_INCLUDE_CSM_MACHINE_BRIDGE_H_
#define CSM_INCLUDE_CSM_MACHINE_BRIDGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CSM_BRIDGE_NATIVE_METHOD    0x01
#define CSM_BRIDGE_NATIVE_OBJECT    0x02
#define CSM_BRIDGE_BC_METHOD        0x03
#define CSM_BRIDGE_BC_OBJECT        0x04
#define CSM_BRIDGE_METHOD           0x05
#define CSM_BRIDGE_OBJECT           0x06

#include "csm/types.h"
#include "csm/bytecode/format.h"
#include "csm/machine/state.h"

extern const char* csm_native_symbol_prefix;

/* Absolute slowmode bridge for native/bytecode lookup. */
csm_descriptor csm_bridge_snail(int kind, csm_u32 idx, csm_thread *t);

/* The prefix for reserved names is "csm". */
int csm_symbol_has_reserved_prefix(csm_bc_string* str);

#ifdef __cplusplus
}
#endif
#endif
