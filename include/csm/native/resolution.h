#ifndef CSM_MACHINE_RESOLUTION_H_
#define CSM_MACHINE_RESOLUTION_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "csm/types.h"
#include "csm/machine/state.h"


extern const char* csm_native_symbol_prefix;
struct csm_bc_string;


int
csm_method_has_reserved_prefix(struct csm_bc_string* str);


/* Absolute slowmode bridge for native/bytecode methods. */
struct csm_descriptor
csm_method_bridge_snail(uint32_t idx, struct csm_thread *t);


/* Absolute slowmode bridge for native/bytecode objects. */
struct csm_descriptor
csm_object_bridge_snail(uint32_t idx, struct csm_thread *t);


#ifdef __cplusplus
}
#endif

#endif