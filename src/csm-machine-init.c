#include "csm/machine/init.h"
#include "csm/machine/state.h"
#include "csm/memory/malloc.h"
#include "csm/errorcodes.h"
#include <stdlib.h>
#include <string.h>


#define CSM_INIT_GLOBAL_BUF         65536       /* 512KB    */
#define CSM_INIT_DATA_STACK         4096        /* 32KB     */
#define CSM_INIT_CALL_STACK         2048        /* 16KB     */
#define CSM_INIT_GC_LATENCY         0           /* ?????    */


static struct csm_init_profile profile_default = {

    .datastack_limit = CSM_INIT_DATA_STACK,
    .callstack_limit = CSM_INIT_CALL_STACK,
    .gc_latency_level = CSM_INIT_GC_LATENCY

};


/* TODO: Ensure that datastack_size/callstack_size is not 0. */
struct csm_init_profile* sanitize_profile(struct csm_init_profile* p)
{
    /* Clarify the actual profile we will use. */
    if (p == NULL) { return &profile_default; }
    return p;
}


/* TODO: Need to make sure to check that we are not overflowing? */
int csm_thread_init_basic(
            struct csm_thread* t,
            struct csm_machine* m,
            uint32_t datastack_size,
            uint32_t callstack_size
){
    int err;
    size_t bytes;

    memset(t, 0, sizeof(*t));

    /* Best way to prevent overflow in this situation? */
    bytes = datastack_size * sizeof(*t->datastack_bot);
    t->datastack_bot = csm_malloc(bytes);
    if (t->datastack_bot == NULL) { return CSM_ERR_MEMORY; }
    memset(t->datastack_bot, 0, bytes);

    t->datastack_size = datastack_size;

    /* Deliberately set to one above last element in buffer. */
    t->datastack_top = t->datastack_bot + datastack_size;
    t->datastack_pos = t->datastack_top;

    /* Best way to prevent overflow in this situation? */
    bytes = callstack_size * sizeof(*t->callstack_bot);
    t->callstack_bot = csm_malloc(bytes);
    if (t->callstack_bot == NULL) {
        err = CSM_ERR_MEMORY;
        goto _unwind_0;
    }
    memset(t->callstack_bot, 0, bytes);

    t->callstack_size = callstack_size;

    /* Deliberately set to one above last element in buffer. */
    t->callstack_top = t->callstack_bot + callstack_size;
    t->callstack_pos = t->callstack_top;

    // Make sure to set the parent machine!
    t->machine = m;

    return 0;

_unwind_0:

    csm_free(t->datastack_bot);
    return err;
}


void csm_thread_cleanup_basic(struct csm_thread* t)
{
    if (t == NULL) { return; }
    if (t->datastack_bot != NULL) { csm_free(t->datastack_bot); }
    if (t->callstack_bot != NULL) { csm_free(t->callstack_bot); }
}


int csm_machine_init_basic(
            struct csm_machine* m,
            struct csm_init_profile* p
){
    struct csm_init_profile *profile;
    int err;
    size_t bytes;

    profile = sanitize_profile(p);

    memset(m, 0, sizeof(*m));

    // This would automatically fail on 16-bit machines?
    bytes = CSM_INIT_GLOBAL_BUF * sizeof(*m->globals);
    m->globals = csm_malloc(bytes);
    if (m->globals == NULL) { return CSM_ERR_MEMORY; }
    memset(m->globals, 0, bytes);

    bytes = sizeof(*m->mainthread);
    m->mainthread = csm_malloc(bytes);
    if (m->mainthread == NULL) {
        err = CSM_ERR_MEMORY;
        goto _unwind_0;
    }
    memset(m->mainthread, 0, bytes);

    // Do we just always assume the profile is passed here?
    err = csm_thread_init_basic(
                m->mainthread,
                m,
                profile->datastack_limit,
                profile->callstack_limit
            );

    if (err) { goto _unwind_1; }

    return 0;

_unwind_1:

    csm_free(m->mainthread);

_unwind_0:

    csm_free(m->globals);
    return err;
}


void csm_machine_cleanup_basic(struct csm_machine* m)
{
    if (m == NULL) { return; }

    if (m->globals != NULL) { csm_free(m->globals); }

    if (m->mainthread) {
        csm_thread_cleanup_basic(m->mainthread);
        csm_free(m->mainthread);
    }
}