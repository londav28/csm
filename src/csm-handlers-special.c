#include "csm/types.h"
#include "csm/machine/state.h"
#include "csm/machine/frame.h"
#include "csm/machine/types.h"
#include "csm/bytecode/format.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


static struct csm_unpacked_op null_handler = {
    .op = 0, .handler = NULL
};


struct csm_unpacked_op
csm_special_terminate_unexpected(struct csm_thread* t)
{
    (void) t;
    assert("Unknown reason for termination." == 0);
    return null_handler;
}


struct csm_unpacked_op
csm_special_bad_array_size(struct csm_thread* t)
{
    (void) t;
    assert("Bad array size on array creation." == 0);
    return null_handler;
}


struct csm_unpacked_op
csm_special_bad_array_load(struct csm_thread* t)
{
    (void) t;
    assert("Bad array index on load." == 0);
    return null_handler;
}


struct csm_unpacked_op
csm_special_bad_array_store(struct csm_thread* t)
{
    (void) t;
    assert("Bad array index on store." == 0);
    return null_handler;
}


struct csm_unpacked_op
csm_special_divide_by_zero(struct csm_thread* t)
{
    (void) t;
    assert("Attempted to divide by zero." == 0);
    return null_handler;
}


/* Copy of the fetch code used in bytecode display. */
static struct csm_bc_string* get_bytecode_string(
            struct csm_bc_module* m,
            uint32_t idx
){
    if (idx >= m->strc)
        return NULL;
    return m->strs + idx;
}


struct csm_unpacked_op
csm_special_no_method(struct csm_thread* t)
{
    struct csm_bc_string* str = NULL;
    struct csm_cell val;
    (void) t;

    val = CSM_DATASTACK_POP(t);
    str = get_bytecode_string(CSM_MACHINE(t)->start_module, val.as.u32);

    assert(str != NULL);

    printf("No such method exists: %s\n", str->data);

    return null_handler;
}


struct csm_unpacked_op
csm_special_no_object(struct csm_thread* t)
{
    (void) t;
    assert("No such object exists." == 0);
    return null_handler;
}


struct csm_unpacked_op
csm_special_callstack_overflow(struct csm_thread* t)
{
    (void) t;
    assert("Callstack overflow occurred." == 0);
    return null_handler;
}


struct csm_unpacked_op
csm_special_memory_exhaustion(struct csm_thread* t)
{
    (void) t;
    assert("Memory exhaustion occurred." == 0);
    return null_handler;
}


struct csm_unpacked_op
csm_special_eox(struct csm_thread* t)
{
    (void) t;
    return null_handler;
}