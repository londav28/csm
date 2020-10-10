#include "csm/native/module/mem.h"
#include "csm/machine/state.h"
#include "csm/bytecode/format.h"
#include "csm/types.h"

static struct csm_native_method nm__mem_gc_minimize = {
    .name               = "csm:mem_gc_minimize",
    .parameter_count    = 0,
    .parameter_str      = NULL,
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static struct csm_native_method nm__mem_gc_collect = {
    .name               = "csm:mem_gc_collect",
    .parameter_count    = 0,
    .parameter_str      = NULL,
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static struct csm_native_method nm__mem_gc_pin_on = {
    .name               = "csm:mem_gc_pin_on",
    .parameter_count    = 1,
    .parameter_str      = "A",
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static struct csm_native_method nm__mem_gc_pin_off = {
    .name               = "csm:mem_gc_pin_off",
    .parameter_count    = 1,
    .parameter_str      = "A",
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static struct csm_native_method nm__mem_gc_disable = {
    .name               = "csm:mem_gc_disable",
    .parameter_count    = 0,
    .parameter_str      = NULL,
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static struct csm_native_method nm__mem_gc_enable = {
    .name               = "csm:mem_gc_enable",
    .parameter_count    = 0,
    .parameter_str      = NULL,
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static struct csm_native_method nm__mem_gc_stats = {
    .name               = "csm:mem_gc_stats",
    .parameter_count    = 0,
    .parameter_str      = NULL,
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static struct csm_native_method nm__mem_malloc_stats = {
    .name               = "csm:mem_malloc_stats",
    .parameter_count    = 0,
    .parameter_str      = NULL,
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

/* Sadly we can't avoid this! Invoke sometime during setup. */
void csm_native_mem_setup(void)
{
    nm__mem_gc_minimize.handler = csm_native_mem_gc_minimize;
    nm__mem_gc_collect.handler = csm_native_mem_gc_collect;
    nm__mem_gc_pin_on.handler = csm_native_mem_gc_pin_on;
    nm__mem_gc_pin_off.handler = csm_native_mem_gc_pin_off;
    nm__mem_gc_disable.handler = csm_native_mem_gc_disable;
    nm__mem_gc_enable.handler = csm_native_mem_gc_enable;
    nm__mem_gc_stats.handler = csm_native_mem_gc_stats;
    nm__mem_malloc_stats.handler = csm_native_mem_malloc_stats;

    return;
}

void csm_native_mem_teardown(void)
{
    return;
}

/* TODO: Refactor to use libraries later? */
struct csm_native_method *csm_native_mem_methods[] = {
    &nm__mem_gc_minimize,
    &nm__mem_gc_collect,
    &nm__mem_gc_pin_on,
    &nm__mem_gc_pin_off,
    &nm__mem_gc_disable,
    &nm__mem_gc_enable,
    &nm__mem_gc_stats,
    &nm__mem_malloc_stats,
    NULL
};

