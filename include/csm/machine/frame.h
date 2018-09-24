#ifndef CSM_MACHINE_FRAME_H_
#define CSM_MACHINE_FRAME_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "csm/bytecode/opcodes.h"
#include <assert.h>


/**
 * BIG TODO:
 * 
 * We need to provide separate datastack manip macros for when you are using
 * typed cells vs untyped.
 * 
 * We'll also need a separate set of stack frames (say "typed/untyped"),
 * either that or we can union them? Not sure.
 * 
 * IE, any of the dynamic handler sets make use of the typed datastack, but
 * the unsafe stack makes use of stack frames instead (absolutely no type
 * information is preserved).
 * 
 */


/* Pointer to current top of stack. */
#define CSM_DATASTACK_POS(t) (t->datastack_pos)

/* Pointer to the current stack frame. */
#define CSM_CUR_FRAME(t) (t->callstack_pos)

/* Pointer to the stream of the current stack frame. */
#define CSM_CUR_STREAM(t) (&(CSM_CUR_FRAME(t)->stream))

/* Pointer to the start of locals for the current stack frame. */
#define CSM_CUR_FRAME_LOCALS(t) (CSM_CUR_FRAME(t)->local_start)

/* Pointer to the start of stack for the current stack frame. */
#define CSM_CUR_FRAME_SP(t) (CSM_CUR_FRAME(t)->sp)

/* Interpret the current frame's owner as a bytecode method descriptor. */
#define CSM_CUR_METHOD_BYTECODE(t) (CSM_CUR_FRAME(t)->owner.as.bytecode)

/* Interpret the current frame's owner as a native method descriptor. */
#define CSM_CUR_METHOD_NATIVE(t) (CSM_CUR_FRAME(t)->owner.as.native)

/* Interpret frame owner as bytecode and get stack limit. */
#define CSM_CUR_STACK_LIMIT(t) (CSM_CUR_METHOD_BYTECODE(t)->stack_limit)

/* Interpret frame owner as bytecode and get local limit. */
#define CSM_CUR_LOCAL_LIMIT(t) (CSM_CUR_METHOD_BYTECODE(t)->local_limit)

/* Push a value onto the stack via the global stack pointer. */
#define CSM_DATASTACK_PUSH(t, value) (*(--CSM_DATASTACK_POS(t)) = value)

/* Pop a value off the stack via the global stack pointer. */
#define CSM_DATASTACK_POP(t) (*CSM_DATASTACK_POS(t)++)

/* Get machine for the current session. */
#define CSM_MACHINE(t) (t->machine)

/* Get global array for the current session. */
#define CSM_GLOBALS(t) (CSM_MACHINE(t)->globals)

/* Decode and return the next op. */
#define CSM_DECODE_RETURN(t, handlers)                                      \
    do {                                                                    \
        struct csm_unpacked_op result;                                      \
        result.op = CSM_CUR_STREAM_U8(t);                                   \
        result.handler = handlers[result.op];                               \
        return result;                                                      \
    } while (0)

/* Properly align the stream, then decode and return the next op. */
#define CSM_ALIGN_DECODE_RETURN(t, op, handlers)                            \
    do {                                                                    \
        switch(csm_op_imd_width(op)) {                                      \
        case CSM_OP_IMD_WIDTH_1:                                            \
            CSM_CUR_STREAM_U8(t);                                           \
            break;                                                          \
        case CSM_OP_IMD_WIDTH_2:                                            \
            CSM_CUR_STREAM_U16(t);                                          \
            break;                                                          \
        case CSM_OP_IMD_WIDTH_4:                                            \
            CSM_CUR_STREAM_U32(t);                                          \
            break;                                                          \
        case CSM_OP_IMD_WIDTH_8:                                            \
            CSM_CUR_STREAM_U64(t);                                          \
            break;                                                          \
        default:                                                            \
            assert("Should never reach here!" == 0);                        \
            break;                                                          \
        }                                                                   \
        CSM_DECODE_RETURN(t, handlers);                                     \
    } while (0)

/* Return a custom handler with value 0. */
#define CSM_RETURN_CUSTOM(t, which)                                         \
    do {                                                                    \
        struct csm_unpacked_op result;                                      \
        result.op = 0;                                                      \
        result.handler = &which;                                            \
        return result;                                                      \
    } while (0)

#define CSM_CUR_STREAM_U8(t) csm_stream_u8(CSM_CUR_STREAM(t))

#define CSM_CUR_STREAM_U16(t) csm_stream_u16(CSM_CUR_STREAM(t))

#define CSM_CUR_STREAM_U32(t) csm_stream_u32(CSM_CUR_STREAM(t))

#define CSM_CUR_STREAM_U64(t) csm_stream_u64(CSM_CUR_STREAM(t))

#define CSM_CUR_STREAM_I8(t) csm_stream_i8(CSM_CUR_STREAM(t))

#define CSM_CUR_STREAM_I16(t) csm_stream_i16(CSM_CUR_STREAM(t))

#define CSM_CUR_STREAM_I32(t) csm_stream_i32(CSM_CUR_STREAM(t))

#define CSM_CUR_STREAM_I64(t) csm_stream_i64(CSM_CUR_STREAM(t))

#define CSM_CUR_STREAM_F32(t) csm_stream_f32(CSM_CUR_STREAM(t))

#define CSM_CUR_STREAM_F64(t) csm_stream_f64(CSM_CUR_STREAM(t))


#ifdef __cplusplus
}
#endif

#endif