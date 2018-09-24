#ifndef CSM_BYTECODE_VERIFY_H_
#define CSM_BYTECODE_VERIFY_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Used when unpacking type descriptors, and during type checking. */
enum csm_bc_types {

    CSM_BC_TYPE_B = 0,
    CSM_BC_TYPE_S,
    CSM_BC_TYPE_D,
    CSM_BC_TYPE_Q,
    CSM_BC_TYPE_F,
    CSM_BC_TYPE_A

};


struct csm_bc_typelabel {

    int type;
    int depth;
    struct csm_bc_object* optr;

};




#ifdef __cplusplus
}
#endif

#endif