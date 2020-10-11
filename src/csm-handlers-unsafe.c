#include "csm/handlers/unsafe.h"
#include "csm/machine/state.h"
#include "csm/machine/frame.h"
#include "csm/bytecode/opcodes.h"
#include "csm/bytecode/format.h"
#include "csm/machine/bridge.h"
#include "csm/memory/gc.h"
#include "csm/handlers/special.h"
#include "csm/memset.h"
#include "csm/types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CSM_BINARY_OP(op, awidth, rwidth, thread)                           \
    do {                                                                    \
        csm_cell a, b, v;                                                   \
        b = CSM_DATASTACK_POP(thread);                                      \
        a = CSM_DATASTACK_POP(thread);                                      \
        v.as.rwidth = (a.as.awidth op b.as.awidth);                         \
        CSM_DATASTACK_PUSH(thread, v);                                      \
    } while (0)

#define CSM_UNARY_OP(op, twidth, fwidth, thread)                            \
    do {                                                                    \
        csm_cell v;                                                         \
        v = CSM_DATASTACK_POP(thread);                                      \
        v.as.twidth = op (v.as.fwidth);                                     \
        CSM_DATASTACK_PUSH(thread, v);                                      \
    } while (0)

#define CSM_THREEWAY_COMPARISON_OP(awidth, thread)                          \
    do {                                                                    \
        csm_cell a, b, v;                                                   \
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
        csm_cell v;                                                         \
        csm_u32 j = 0;                                                      \
        v = CSM_DATASTACK_POP(thread);                                      \
        j = CSM_CUR_STREAM_U32(thread);                                     \
        if (v.as.i64 cmp 0) {                                               \
            csm_stream_shift_abs(CSM_CUR_STREAM(thread), j);                \
        }                                                                   \
    } while (0)

#define CSM_PUSH_OP(awidth, readw, thread)                                  \
    do {                                                                    \
        csm_cell v;                                                         \
        v.as.awidth = readw(thread);                                        \
        CSM_DATASTACK_PUSH(thread, v);                                      \
    } while (0)

