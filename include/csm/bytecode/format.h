#ifndef CSM_INCLUDE_CSM_BYTECODE_FORMAT_H_
#define CSM_INCLUDE_CSM_BYTECODE_FORMAT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"

struct csm_bc_module;
struct csm_bc_ete;
struct csm_bc_tlabel;
struct csm_bc_method;
struct csm_bc_object;
struct csm_bc_string;

typedef struct csm_bc_module {

    /* Does module own buffer, or not? */
    csm_u64 bufsize;
    void *buf;

    csm_u8 status_0;
    csm_u8 status_1;
    csm_u8 status_2;
    csm_u8 status_3;

    csm_u32 methodc;
    struct csm_bc_method *methods;

    csm_u32 objectc;
    struct csm_bc_object *objects;

    csm_u32 strc;
    struct csm_bc_string *strs;

    csm_u32 int64c;
    csm_i64 *int64s;

    csm_u32 flt64c;
    csm_f64 *flt64s;

} csm_bc_module;

/* NOTE: Abbreviation for "exception table entry". */
typedef struct csm_bc_ete {

    csm_u32 type;
    csm_u32 start;
    csm_u32 end;
    csm_u32 target;

} csm_bc_ete;

/* TODO: Might move this somewhere else, not sure. */
typedef struct csm_bc_tlabel {

    csm_u8 type;
    char* name;

} csm_bc_tlabel;

typedef struct csm_bc_method {

    /* Individual flags are unpacked below at deserialization time. */
    csm_u8 status_0;
    csm_u8 status_1;

    /* Can be computed before post. */
    int is_void;
    int is_zero_arg;
    int is_throwing;

    csm_u32 name;
    csm_u32 debugsymbol;
    csm_u32 sigblock;
    csm_u8 limstack;
    csm_u8 limlocal;
    csm_u32 insbytec;
    csm_u8* insbytes;
    csm_u32 etec;
    csm_bc_ete* etes;

    /* Computed after load. */
    int is_post;
    csm_u32 post_paramc;
    struct csm_bc_tlabel* post_params;
    struct csm_bc_tlabel post_rtype;
    csm_u32 post_insc;

} csm_bc_method;

typedef struct csm_bc_object {

    csm_u32 name;
    csm_u32 fieldblock;

    csm_u8 status_0;
    csm_u8 status_1;

    /* Computed after load. */
    int post;
    csm_u32 _fieldc_;
    struct csm_bc_tlabel *_fields_;

} csm_bc_object;

typedef struct csm_bc_string {

    csm_u32 length;
    char* data;

} csm_bc_string;

int csm_bc_module_init_file(csm_bc_module* out, const char* name);

int csm_bc_module_init(csm_bc_module* out, void* buf, size_t size);

void csm_bc_module_deinit(csm_bc_module* m);

#ifdef __cplusplus
}
#endif
#endif
