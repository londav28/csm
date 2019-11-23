#ifndef CSM_INCLUDE_CSM_BYTECODE_VERIFY_H_
#define CSM_INCLUDE_CSM_BYTECODE_VERIFY_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Used when unpacking type descriptors, and during type checking. */
typedef enum csm_bc_types {

    CSM_BC_TYPE_B = 0,
    CSM_BC_TYPE_S,
    CSM_BC_TYPE_D,
    CSM_BC_TYPE_Q,
    CSM_BC_TYPE_F,
    CSM_BC_TYPE_A

} csm_bc_types;

typedef struct csm_bc_typelabel {

    int type;
    int depth;
    struct csm_bc_object* optr;

} csm_bc_typelabel;

#ifdef __cplusplus
}
#endif
#endif
