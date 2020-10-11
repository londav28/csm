#include "csm/machine/start.h"
#include "csm/native/setup.h"
#include "csm/bytecode/format.h"
#include "csm/machine/state.h"
#include "csm/errorcodes.h"
#include "csm/machine/dispatch.h"
#include "csm/types.h"
#include <string.h>

typedef csm_bc_module module;
typedef csm_bc_method method;
typedef csm_bc_object object;
typedef csm_bc_string string;

static string *get_string(module *m, csm_u32 idx)
{
    if (idx >= m->strc) { return NULL; }
    return m->strs + idx;
}

static int is_string(module* m, csm_u32 idx, const char* name)
{
    string *str = get_string(m, idx);

    if (str == NULL) { return 0; }

    return !strncmp(name, str->data, str->length);
}

method *resolve_start_method(module *m)
{
    method *f = NULL;
    csm_u32 i = 0;

    for (i = 0; i < m->methodc; i++) {
        f = m->methods + i;
        if (is_string(m, f->name, "start")) { return f; }
    }

    return NULL;
}

int csm_start_basic(csm_machine *m, csm_bc_module *mod)
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
