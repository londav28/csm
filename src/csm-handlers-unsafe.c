#include "csm/machine/state.h"
#include "csm/machine/frame.h"
#include "csm/bytecode/opcodes.h"
#include "csm/bytecode/format.h"
#include "csm/native/resolution.h"
#include "csm/native/module/io.h"
#include "csm/memory/gc.h"
#include "csm/handlers/special.h"
#include "csm/memset.h"
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// TODO: Macro assumes struct field name containing union?
#define CSM_BINARY_OP(op, awidth, rwidth, thread)                           \
    do {                                                                    \
        struct csm_cell a, b, v;                                            \
        b = CSM_DATASTACK_POP(thread);                                      \
        a = CSM_DATASTACK_POP(thread);                                      \
        v.as.rwidth = (a.as.awidth op b.as.awidth);                         \
        CSM_DATASTACK_PUSH(thread, v);                                      \
    } while (0)

#define CSM_UNARY_OP(op, twidth, fwidth, thread)                            \
    do {                                                                    \
        struct csm_cell v;                                                  \
        v = CSM_DATASTACK_POP(thread);                                      \
        v.as.twidth = op (v.as.fwidth);                                     \
        CSM_DATASTACK_PUSH(thread, v);                                      \
    } while (0)

// TODO: Can we unparameterize this a little bit?
#define CSM_THREEWAY_COMPARISON_OP(awidth, thread)                          \
    do {                                                                    \
        struct csm_cell a, b, v;                                            \
        b = CSM_DATASTACK_POP(thread);                                      \
        a = CSM_DATASTACK_POP(thread);                                      \
        if (a.as.awidth < b.as.awidth) {                                    \
            v.as.i64 = (-1);                                                \
        } else if (a.as.awidth > b.as.awidth) {                             \
            v.as.i64 = 1;                                                   \
        } else {                                                            \
            v.as.i64 = 0;                                                   \
        }                                                                   \
        CSM_DATASTACK_PUSH(thread, v);                                      \
    } while (0)

#define CSM_COMPARE_ZERO_JUMP_OP(cmp, thread)                               \
    do {                                                                    \
        struct csm_cell v;                                                  \
        uint32_t j;                                                         \
        v = CSM_DATASTACK_POP(thread);                                      \
        j = CSM_CUR_STREAM_U32(thread);                                     \
        if (v.as.i64 cmp 0) {                                               \
            csm_stream_shift_abs(CSM_CUR_STREAM(thread), j);                \
        }                                                                   \
    } while (0)

#define CSM_PUSH_OP(awidth, readw, thread)                                  \
    do {                                                                    \
        struct csm_cell v;                                                  \
        v.as.awidth = readw(thread);                                        \
        CSM_DATASTACK_PUSH(thread, v);                                      \
    } while (0)


/* TODO: Bytes calculation can overflow. */
/* TODO: Load and store exceptions could mention array kind. */
#define CSM_PUSH_ARRAY_OP(ctype, what, thread)                              \
    do {                                                                    \
        struct csm_array_header* array = NULL;                              \
        struct csm_cell v;                                                  \
        int64_t length = 0;                                                 \
        size_t stride = 0;                                                  \
        size_t bytes = 0;                                                   \
        struct csm_gc_header hdr;                                           \
        length = CSM_DATASTACK_POP(thread).as.i64;                          \
        if (length < 0) {                                                   \
            struct csm_cell bad;                                            \
            bad.as.i64 = length;                                            \
            CSM_DATASTACK_PUSH(thread, bad);                                \
            CSM_RETURN_CUSTOM(thread, csm_special_bad_array_size);          \
        }                                                                   \
        stride = sizeof(ctype);                                             \
        /* TODO: Should align this amount later? */                         \
        bytes = (length * stride) + sizeof(*array);                         \
        hdr.string = NULL;                                                  \
        hdr.tag = CSM_GC_TAG_ARRAY;                                         \
        hdr.bytes = bytes;                                                  \
        array = csm_gc_alloc(hdr, thread);                                  \
        if (array == NULL) {                                                \
            struct csm_cell bad;                                            \
            bad.as.u64 = bytes;                                             \
            CSM_DATASTACK_PUSH(thread, bad);                                \
            CSM_RETURN_CUSTOM(thread, csm_special_memory_exhaustion);       \
        }                                                                   \
        array->length = length;                                             \
        array->dimensions = 1;                                              \
        array->kind = what;                                                 \
        array->data = (array + 1);                                          \
        csm_memset(array->data, 0, (length * stride));                      \
        v.as.raw = array;                                                   \
        CSM_DATASTACK_PUSH(thread, v);                                      \
    } while (0)


