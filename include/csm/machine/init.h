#ifndef CSM_MACHINE_INIT_H_
#define CSM_MACHINE_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>
#include <stdint.h>


struct csm_thread;
struct csm_machine;


struct csm_init_profile {

    uint32_t datastack_limit;
    uint32_t callstack_limit;
    uint8_t gc_latency_level;

};


int
csm_thread_init_basic(
        struct csm_thread *t,
        struct csm_machine *m,
        uint32_t data_stack_size,
        uint32_t call_stack_size
);


void
csm_thread_cleanup_basic(struct csm_thread *t);


int
csm_machine_init_basic(struct csm_machine *m, struct csm_init_profile *p);


void
csm_machine_cleanup_basic(struct csm_machine *m);


#ifdef __cplusplus
}
#endif

#endif