/* TODO: Bytes calculation can overflow. */
/* TODO: Load and store exceptions could mention array kind. */
#define CSM_PUSH_ARRAY_OP(ctype, what, thread)                              \
    do {                                                                    \
        csm_array_header *array = NULL;                                     \
        csm_cell v;                                                         \
        csm_i64 length = 0;                                                 \
        size_t stride = 0;                                                  \
        size_t bytes = 0;                                                   \
        length = CSM_DATASTACK_POP(thread).as.i64;                          \
        if (length < 0) {                                                   \
            csm_cell bad;                                                   \
            bad.as.i64 = length;                                            \
            CSM_DATASTACK_PUSH(thread, bad);                                \
            CSM_RETURN_CUSTOM(thread, csm_special_bad_array_size);          \
        }                                                                   \
        stride = sizeof(ctype);                                             \
        /* TODO: Should align this amount later? */                         \
        bytes = (length * stride) + sizeof(*array);                         \
        array = csm_gc_alloc(CSM_GC_TAG_ARRAY, bytes, thread);              \
        if (array == NULL) {                                                \
            csm_cell bad;                                                   \
            bad.as.u64 = bytes;                                             \
            CSM_DATASTACK_PUSH(thread, bad);                                \
            CSM_RETURN_CUSTOM(thread, csm_special_memory_exhaustion);       \
        }                                                                   \
        csm_memset(array, 0, sizeof(*array));                               \
        /* TODO: Special set array descriptor for par_obj. */               \
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
csm_handler *hds = csm_handlers_unsafe;

static csm_unpacked_op op_nop(csm_thread *t)
{
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_ldl(csm_thread *t)
{
    csm_cell value;
    csm_u8 idx = 0;

    idx = CSM_CUR_STREAM_U8(t);
    value = CSM_CUR_FRAME_LOCALS(t)[idx];
    CSM_DATASTACK_PUSH(t, value);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_stl(csm_thread *t)
{
    csm_cell value;
    csm_u8 idx = 0;

    idx = CSM_CUR_STREAM_U8(t);
    value = CSM_DATASTACK_POP(t);
    CSM_CUR_FRAME_LOCALS(t)[idx] = value;
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_ldg(csm_thread *t)
{
    csm_cell value;
    csm_u8 idx = 0;

    idx = CSM_CUR_STREAM_U16(t);
    value = CSM_GLOBALS(t)[idx];
    CSM_DATASTACK_PUSH(t, value);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_stg(csm_thread *t)
{
    csm_cell value;
    csm_u8 idx = 0;

    idx = CSM_CUR_STREAM_U16(t);
    value = CSM_DATASTACK_POP(t);
    CSM_GLOBALS(t)[idx] = value;
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_lfd(csm_thread* t)
{
    csm_object_header *object = NULL;
    csm_cell *cells = NULL;
    csm_cell value;
    csm_u8 idx = 0;

    idx = CSM_CUR_STREAM_U16(t);
    object = CSM_DATASTACK_POP(t).as.raw;
    cells = object->data;
    value = cells[idx];

    CSM_DATASTACK_PUSH(t, value);

    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_sfd(csm_thread *t)
{
    csm_object_header *object = NULL;
    csm_cell *cells = NULL;
    csm_cell value;
    csm_u8 idx = 0;

    idx = CSM_CUR_STREAM_U16(t);
    value = CSM_DATASTACK_POP(t);
    object = CSM_DATASTACK_POP(t).as.raw;

    cells = object->data;
    cells[idx] = value;

    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_ldsc(csm_thread *t)
{
    csm_cell v;

    v.as.u32 = CSM_CUR_STREAM_U32(t);
    CSM_DATASTACK_PUSH(t, v);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_pop(csm_thread* t)
{
    csm_cell unused;

    unused = CSM_DATASTACK_POP(t);
    (void) unused;
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_swp(csm_thread *t)
{
    csm_cell v1, v2;

    v1 = CSM_DATASTACK_POP(t);
    v2 = CSM_DATASTACK_POP(t);
    CSM_DATASTACK_PUSH(t, v1);
    CSM_DATASTACK_PUSH(t, v2);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_dup(csm_thread *t)
{
    csm_cell v;

    v = CSM_DATASTACK_POP(t);
    CSM_DATASTACK_PUSH(t, v);
    CSM_DATASTACK_PUSH(t, v);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_psh_b(csm_thread *t)
{
    CSM_PUSH_OP(i64, CSM_CUR_STREAM_I8, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_psh_s(csm_thread *t)
{
    CSM_PUSH_OP(i64, CSM_CUR_STREAM_I16, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_psh_d(csm_thread *t)
{
    CSM_PUSH_OP(i64, CSM_CUR_STREAM_I32, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_psh_q(csm_thread *t)
{
    CSM_PUSH_OP(i64, CSM_CUR_STREAM_I64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_psh_f(csm_thread *t)
{
    CSM_PUSH_OP(f64, CSM_CUR_STREAM_F64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op
do_object_bytecode(csm_u32 idx, csm_descriptor d, csm_thread *t)
{
    struct csm_bc_object *o = d.as.bc_object;
    csm_object_header *object;
    csm_cell v;
    csm_u64 bytes = 0;

    (void) idx;

    assert(d.what == CSM_DESCRIPTOR_BC_OBJECT);
    assert(o->is_post);

    /* TODO: Overflow check for this? */
    bytes = (sizeof(csm_cell) * o->post_fieldc) + sizeof(*object);

    object = csm_gc_alloc(CSM_GC_TAG_OBJECT_BC, bytes, t);

    if (object == NULL) {
        csm_cell bad;
        bad.as.u64 = bytes;
        CSM_DATASTACK_PUSH(t, bad);
        CSM_RETURN_CUSTOM(t, csm_special_memory_exhaustion);
    }

    object->what = d;
    object->data = (object + 1);

    csm_memset(object->data, 0, (sizeof(csm_cell) * o->post_fieldc));

    v.as.raw = object;

    CSM_DATASTACK_PUSH(t, v);

    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op
do_object_native(csm_u32 idx, csm_descriptor d, csm_thread *t)
{
    struct csm_bc_object* o = d.as.bc_object;
    csm_u64 bytes = 0;
    csm_u32 fieldcount = 0;

    (void) idx;
    (void) d;
    (void) t;
    (void) o;
    (void) bytes;
    (void) fieldcount;

    assert(d.what == CSM_DESCRIPTOR_NATIVE_OBJECT);

    assert("Not implemented yet!" == NULL);

    CSM_DECODE_RETURN(t, hds);
}

/* TODO: Plan logic for object allocation! */
static csm_unpacked_op op_psh_a(csm_thread *t)
{
    csm_cell val;
    csm_descriptor d;
    csm_u32 idx = 0;

    idx = CSM_CUR_STREAM_U32(t);

    /* TODO: Make all bridges/runtime hooks parameterized! */
    d = csm_bridge_snail(CSM_BRIDGE_OBJECT, idx, t);

    switch (d.what) {
    case CSM_DESCRIPTOR_UNRESOLVED:
        val.as.u32 = idx;
        CSM_DATASTACK_PUSH(t, val);
        CSM_RETURN_CUSTOM(t, csm_special_no_object);
        break;
    case CSM_DESCRIPTOR_BC_OBJECT:
        return do_object_bytecode(idx, d, t);
        break;
    case CSM_DESCRIPTOR_NATIVE_OBJECT:
        return do_object_native(idx, d, t);
        break;
    }

    /* Should never reach here... */
    CSM_RETURN_CUSTOM(t, csm_special_terminate_unexpected);
}

static csm_unpacked_op op_psh_nil(csm_thread *t)
{
    csm_cell value;

    value.as.raw = NULL;
    CSM_DATASTACK_PUSH(t, value);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_par_b(csm_thread *t)
{
    CSM_PUSH_ARRAY_OP(csm_i8, CSM_ARRAY_TYPE_I8, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_par_s(csm_thread *t)
{
    CSM_PUSH_ARRAY_OP(csm_i16, CSM_ARRAY_TYPE_I16, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_par_d(csm_thread *t)
{
    CSM_PUSH_ARRAY_OP(csm_i32, CSM_ARRAY_TYPE_I32, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_par_q(csm_thread *t)
{
    CSM_PUSH_ARRAY_OP(csm_i64, CSM_ARRAY_TYPE_I64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_par_f(csm_thread *t)
{
    CSM_PUSH_ARRAY_OP(csm_f64, CSM_ARRAY_TYPE_F64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_par_a(csm_thread *t)
{
    CSM_PUSH_ARRAY_OP(void*, CSM_ARRAY_TYPE_OBJ, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_lai(csm_thread *t)
{
    csm_array_header *hdr = NULL;
    csm_cell val;
    csm_i64 idx = 0;

    idx = CSM_DATASTACK_POP(t).as.i64;
    hdr = CSM_DATASTACK_POP(t).as.raw;

    /* TODO: Throw an exception (or exception handler) instead. */
    if (idx < 0 || idx >= hdr->length) {
        csm_cell bad;
        bad.as.i64 = idx;
        CSM_DATASTACK_PUSH(t, bad);
        CSM_RETURN_CUSTOM(t, csm_special_bad_array_load);
    }

    /* From memcpy to a more blocky switch. Better or worse? */
    switch(hdr->kind) {
    case CSM_ARRAY_TYPE_I8:
        val.as.i64 = ((csm_i8*) hdr->data)[idx];
        break;
    case CSM_ARRAY_TYPE_I16:
        val.as.i64 = ((csm_i16*) hdr->data)[idx];
        break;
    case CSM_ARRAY_TYPE_I32:
        val.as.i64 = ((csm_i32*) hdr->data)[idx];
        break;
    case CSM_ARRAY_TYPE_I64:
        val.as.i64 = ((csm_i64*) hdr->data)[idx];
        break;
    case CSM_ARRAY_TYPE_F64:
        val.as.f64 = ((csm_f64*) hdr->data)[idx];
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

static csm_unpacked_op op_sai(csm_thread *t)
{
    /* Pop items (even if unused) to preserve stack height. */
    csm_array_header *hdr = NULL;
    csm_cell val;
    csm_i64 idx = 0;

    val = CSM_DATASTACK_POP(t);
    idx = CSM_DATASTACK_POP(t).as.i64;
    hdr = CSM_DATASTACK_POP(t).as.raw;

    /* TODO: Throw an exception (or exception handler) instead. */
    if (idx < 0 || idx >= hdr->length) {
        csm_cell bad;
        bad.as.i64 = idx;
        CSM_DATASTACK_PUSH(t, bad);
        CSM_RETURN_CUSTOM(t, csm_special_bad_array_store);
    }

    /* From memcpy to a more blocky switch. Better or worse? */
    switch(hdr->kind) {
    case CSM_ARRAY_TYPE_I8:
        ((csm_i8*) hdr->data)[idx] = val.as.i8;
        break;
    case CSM_ARRAY_TYPE_I16:
        ((csm_i16*) hdr->data)[idx] = val.as.i16;
        break;
    case CSM_ARRAY_TYPE_I32:
        ((csm_i32*) hdr->data)[idx] = val.as.i32;
        break;
    case CSM_ARRAY_TYPE_I64:
        ((csm_i64*) hdr->data)[idx] = val.as.i64;
        break;
    case CSM_ARRAY_TYPE_F64:
        ((csm_f64*) hdr->data)[idx] = val.as.f64;
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

static csm_unpacked_op op_alen(csm_thread *t)
{
    csm_array_header *header = NULL;
    csm_cell v;

    header = CSM_DATASTACK_POP(t).as.raw;
    assert(header != NULL);
    v.as.i64 = header->length;

    CSM_DATASTACK_PUSH(t, v);

    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_and(csm_thread *t)
{
    CSM_BINARY_OP(&, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_or(csm_thread *t)
{
    CSM_BINARY_OP(|, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_xor(csm_thread *t)
{
    CSM_BINARY_OP(^, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_not(csm_thread *t)
{
    CSM_UNARY_OP(~, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_shl(csm_thread *t)
{
    CSM_BINARY_OP(<<, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_shr(csm_thread *t)
{
    CSM_BINARY_OP(>>, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_add_i64(csm_thread *t)
{
    CSM_BINARY_OP(+, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_sub_i64(csm_thread *t)
{
    CSM_BINARY_OP(-, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_mul_i64(csm_thread *t)
{
    CSM_BINARY_OP(*, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_div_i64(csm_thread *t)
{
    CSM_BINARY_OP(/, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_mod_i64(csm_thread *t)
{
    CSM_BINARY_OP(%, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_neg_i64(csm_thread *t)
{
    CSM_UNARY_OP(-, i64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_add_f64(csm_thread *t)
{
    CSM_BINARY_OP(+, f64, f64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_sub_f64(csm_thread *t)
{
    CSM_BINARY_OP(-, f64, f64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_mul_f64(csm_thread *t)
{
    CSM_BINARY_OP(*, f64, f64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_div_f64(csm_thread *t)
{
    CSM_BINARY_OP(/, f64, f64, t);
    CSM_DECODE_RETURN(t, hds);
}

/* TODO: Implement support for floating point modulo. */
static csm_unpacked_op op_mod_f64(csm_thread *t)
{
    assert("Floating point modulo currently unsupported." == 0);
    CSM_ALIGN_DECODE_RETURN(t, t->last_op, hds);
}

static csm_unpacked_op op_neg_f64(csm_thread *t)
{
    CSM_UNARY_OP(-, f64, f64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_cst_qf(csm_thread *t)
{
    CSM_UNARY_OP((csm_f64), f64, i64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_cst_fq(csm_thread *t)
{
    CSM_UNARY_OP((csm_i64), i64, f64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_cmp_q(csm_thread *t)
{
    CSM_THREEWAY_COMPARISON_OP(i64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_cmp_f(csm_thread *t)
{
    CSM_THREEWAY_COMPARISON_OP(f64, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_refcmp(csm_thread *t)
{
    csm_cell a, b, v;

    b = CSM_DATASTACK_POP(t);
    a = CSM_DATASTACK_POP(t);

    v.as.i64 = a.as.raw == b.as.raw ? 1 : 0;

    CSM_DATASTACK_PUSH(t, v);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_jmp_eqz(csm_thread *t)
{
    CSM_COMPARE_ZERO_JUMP_OP(==, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_jmp_nez(csm_thread *t)
{
    CSM_COMPARE_ZERO_JUMP_OP(!=, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_jmp_ltz(csm_thread *t)
{
    CSM_COMPARE_ZERO_JUMP_OP(<, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_jmp_lez(csm_thread *t)
{
    CSM_COMPARE_ZERO_JUMP_OP(<=, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_jmp_gtz(csm_thread *t)
{
    CSM_COMPARE_ZERO_JUMP_OP(>, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_jmp_gez(csm_thread *t)
{
    CSM_COMPARE_ZERO_JUMP_OP(>=, t);
    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_jmp(csm_thread *t)
{
    csm_i32 jump = 0;

    jump = CSM_CUR_STREAM_U32(t);
    csm_stream_shift_abs(CSM_CUR_STREAM(t), jump);
    CSM_DECODE_RETURN(t, hds);
}

/* TODO: Figure out object model/metadata for this. */
static csm_unpacked_op op_typeof(csm_thread *t)
{
    assert("Not implemented yet!" == NULL);
    CSM_ALIGN_DECODE_RETURN(t, t->last_op, hds);
}

static csm_unpacked_op
do_call_bytecode(csm_u32 idx, csm_descriptor d, csm_thread *t)
{
    csm_bc_method *f = d.as.bc_method;
    csm_cell *new_frame_top = NULL;
    csm_u32 i = 0;
    csm_u16 localspace = 0;

    /* Calculate new frame ceiling, is also saved datastack. */
    new_frame_top = CSM_DATASTACK_POS(t) + f->post_paramc;

    /* Save current frame's datastack position. */
    CSM_CUR_FRAME(t)->saved_datastack_pos = new_frame_top;

    /* Save current frame's instruction stream position. */
    CSM_CUR_FRAME(t)->ins_pos = csm_stream_get_pos(CSM_CUR_STREAM(t));

    /* Move frame pointer down. */
    CSM_CUR_FRAME(t)--;

    /*
        If we've run out of callstack space, push the offending function's
        name and return the overflow handler. TODO: What about if we
        run out of datastack space?
    */
    if (CSM_CUR_FRAME(t) < t->callstack_bot) {
        csm_cell bad = {.as.u32 = idx};
        CSM_DATASTACK_PUSH(t, bad);
        CSM_RETURN_CUSTOM(t, csm_special_callstack_overflow);
    }

    /* This value should not exceed 256. */
    localspace = f->limstack + f->limlocal;
    assert(localspace <= 255);

    /* Make space for locals on the stack. */
    CSM_CUR_FRAME_LOCALS(t) = new_frame_top - localspace;

    /* Move parameters into local slots. */
    for (i = 0; i < f->post_paramc; i++) {
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
static csm_unpacked_op
do_call_native(csm_u32 idx, csm_descriptor d, csm_thread *t)
{
    csm_native_handler native;

    /* Move frame pointer down. TODO: Adjust this for natives? */
    CSM_CUR_FRAME(t)--;

    /*
        If we've run out of callstack space, push the offending function's
        name and return the overflow handler. TODO: What about if we
        run out of datastack space?
    */
    if (CSM_CUR_FRAME(t) < t->callstack_bot) {
        csm_cell bad = { .as.u32 = idx };
        CSM_DATASTACK_PUSH(t, bad);
        CSM_RETURN_CUSTOM(t, csm_special_callstack_overflow);
    }

    CSM_CUR_FRAME(t)->owner = d;

    /* Fetch the native method from the descriptor. */
    native = d.as.native_method->handler;

    /* Execute the native method. */
    native(t);

    /* Restore prior state! */
    CSM_CUR_FRAME(t)++;

    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_call(csm_thread *t)
{
    csm_cell val;
    csm_descriptor d;
    csm_u32 idx = 0;

    idx = CSM_CUR_STREAM_U32(t);

    /* TODO: Make all bridges/runtime hooks parameterized! */
    d = csm_bridge_snail(CSM_BRIDGE_METHOD, idx, t);

    switch (d.what) {
    case CSM_DESCRIPTOR_UNRESOLVED:
        val.as.u32 = idx;
        CSM_DATASTACK_PUSH(t, val);
        CSM_RETURN_CUSTOM(t, csm_special_no_method);
        break;
    case CSM_DESCRIPTOR_BC_METHOD:
        return do_call_bytecode(idx, d, t);
        break;
    case CSM_DESCRIPTOR_NATIVE_METHOD:
        return do_call_native(idx, d, t);
        break;
    }

    CSM_RETURN_CUSTOM(t, csm_special_terminate_unexpected);
}

static csm_unpacked_op pop_callstack_frame(csm_thread *t)
{
    csm_bc_method * f = NULL;
    csm_descriptor d;

    /* Restore the previous frame. */
    CSM_CUR_FRAME(t)++;

    /* Fetch the descriptor for the previous frame. */
    d = CSM_CUR_FRAME(t)->owner;

    /* Calling bytecode from native methods is not possible yet. */
    if (d.what != CSM_DESCRIPTOR_BC_METHOD) {
        assert("Unsupported." == NULL);
    }

    /* Read method info from the descriptor. */
    f = d.as.bc_method;

    /* If we're popping the last frame, return the EOX handler. */
    if (CSM_CUR_FRAME(t) == t->callstack_top) {
        CSM_RETURN_CUSTOM(t, csm_special_eox);
    }

    /* Recall the saved datastack position. */
    CSM_DATASTACK_POS(t) = CSM_CUR_FRAME(t)->saved_datastack_pos;

    /* Initialize decoder stream with instruction stream. */
    csm_stream_init(
            CSM_CUR_STREAM(t),
            f->insbytes,
            f->insbytec,
            CSM_STREAM_MODE_LE
    );

    /* Set decoder stream with saved instruction stream position. */
    csm_stream_shift_abs(
            CSM_CUR_STREAM(t),
            CSM_CUR_FRAME(t)->ins_pos
    );

    CSM_DECODE_RETURN(t, hds);
}

static csm_unpacked_op op_ret(csm_thread *t)
{
    csm_unpacked_op result;
    csm_cell v;

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
static csm_unpacked_op op_leave(csm_thread *t)
{
    return pop_callstack_frame(t);
}

/* TODO: Figure out how we'd hook up a debugger. */
static csm_unpacked_op op_break(csm_thread *t)
{
    assert("Break instruction currently unsupported." == 0);
    CSM_ALIGN_DECODE_RETURN(t, t->last_op, hds);
}

/* TODO: Figure out how exception tables are stored in bytecode. */
static csm_unpacked_op op_throw(csm_thread *t)
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
    op_psh_b,
    op_psh_s,
    op_psh_d,
    op_psh_q,
    op_psh_f,
    op_psh_a,
    op_psh_nil,
    op_par_b,
    op_par_s,
    op_par_d,
    op_par_q,
    op_par_f,
    op_par_a,
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
    op_cst_qf,
    op_cst_fq,
    op_cmp_q,
    op_cmp_f,
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

