#include "csm/machine/state.h"
#include "csm/machine/frame.h"
#include "csm/bytecode/format.h"
#include <inttypes.h>
#include <stdio.h>


/* TODO: Should make this a common method. */
static struct csm_bc_string* get_bytecode_string(
            struct csm_bc_module* m,
            uint32_t idx
){
    if (idx >= m->strc) { return NULL; }
    return m->strs + idx;
}


void
csm_native_io_flush(struct csm_thread* t)
{
    (void) t;
    fflush(stdout);
}


void
csm_native_io_print_strconst(struct csm_thread* t)
{
    struct csm_bc_string* str = NULL;
    uint32_t idx = 0;

    idx = CSM_DATASTACK_POP(t).as.u32;
    str = get_bytecode_string(CSM_MACHINE(t)->start_module, idx);
    
    printf("%s", str->data);
    return;
}


void
csm_native_io_print_b(struct csm_thread* t)
{
    int8_t val = CSM_DATASTACK_POP(t).as.i8;
    printf("%" PRId8, val);
}


void
csm_native_io_print_s(struct csm_thread* t)
{
    int16_t val = CSM_DATASTACK_POP(t).as.i16;
    printf("%" PRId16, val);
}


void
csm_native_io_print_d(struct csm_thread* t)
{
    int32_t val = CSM_DATASTACK_POP(t).as.i32;
    printf("%" PRId32, val);
}


void
csm_native_io_print_q(struct csm_thread* t)
{
    int64_t val = CSM_DATASTACK_POP(t).as.i64;
    printf("%" PRId64, val);
}


void
csm_native_io_print_f(struct csm_thread* t)
{
    double val = CSM_DATASTACK_POP(t).as.f64;
    printf("%f", val);
}


void
csm_native_io_print_a(struct csm_thread* t)
{
    void* val = CSM_DATASTACK_POP(t).as.raw;
    printf("%p", val);
}


void
csm_native_io_print_b_ascii(struct csm_thread* t)
{
    int8_t val = CSM_DATASTACK_POP(t).as.i8;
    printf("%c", val);
}


void
csm_native_io_print_d_utf8(struct csm_thread* t)
{
    (void) t;
    assert("Not implemented yet." == 0);
}


void
csm_native_io_print_arr_b_ascii(struct csm_thread* t)
{
    (void) t;
    assert("Not implemented yet." == 0);
}


void
csm_native_io_print_arr_b_utf8(struct csm_thread* t)
{
    (void) t;
    assert("Not implemented yet." == 0);
}