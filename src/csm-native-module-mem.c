#include "csm/machine/state.h"
#include "csm/machine/frame.h"
#include "csm/bytecode/format.h"
#include "csm/memory/gc.h"
#include "csm/memory/malloc.h"
#include <inttypes.h>
#include <assert.h>

/* TODO: Should make this a common method. */
static csm_bc_string *get_bytecode_string(csm_bc_module *m, csm_u32 idx) {
    if (idx >= m->strc) { return NULL; }
    return m->strs + idx;
}

void csm_native_mem_gc_minimize(csm_thread *t)
{
    (void) t;
    assert("Not implemented yet." == 0);
    (void) get_bytecode_string(NULL, 0);
    return;
}

void csm_native_mem_gc_collect(csm_thread *t)
{
    (void) t;
    assert("Not implemented yet." == 0);
    return;
}

void csm_native_mem_gc_pin_on(csm_thread *t)
{
    (void) t;
    assert("Not implemented yet." == 0);
    return;
}

void csm_native_mem_gc_pin_off(csm_thread *t)
{
    (void) t;
    assert("Not implemented yet." == 0);
    return;
}

void csm_native_mem_gc_disable(csm_thread *t)
{
    (void) t;
    assert("Not implemented yet." == 0);
    return;
}

void csm_native_mem_gc_enable(csm_thread *t)
{
    (void) t;
    assert("Not implemented yet." == 0);
    return;
}

void csm_native_mem_gc_stats(csm_thread *t)
{
    (void) t;
    csm_gc_stats();
    return;
}

void csm_native_mem_malloc_stats(csm_thread *t)
{
    (void) t;
    csm_malloc_stats();
    return;
}