/* Need this so that bytecode can decode the next op. */
csm_handler csm_handlers_unsafe[];

/* Just for convenience! */
csm_handler* hds = csm_handlers_unsafe;


static struct csm_unpacked_op op_nop(struct csm_thread* t)
{
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_ldl(struct csm_thread* t)
{
    struct csm_cell value;
    uint8_t idx = 0;

    idx = CSM_CUR_STREAM_U8(t);
    value = CSM_CUR_FRAME_LOCALS(t)[idx];
    CSM_DATASTACK_PUSH(t, value);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_stl(struct csm_thread* t)
{
    struct csm_cell value;
    uint8_t idx = 0;

    idx = CSM_CUR_STREAM_U8(t);
    value = CSM_DATASTACK_POP(t);
    CSM_CUR_FRAME_LOCALS(t)[idx] = value;
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_ldg(struct csm_thread* t)
{
    struct csm_cell value;
    uint16_t idx = 0;

    idx = CSM_CUR_STREAM_U16(t);
    value = CSM_GLOBALS(t)[idx];
    CSM_DATASTACK_PUSH(t, value);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_stg(struct csm_thread* t)
{
    struct csm_cell value;
    uint16_t idx = 0;

    idx = CSM_CUR_STREAM_U16(t);
    value = CSM_DATASTACK_POP(t);
    CSM_GLOBALS(t)[idx] = value;
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_lfd(struct csm_thread* t)
{
    struct csm_cell *object = NULL;
    struct csm_cell value;
    uint16_t idx = 0;

    idx = CSM_CUR_STREAM_U16(t);
    object = CSM_DATASTACK_POP(t).as.raw;
    value = object[idx];
    CSM_DATASTACK_PUSH(t, value);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_sfd(struct csm_thread* t)
{
    struct csm_cell *object = NULL;
    struct csm_cell value;
    uint16_t idx = 0;

    idx = CSM_CUR_STREAM_U16(t);
    value = CSM_DATASTACK_POP(t);
    object = CSM_DATASTACK_POP(t).as.raw;
    object[idx] = value;
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_ldsc(struct csm_thread* t)
{
    struct csm_cell v;

    v.as.u32 = CSM_CUR_STREAM_U32(t);
    CSM_DATASTACK_PUSH(t, v);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_pop(struct csm_thread* t)
{
    struct csm_cell unused;

    unused = CSM_DATASTACK_POP(t);
    (void) unused;
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_swp(struct csm_thread* t)
{
    struct csm_cell v1, v2;

    v1 = CSM_DATASTACK_POP(t);
    v2 = CSM_DATASTACK_POP(t);
    CSM_DATASTACK_PUSH(t, v1);
    CSM_DATASTACK_PUSH(t, v2);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_dup(struct csm_thread* t)
{
    struct csm_cell v;

    v = CSM_DATASTACK_POP(t);
    CSM_DATASTACK_PUSH(t, v);
    CSM_DATASTACK_PUSH(t, v);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_psh_i8(struct csm_thread* t)
{
    CSM_PUSH_OP(i64, CSM_CUR_STREAM_I8, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_psh_i16(struct csm_thread* t)
{
    CSM_PUSH_OP(i64, CSM_CUR_STREAM_I16, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_psh_i32(struct csm_thread* t)
{
    CSM_PUSH_OP(i64, CSM_CUR_STREAM_I32, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_psh_i64(struct csm_thread* t)
{
    CSM_PUSH_OP(i64, CSM_CUR_STREAM_I64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_psh_f64(struct csm_thread* t)
{
    CSM_PUSH_OP(f64, CSM_CUR_STREAM_F64, t);
    CSM_DECODE_RETURN(t, hds);
}


/* TODO: Plan logic for object allocation! */
static struct csm_unpacked_op op_psh_obj(struct csm_thread* t)
{
    uint32_t idx = 0;

    idx = CSM_CUR_STREAM_U32(t);
    (void) idx;

    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_psh_nil(struct csm_thread* t)
{
    struct csm_cell value;

    value.as.raw = NULL;
    CSM_DATASTACK_PUSH(t, value);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_par_i8(struct csm_thread* t)
{
    CSM_PUSH_ARRAY_OP(int8_t, CSM_ARRAY_TYPE_I8, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_par_i16(struct csm_thread* t)
{
    CSM_PUSH_ARRAY_OP(int16_t, CSM_ARRAY_TYPE_I16, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_par_i32(struct csm_thread* t)
{
    CSM_PUSH_ARRAY_OP(int32_t, CSM_ARRAY_TYPE_I32, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_par_i64(struct csm_thread* t)
{
    CSM_PUSH_ARRAY_OP(int64_t, CSM_ARRAY_TYPE_I64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_par_f64(struct csm_thread* t)
{
    CSM_PUSH_ARRAY_OP(double, CSM_ARRAY_TYPE_F64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_par_obj(struct csm_thread* t)
{
    CSM_PUSH_ARRAY_OP(void*, CSM_ARRAY_TYPE_OBJ, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_lai(struct csm_thread* t)
{
    struct csm_array_header* hdr = NULL;
    struct csm_cell val;
    int64_t idx = 0;

    hdr = CSM_DATASTACK_POP(t).as.raw;
    idx = CSM_DATASTACK_POP(t).as.i64;

    /* TODO: Throw an exception (or exception handler) instead. */
    if (idx < 0 || idx >= hdr->length) {
        struct csm_cell bad;
        bad.as.i64 = idx;
        CSM_DATASTACK_PUSH(t, bad);
        CSM_RETURN_CUSTOM(t, csm_special_bad_array_load);
    }

    /* From memcpy to a more blocky switch. Better or worse? */
    switch(hdr->kind) {
    case CSM_ARRAY_TYPE_I8:
        val.as.i64 = ((int8_t*) hdr->data)[idx];
        break;
    case CSM_ARRAY_TYPE_I16:
        val.as.i64 = ((int16_t*) hdr->data)[idx];
        break;
    case CSM_ARRAY_TYPE_I32:
        val.as.i64 = ((int32_t*) hdr->data)[idx];
        break;
    case CSM_ARRAY_TYPE_I64:
        val.as.i64 = ((int64_t*) hdr->data)[idx];
        break;
    case CSM_ARRAY_TYPE_F64:
        val.as.f64 = ((double*) hdr->data)[idx];
        break;
    case CSM_ARRAY_TYPE_OBJ:
        val.as.raw = ((void**) hdr->data)[idx];
        break;
    default:
        assert("Should never reach here!" == 0);
        break;
    }

    CSM_DATASTACK_PUSH(t, val);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_sai(struct csm_thread* t)
{
    // Pop items (even if unused) to preserve stack height.
    struct csm_array_header* hdr = NULL;
    struct csm_cell val;
    int64_t idx = 0;

    hdr = CSM_DATASTACK_POP(t).as.raw;
    idx = CSM_DATASTACK_POP(t).as.i64;
    val = CSM_DATASTACK_POP(t);

    /* TODO: Throw an exception (or exception handler) instead. */
    if (idx < 0 || idx >= hdr->length) {
        struct csm_cell bad;
        bad.as.i64 = idx;
        CSM_DATASTACK_PUSH(t, bad);
        CSM_RETURN_CUSTOM(t, csm_special_bad_array_store);
    }

    /* From memcpy to a more blocky switch. Better or worse? */
    switch(hdr->kind) {
    case CSM_ARRAY_TYPE_I8:
        ((int8_t*) hdr->data)[idx] = val.as.i8;
        break;
    case CSM_ARRAY_TYPE_I16:
        ((int16_t*) hdr->data)[idx] = val.as.i16;
        break;
    case CSM_ARRAY_TYPE_I32:
        ((int32_t*) hdr->data)[idx] = val.as.i32;
        break;
    case CSM_ARRAY_TYPE_I64:
        ((int64_t*) hdr->data)[idx] = val.as.i64;
        break;
    case CSM_ARRAY_TYPE_F64:
        ((double*) hdr->data)[idx] = val.as.f64;
        break;
    case CSM_ARRAY_TYPE_OBJ:
        ((void**) hdr->data)[idx] = val.as.raw;
        break;
    default:
        assert("Should never reach here!" == 0);
        break;
    }

    CSM_DECODE_RETURN(t, hds);
}


// TODO: Wait until GC interface and object model for this.
static struct csm_unpacked_op op_alen(struct csm_thread* t)
{
    struct csm_array_header* header = NULL;
    struct csm_cell v;

    header = CSM_DATASTACK_POP(t).as.raw;
    assert(header != NULL);
    v.as.i64 = header->length;

    CSM_DATASTACK_PUSH(t, v);

    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_and(struct csm_thread* t)
{
    CSM_BINARY_OP(&, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_or(struct csm_thread* t)
{
    CSM_BINARY_OP(|, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_xor(struct csm_thread* t)
{
    CSM_BINARY_OP(^, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_not(struct csm_thread* t)
{
    CSM_UNARY_OP(~, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_shl(struct csm_thread* t)
{
    CSM_BINARY_OP(<<, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_shr(struct csm_thread* t)
{
    CSM_BINARY_OP(>>, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_add_i64(struct csm_thread* t)
{
    CSM_BINARY_OP(+, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_sub_i64(struct csm_thread* t)
{
    CSM_BINARY_OP(-, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_mul_i64(struct csm_thread* t)
{
    CSM_BINARY_OP(*, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_div_i64(struct csm_thread* t)
{
    CSM_BINARY_OP(/, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_mod_i64(struct csm_thread* t)
{
    CSM_BINARY_OP(%, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_neg_i64(struct csm_thread* t)
{
    CSM_UNARY_OP(-, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_add_f64(struct csm_thread* t)
{
    CSM_BINARY_OP(+, f64, f64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_sub_f64(struct csm_thread* t)
{
    CSM_BINARY_OP(-, f64, f64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_mul_f64(struct csm_thread* t)
{
    CSM_BINARY_OP(*, f64, f64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_div_f64(struct csm_thread* t)
{
    CSM_BINARY_OP(/, f64, f64, t);
    CSM_DECODE_RETURN(t, hds);
}


/* TODO: Implement support for floating point modulo. */
static struct csm_unpacked_op op_mod_f64(struct csm_thread* t)
{
    assert("Floating point modulo currently unsupported." == 0);
    CSM_ALIGN_DECODE_RETURN(t, t->last_op, hds);
}


static struct csm_unpacked_op op_neg_f64(struct csm_thread* t)
{
    CSM_UNARY_OP(-, f64, f64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_cst_i64_f64(struct csm_thread* t)
{
    CSM_UNARY_OP((double), f64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_cst_f64_i64(struct csm_thread* t)
{
    CSM_UNARY_OP((int64_t), i64, f64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_cmp_i64(struct csm_thread* t)
{
    CSM_THREEWAY_COMPARISON_OP(i64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_cmp_f64(struct csm_thread* t)
{
    CSM_THREEWAY_COMPARISON_OP(f64, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_refcmp(struct csm_thread* t)
{
    struct csm_cell a, b, v;

    b = CSM_DATASTACK_POP(t);
    a = CSM_DATASTACK_POP(t);

    v.as.i64 = a.as.raw == b.as.raw ? 1 : 0;

    CSM_DATASTACK_PUSH(t, v);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_jmp_eqz(struct csm_thread* t)
{
    CSM_COMPARE_ZERO_JUMP_OP(==, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_jmp_nez(struct csm_thread* t)
{
    CSM_COMPARE_ZERO_JUMP_OP(!=, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_jmp_ltz(struct csm_thread* t)
{
    CSM_COMPARE_ZERO_JUMP_OP(<, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_jmp_lez(struct csm_thread* t)
{
    CSM_COMPARE_ZERO_JUMP_OP(<=, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_jmp_gtz(struct csm_thread* t)
{
    CSM_COMPARE_ZERO_JUMP_OP(>, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_jmp_gez(struct csm_thread* t)
{
    CSM_COMPARE_ZERO_JUMP_OP(>=, t);
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_jmp(struct csm_thread* t)
{
    int32_t jump = 0;

    jump = CSM_CUR_STREAM_U32(t);
    csm_stream_shift_abs(CSM_CUR_STREAM(t), jump);
    CSM_DECODE_RETURN(t, hds);
}


/* TODO: Figure out object model/metadata for this. */
static struct csm_unpacked_op op_typeof(struct csm_thread* t)
{
    CSM_ALIGN_DECODE_RETURN(t, t->last_op, hds);
}


static struct csm_unpacked_op
cbridge_bc_(uint32_t idx, struct csm_descriptor d, struct csm_thread* t)
{
    struct csm_bc_method* f = d.as.bc_method;
    struct csm_cell* new_frame_top = NULL;
    uint32_t i = 0;
    uint16_t localspace = 0;

    /* Calculate new frame ceiling, is also saved datastack. */
    new_frame_top = CSM_DATASTACK_POS(t) + f->_paramc_;

    /* Save current frame's datastack pos, then move FP down. */
    CSM_CUR_FRAME(t)->saved_datastack_pos = new_frame_top;
    CSM_CUR_FRAME(t)--;

    /* Push the string constant index of the function name! */
    if (CSM_CUR_FRAME(t) < t->callstack_bot) {
        struct csm_cell bad = {.as.u32 = idx};
        CSM_DATASTACK_PUSH(t, bad);
        CSM_RETURN_CUSTOM(t, csm_special_callstack_overflow);
    }

    /* This value should not exceed 256. */
    localspace = f->limstack + f->limlocal;
    assert(localspace <= 256);

    CSM_CUR_FRAME_LOCALS(t) = new_frame_top - localspace;

    for (i = 0; i < f->_paramc_; i++) {
        CSM_CUR_FRAME_LOCALS(t)[i] = CSM_DATASTACK_POP(t);
    }

    /* Test assertion should never fail. */
    assert(new_frame_top == CSM_DATASTACK_POS(t));

    /* End by setting datastack pointer to start of locals. */
    CSM_DATASTACK_POS(t) = CSM_CUR_FRAME_LOCALS(t);

    /* Don't forget to set owner! */
     CSM_CUR_FRAME(t)->owner = d;

    /* Don't forget to set stream! */
    csm_stream_init(
            CSM_CUR_STREAM(t),
            f->insbytes,
            f->insbytec,
            CSM_STREAM_MODE_LE
    );

    CSM_DECODE_RETURN(t, hds);
}


/* TODO: Way to bridge back into bytecode methods during native calls? */
/* TODO: "local_start" / "stream" / "saved_datastack_pos" are unset? */
static struct csm_unpacked_op
cbridge_native_(uint32_t idx, struct csm_descriptor d, struct csm_thread* t)
{
    csm_native_handler native;

    /* TODO: Adjust this for natives? */
    CSM_CUR_FRAME(t)--;

    /* Push the string constant index of the function name! */
    if (CSM_CUR_FRAME(t) < t->callstack_bot) {
        struct csm_cell bad = { .as.u32 = idx };
        CSM_DATASTACK_PUSH(t, bad);
        CSM_RETURN_CUSTOM(t, csm_special_callstack_overflow);
    }

    CSM_CUR_FRAME(t)->owner = d;

    native = d.as.native_method->handler;

    /* Execute the native method. */
    native(t);

    /* Restore prior state! */
    CSM_CUR_FRAME(t)++;

    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_call(struct csm_thread* t)
{
    struct csm_cell val;
    struct csm_descriptor d;
    uint32_t idx = 0;

    idx = CSM_CUR_STREAM_U32(t);

    /* TODO: Make all bridges/runtime hooks parameterized! */
    d = csm_method_bridge_snail(idx, t);

    switch (d.what) {
    case CSM_DESCRIPTOR_UNRESOLVED:
        val.as.u32 = idx;
        CSM_DATASTACK_PUSH(t, val);
        CSM_RETURN_CUSTOM(t, csm_special_no_method);
        break;
    case CSM_DESCRIPTOR_BC_METHOD:
        return cbridge_bc_(idx, d, t);
        break;
    case CSM_DESCRIPTOR_NATIVE_METHOD:
        return cbridge_native_(idx, d, t);
        break;
    }

    CSM_RETURN_CUSTOM(t, csm_special_terminate_unexpected);
}


static struct csm_unpacked_op pop_callstack_frame(struct csm_thread* t)
{
    CSM_CUR_FRAME(t)++;

    /* Trying out the custom handler instead, who knows? */
    if (CSM_CUR_FRAME(t) == t->callstack_top) {
        CSM_RETURN_CUSTOM(t, csm_special_eox);
    }

    CSM_DATASTACK_POS(t) = CSM_CUR_FRAME(t)->saved_datastack_pos;
    CSM_DECODE_RETURN(t, hds);
}


static struct csm_unpacked_op op_ret(struct csm_thread* t)
{
    struct csm_unpacked_op result;
    struct csm_cell v;

    v = CSM_DATASTACK_POP(t);
    result = pop_callstack_frame(t);
    CSM_DATASTACK_PUSH(t, v);
    return result;
}


/**
 * The general algorithm for the return instruction handler is as follows:
 *
 * 1. Move the current frame up one.
 * 2. If the new frame address is above top of callstack, terminate.
 * 3. Restore thread's "datastack_pos" using "saved_datastack_pos".
 * 4. Decode next op.
 *
 */
static struct csm_unpacked_op op_leave(struct csm_thread* t)
{
    return pop_callstack_frame(t);
}


/* TODO: Figure out how we'd hook up a debugger. */
static struct csm_unpacked_op op_break(struct csm_thread* t)
{
    assert("Break instruction currently unsupported." == 0);
    CSM_ALIGN_DECODE_RETURN(t, t->last_op, hds);
}


/* TODO: Figure out how exception tables are stored in bytecode. */
static struct csm_unpacked_op op_throw(struct csm_thread* t)
{
    assert("Throw instruction currently unsupported." == 0);
    CSM_ALIGN_DECODE_RETURN(t, t->last_op, hds);
}


csm_handler csm_handlers_unsafe[] = {

    op_nop,
    op_ldl,
    op_stl,
    op_ldg,
    op_stg,
    op_lfd,
    op_sfd,
    op_ldsc,
    op_pop,
    op_swp,
    op_dup,
    op_psh_i8,
    op_psh_i16,
    op_psh_i32,
    op_psh_i64,
    op_psh_f64,
    op_psh_obj,
    op_psh_nil,
    op_par_i8,
    op_par_i16,
    op_par_i32,
    op_par_i64,
    op_par_f64,
    op_par_obj,
    op_lai,
    op_sai,
    op_alen,
    op_and,
    op_or,
    op_xor,
    op_not,
    op_shl,
    op_shr,
    op_add_i64,
    op_sub_i64,
    op_mul_i64,
    op_div_i64,
    op_mod_i64,
    op_neg_i64,
    op_add_f64,
    op_sub_f64,
    op_mul_f64,
    op_div_f64,
    op_mod_f64,
    op_neg_f64,
    op_cst_i64_f64,
    op_cst_f64_i64,
    op_cmp_i64,
    op_cmp_f64,
    op_refcmp,
    op_jmp_eqz,
    op_jmp_nez,
    op_jmp_ltz,
    op_jmp_lez,
    op_jmp_gtz,
    op_jmp_gez,
    op_jmp,
    op_typeof,
    op_call,
    op_ret,
    op_leave,
    op_break,
    op_throw

};