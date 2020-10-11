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

    int is_loaded_from_file;
    const char* origin;

    /* Does module own buffer, or not? */
    csm_u64 bufsize;
    void *buf;

    csm_u8 status_0;
    csm_u8 status_1;
    csm_u8 status_2;
    csm_u8 status_3;

    csm_u32 modname;

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

    int is_post;
    const char* post_name;

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

    struct csm_bc_module* module;

    /* Individual flags are unpacked below at deserialization time. */
    csm_u8 status_0;
    csm_u8 status_1;

    /* Can be computed before post. */
    int is_void;
    int is_zero_arg;

    csm_u32 name;
    csm_u32 debugsymbol;
    csm_u32 sigblock;
    csm_u32 rtype;
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

    /* TODO: Make this a u8 traced array. */
    const char* post_name;

} csm_bc_method;

typedef struct csm_bc_object {

    struct csm_bc_module* parent;

    csm_u32 name;
    csm_u32 fieldblock;

    csm_u8 status_0;
    csm_u8 status_1;

    /* Computed after load. */
    int is_post;
    csm_u32 post_fieldc;
    struct csm_bc_tlabel *post_fields;
    csm_u64 post_size;

} csm_bc_object;

typedef struct csm_bc_string {

    char* data;
    csm_u32 length;

} csm_bc_string;

int csm_bc_module_init_file(csm_bc_module* out, const char* name);

int csm_bc_module_init(csm_bc_module* out, void* buf, size_t size);

void csm_bc_module_deinit(csm_bc_module* m);

int csm_bc_get_string(csm_bc_string* out, csm_bc_module *m, csm_u32 idx);

#ifdef __cplusplus
}
#endif
#endif
