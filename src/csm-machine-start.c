#include "csm/machine/start.h"
#include "csm/native/setup.h"
#include "csm/bytecode/format.h"
#include "csm/machine/state.h"
#include "csm/errorcodes.h"
#include "csm/machine/dispatch.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>


typedef struct csm_bc_module module;
typedef struct csm_bc_method method;
typedef struct csm_bc_object object;
typedef struct csm_bc_string string;


static string* get_string(module* m, uint32_t idx)
{
    if (idx >= m->strc) { return NULL; }

    return m->strs + idx;
}


static int is_string(module* m, uint32_t idx, const char* name)
{
    string* str = get_string(m, idx);

    if (str == NULL) { return 0; }

    return !strncmp(name, str->data, str->length);
}


method* resolve_start_method(module* m)
{
    method* f = NULL;
    uint32_t i = 0;

    for (i = 0; i < m->methodc; i++) {
        f = m->methods + i;
        if (is_string(m, f->name, "start")) { return f; }
    }

    return NULL;
}


int csm_start_basic(struct csm_machine* m, struct csm_bc_module* mod)
{
    method* start = NULL;
    int err = 0;

    start = resolve_start_method(mod);

    if (start == NULL) { return CSM_ERR_NOSTART; }

    /* Here is where we would normally check the validity of start. */
    m->start_module = mod;
    m->start_method = start;

    /* TODO: Maybe there is a better place to put this call? */
    /* We need to set up native method and object tables. */
    csm_native_modules_setup();

    err = csm_dispatch_basic(m->mainthread, m->start_method);
    if (err) { return err; }

    /* Ditto this one. */
    csm_native_modules_teardown();

    return 0;
}
