#ifndef CSM_INCLUDE_CSM_BYTECODE_OPCODES_H_
#define CSM_INCLUDE_CSM_BYTECODE_OPCODES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"

typedef enum {

    CSM_OP_NOP = 0,
    CSM_OP_LDL,
    CSM_OP_STL,
    CSM_OP_LDG,
    CSM_OP_STG,
    CSM_OP_LFD,
    CSM_OP_SFD,
    CSM_OP_LDSC,
    CSM_OP_POP,
    CSM_OP_SWP,
    CSM_OP_DUP,
    CSM_OP_PSH_B,
    CSM_OP_PSH_S,
    CSM_OP_PSH_D,
    CSM_OP_PSH_Q,
    CSM_OP_PSH_F,
    CSM_OP_PSH_A,
    CSM_OP_PSH_NIL,
    CSM_OP_PAR_B,
    CSM_OP_PAR_S,
    CSM_OP_PAR_D,
    CSM_OP_PAR_Q,
    CSM_OP_PAR_F,
    CSM_OP_PAR_A,
    CSM_OP_LAI,
    CSM_OP_SAI,
    CSM_OP_ALEN,
    CSM_OP_AND,
    CSM_OP_OR,
    CSM_OP_XOR,
    CSM_OP_NOT,
    CSM_OP_SHL,
    CSM_OP_SHR,
    CSM_OP_ADD_Q,
    CSM_OP_SUB_Q,
    CSM_OP_MUL_Q,
    CSM_OP_DIV_Q,
    CSM_OP_MOD_Q,
    CSM_OP_NEG_Q,
    CSM_OP_ADD_F,
    CSM_OP_SUB_F,
    CSM_OP_MUL_F,
    CSM_OP_DIV_F,
    CSM_OP_MOD_F,
    CSM_OP_NEG_F,
    CSM_OP_CST_QF,
    CSM_OP_CST_FQ,
    CSM_OP_CMP_Q,
    CSM_OP_CMP_F,
    CSM_OP_REFCMP,
    CSM_OP_JMP_EQZ,
    CSM_OP_JMP_NEZ,
    CSM_OP_JMP_LTZ,
    CSM_OP_JMP_LEZ,
    CSM_OP_JMP_GTZ,
    CSM_OP_JMP_GEZ,
    CSM_OP_JMP,
    CSM_OP_TYPEOF,
    CSM_OP_CALL,
    CSM_OP_RET,
    CSM_OP_LEAVE,
    CSM_OP_BREAK,
    CSM_OP_THROW

};

/* Possible widths of instruction immediates (in bytes). */
#define CSM_OP_IMD_WIDTH_0          0
#define CSM_OP_IMD_WIDTH_1          1
#define CSM_OP_IMD_WIDTH_2          2
#define CSM_OP_IMD_WIDTH_4          4
#define CSM_OP_IMD_WIDTH_8          8

const char *csm_op_mnemonic(csm_u8 op);

int csm_op_is_jump(csm_u8 op);

int csm_op_imd_u8(csm_u8 op);

int csm_op_imd_u16(csm_u8 op);

int csm_op_imd_u32(csm_u8 op);

int csm_op_imd_u64(csm_u8 op);

int csm_op_imd_i8(csm_u8 op);

int csm_op_imd_i16(csm_u8 op);

int csm_op_imd_i32(csm_u8 op);

int csm_op_imd_i64(csm_u8 op);

int csm_op_imd_f32(csm_u8 op);

int csm_op_imd_f64(csm_u8 op);

csm_u8 csm_op_imd_width(csm_u8 op);

#ifdef __cplusplus
}
#endif
#endif