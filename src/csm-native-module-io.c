#include "csm/machine/state.h"
#include "csm/machine/frame.h"
#include "csm/bytecode/format.h"
#include "csm/types.h"
#include <inttypes.h>
#include <stdio.h>

/* TODO: Should make this a common method. */
static csm_bc_string *get_bytecode_string(csm_bc_module *m, csm_u32 idx) {
    if (idx >= m->strc) { return NULL; }
    return m->strs + idx;
}

void csm_native_io_flush(csm_thread *t)
{
    (void) t;
    fflush(stdout);
    return;
}

void csm_native_io_print_strconst(csm_thread *t)
{
    csm_bc_string *str = NULL;
    csm_u32 idx = 0;

    idx = CSM_DATASTACK_POP(t).as.u32;
    str = get_bytecode_string(CSM_MACHINE(t)->start_module, idx);
    
    printf("%s", str->data);
    return;
}

void csm_native_io_print_b(csm_thread *t)
{
    csm_i8 val = CSM_DATASTACK_POP(t).as.i8;
    printf("%" PRId8, val);
    return;
}

void csm_native_io_print_s(csm_thread *t)
{
    csm_i16 val = CSM_DATASTACK_POP(t).as.i16;
    printf("%" PRId16, val);
    return;
}

void csm_native_io_print_d(csm_thread *t)
{
    csm_i32 val = CSM_DATASTACK_POP(t).as.i32;
    printf("%" PRId32, val);
    return;
}

void csm_native_io_print_q(csm_thread *t)
{
    csm_i64 val = CSM_DATASTACK_POP(t).as.i64;
    printf("%" PRId64, val);
    return;
}

void csm_native_io_print_f(csm_thread *t)
{
    csm_f64 val = CSM_DATASTACK_POP(t).as.f64;
    printf("%f", val);
    return;
}

void csm_native_io_print_a(csm_thread *t)
{
    void *val = CSM_DATASTACK_POP(t).as.raw;
    printf("%p", val);
    return;
}

void csm_native_io_print_b_ascii(csm_thread *t)
{
    csm_i8 val = CSM_DATASTACK_POP(t).as.i8;
    printf("%c", val);
    return;
}

void csm_native_io_print_d_utf8(csm_thread *t)
{
    (void) t;
    assert("Not implemented yet." == 0);
    return;
}

void csm_native_io_print_arr_b_ascii(csm_thread *t)
{
    (void) t;
    assert("Not implemented yet." == 0);
    return;
}

void csm_native_io_print_arr_b_utf8(csm_thread *t)
{
    (void) t;
    assert("Not implemented yet." == 0);
    return;
}

