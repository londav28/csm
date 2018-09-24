#include "csm/machine/state.h"
#include "csm/machine/frame.h"
#include "csm/bytecode/format.h"
#include "csm/memory/gc.h"
#include "csm/memory/malloc.h"
#include <inttypes.h>
#include <assert.h>


/* TODO: Should make this a common method. */
static struct csm_bc_string* get_bytecode_string(
            struct csm_bc_module* m,
            uint32_t idx
){
    if (idx >= m->strc) { return NULL; }
    return m->strs + idx;
}


void
csm_native_mem_gc_minimize(struct csm_thread* t)
{
    (void) t;
    assert("Not implemented yet." == 0);
    (void) get_bytecode_string(NULL, 0);
}


void
csm_native_mem_gc_collect(struct csm_thread* t)
{
    (void) t;
    assert("Not implemented yet." == 0);
}


void
csm_native_mem_gc_pin_on(struct csm_thread* t)
{
    (void) t;
    assert("Not implemented yet." == 0);
}


void
csm_native_mem_gc_pin_off(struct csm_thread* t)
{
    (void) t;
    assert("Not implemented yet." == 0);
}


void
csm_native_mem_gc_disable(struct csm_thread* t)
{
    (void) t;
    assert("Not implemented yet." == 0);
}


void
csm_native_mem_gc_enable(struct csm_thread* t)
{
    (void) t;
    assert("Not implemented yet." == 0);
}


void
csm_native_mem_gc_stats(struct csm_thread* t)
{
    (void) t;
    csm_gc_stats();
}
