#ifndef CSM_START_H_
#define CSM_START_H_

#ifdef __cplusplus
extern "C" {
#endif


struct csm_bc_module;
struct csm_machine;


int
csm_start_basic(struct csm_machine *m, struct csm_bc_module *mod);


#ifdef __cplusplus
}
#endif

#endif