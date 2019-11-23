#ifndef CSM_INCLUDE_CSM_MACHINE_INIT_H_
#define CSM_INCLUDE_CSM_MACHINE_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"
#include "csm/machine/state.h"

typedef struct csm_init_profile {

    csm_u32 datastack_limit;
    csm_u32 callstack_limit;
    csm_u8 gc_latency_level;

} csm_init_profile;

int csm_thread_init_basic(
        csm_thread *t,
        csm_machine *m,
        csm_u32 data_stack_size,
        csm_u32 call_stack_size
        );

void csm_thread_cleanup_basic(csm_thread *t);

int csm_machine_init_basic(csm_machine *m, csm_init_profile *p);

void csm_machine_cleanup_basic(csm_machine *m);

#ifdef __cplusplus
}
#endif
#endif
