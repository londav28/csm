#ifndef CSM_MACHINE_DISPATCH_H_
#define CSM_MACHINE_DISPATCH_H_

#ifdef __cplusplus
extern "C" {
#endif


struct csm_bc_method;
struct csm_thread;


int
csm_dispatch_basic(struct csm_thread *t, struct csm_bc_method *m);


#ifdef __cplusplus
}
#endif

#endif