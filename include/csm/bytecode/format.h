#ifndef CSM_BYTECODE_FORMAT_H_
#define CSM_BYTECODE_FORMAT_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "csm/types.h"


struct csm_bc_module {

    /* Does module own buffer, or not? */
    uint64_t bufsize;
    void* buf;

    uint8_t status_0;
    uint8_t status_1;
    uint8_t status_2;
    uint8_t status_3;
    
    uint32_t methodc;
    struct csm_bc_method* methods;
    
    uint32_t objectc;
    struct csm_bc_object* objects;
    
    uint32_t strc;
    struct csm_bc_string* strs;
    
    uint32_t int64c;
    int64_t* int64s;
    
    uint32_t flt64c;
    double* flt64s;

};


/* NOTE: Abbreviation for "exception table entry". */
struct csm_bc_ete {

    uint32_t type;
    uint32_t start;
    uint32_t end;
    uint32_t target;

};


struct csm_bc_tlabel {

    uint8_t type;
    char* name;

};


struct csm_bc_method {
    
    /* Computed at deserialization. */
    uint8_t status_0;
    uint8_t status_1;
    uint32_t name;
    uint32_t debugsymbol;
    uint32_t sigblock;
    uint8_t limstack;
    uint8_t limlocal;
    uint32_t insbytec;
    uint8_t* insbytes;
    uint32_t etec;
    struct csm_bc_ete* etes;

    /* Computed after load. */
    int post;
    uint32_t _paramc_;
    struct csm_bc_tlabel* _params_;
    struct csm_bc_tlabel _rtype_;
    uint32_t _insc_;

};


struct csm_bc_object {

    uint8_t status_0;
    uint32_t name;
    uint32_t fieldblock;

    /* Computed after load. */
    int post;
    uint32_t _fieldc_;
    struct csm_bc_tlabel* _fields_;
}; 


struct csm_bc_string {

    uint32_t length;
    char* data;

};


/**___________________________________________________________________________
 * FUNCTIONS
 */


int
csm_bc_module_init_file(struct csm_bc_module* out, const char* name);


int
csm_bc_module_init(struct csm_bc_module* out, void* buf, size_t size);


void
csm_bc_module_deinit(struct csm_bc_module* m);


#ifdef __cplusplus
}
#endif

#endif