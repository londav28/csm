#include "csm/bytecode/opcodes.h"
#include <string.h>


const char* csm_op_mnemonic_table[] = {

    "nop",
    "ldl",
    "stl",
    "ldg",
    "stg",
    "lfd",
    "sfd",
    "ldsc",
    "pop",
    "swp",
    "dup",
    "psh_b",
    "psh_s",
    "psh_d",
    "psh_q",
    "psh_f",
    "psh_a",
    "psh_nil",
    "par_b",
    "par_s",
    "par_d",
    "par_q",
    "par_f",
    "par_a",
    "lai",
    "sai",
    "alen",
    "and",
    "or",
    "xor",
    "not",
    "shl",
    "shr",
    "add_q",
    "sub_q",
    "mul_q",
    "div_q",
    "mod_q",
    "neg_q",
    "add_f",
    "sub_f",
    "mul_f",
    "div_f",
    "mod_f",
    "neg_f",
    "cst_qf",
    "cst_fq",
    "cmp_q",
    "cmp_f",
    "refcmp",
    "jmp_eqz",
    "jmp_nez",
    "jmp_ltz",
    "jmp_lez",
    "jmp_gtz",
    "jmp_gez",
    "jmp",
    "typeof",
    "call",
    "ret",
    "leave",
    "break",
    "throw",
    NULL

};


uint8_t csm_op_imd_width_table[] = {

    CSM_OP_IMD_WIDTH_0,     /* nop      */
    CSM_OP_IMD_WIDTH_1,     /* ldl      */
    CSM_OP_IMD_WIDTH_1,     /* stl      */
    CSM_OP_IMD_WIDTH_2,     /* ldg      */
    CSM_OP_IMD_WIDTH_2,     /* stg      */
    CSM_OP_IMD_WIDTH_2,     /* lfd      */
    CSM_OP_IMD_WIDTH_2,     /* sfd      */
    CSM_OP_IMD_WIDTH_4,     /* ldsc     */
    CSM_OP_IMD_WIDTH_0,     /* pop      */
    CSM_OP_IMD_WIDTH_0,     /* swp      */
    CSM_OP_IMD_WIDTH_0,     /* dup      */
    CSM_OP_IMD_WIDTH_1,     /* psh_b    */
    CSM_OP_IMD_WIDTH_2,     /* psh_s    */
    CSM_OP_IMD_WIDTH_4,     /* psh_d    */
    CSM_OP_IMD_WIDTH_8,     /* psh_q    */
    CSM_OP_IMD_WIDTH_8,     /* psh_f    */
    CSM_OP_IMD_WIDTH_4,     /* psh_a    */
    CSM_OP_IMD_WIDTH_0,     /* psh_nil  */
    CSM_OP_IMD_WIDTH_0,     /* par_b    */
    CSM_OP_IMD_WIDTH_0,     /* par_s    */
    CSM_OP_IMD_WIDTH_0,     /* par_d    */
    CSM_OP_IMD_WIDTH_0,     /* par_q    */
    CSM_OP_IMD_WIDTH_0,     /* par_f    */
    CSM_OP_IMD_WIDTH_4,     /* par_a    */
    CSM_OP_IMD_WIDTH_0,     /* lai      */
    CSM_OP_IMD_WIDTH_0,     /* sai      */
    CSM_OP_IMD_WIDTH_0,     /* alen     */
    CSM_OP_IMD_WIDTH_0,     /* and      */
    CSM_OP_IMD_WIDTH_0,     /* or       */
    CSM_OP_IMD_WIDTH_0,     /* xor      */
    CSM_OP_IMD_WIDTH_0,     /* not      */
    CSM_OP_IMD_WIDTH_0,     /* shl      */
    CSM_OP_IMD_WIDTH_0,     /* shr      */
    CSM_OP_IMD_WIDTH_0,     /* add_q    */
    CSM_OP_IMD_WIDTH_0,     /* sub_q    */
    CSM_OP_IMD_WIDTH_0,     /* mul_q    */
    CSM_OP_IMD_WIDTH_0,     /* div_q    */
    CSM_OP_IMD_WIDTH_0,     /* mod_q    */
    CSM_OP_IMD_WIDTH_0,     /* neg_q    */
    CSM_OP_IMD_WIDTH_0,     /* add_f    */
    CSM_OP_IMD_WIDTH_0,     /* sub_f    */
    CSM_OP_IMD_WIDTH_0,     /* mul_f    */
    CSM_OP_IMD_WIDTH_0,     /* div_f    */
    CSM_OP_IMD_WIDTH_0,     /* mod_f    */
    CSM_OP_IMD_WIDTH_0,     /* neg_f    */
    CSM_OP_IMD_WIDTH_0,     /* cst_qf   */
    CSM_OP_IMD_WIDTH_0,     /* cst_fq   */
    CSM_OP_IMD_WIDTH_0,     /* cmp_q    */
    CSM_OP_IMD_WIDTH_0,     /* cmp_f    */
    CSM_OP_IMD_WIDTH_0,     /* refcmp   */
    CSM_OP_IMD_WIDTH_4,     /* jmp_eqz  */
    CSM_OP_IMD_WIDTH_4,     /* jmp_nez  */
    CSM_OP_IMD_WIDTH_4,     /* jmp_ltz  */
    CSM_OP_IMD_WIDTH_4,     /* jmp_lez  */
    CSM_OP_IMD_WIDTH_4,     /* jmp_gtz  */
    CSM_OP_IMD_WIDTH_4,     /* jmp_gez  */
    CSM_OP_IMD_WIDTH_4,     /* jmp      */
    CSM_OP_IMD_WIDTH_0,     /* typeof   */
    CSM_OP_IMD_WIDTH_4,     /* call     */
    CSM_OP_IMD_WIDTH_0,     /* ret      */
    CSM_OP_IMD_WIDTH_0,     /* leave    */
    CSM_OP_IMD_WIDTH_0,     /* break    */
    CSM_OP_IMD_WIDTH_4      /* throw    */

};


const char*
csm_op_mnemonic(uint8_t op) {
    if (op <= CSM_OP_THROW) {
        return csm_op_mnemonic_table[op];
    }
    return NULL;
}


int
csm_op_is_jump(uint8_t op)
{
    return (
        op == CSM_OP_JMP_EQZ ||
        op == CSM_OP_JMP_NEZ ||
        op == CSM_OP_JMP_LTZ ||
        op == CSM_OP_JMP_LEZ ||
        op == CSM_OP_JMP_GTZ ||
        op == CSM_OP_JMP_GEZ ||
        op == CSM_OP_JMP
    );
}


int
csm_op_imd_u8(uint8_t op)
{
    return (op == CSM_OP_LDL || op == CSM_OP_STL);
}


int
csm_op_imd_u16(uint8_t op)
{
    return (
        op == CSM_OP_LDG    ||
        op == CSM_OP_STG    ||
        op == CSM_OP_LFD    ||
        op == CSM_OP_SFD
    );
}


int
csm_op_imd_u32(uint8_t op)
{
    return (
        csm_op_is_jump(op)  ||
        op == CSM_OP_PSH_Q  ||
        op == CSM_OP_PSH_F  ||
        op == CSM_OP_PSH_A  ||
        op == CSM_OP_PAR_A  ||
        op == CSM_OP_LDSC   ||
        op == CSM_OP_CALL
    );
}


int
csm_op_imd_u64(uint8_t op)
{
    (void) op;
    return 0;
}


int
csm_op_imd_i8(uint8_t op)
{
    return (op == CSM_OP_PSH_B);
}


int
csm_op_imd_i16(uint8_t op)
{
    return (op == CSM_OP_PSH_S);
}


int
csm_op_imd_i32(uint8_t op)
{
    return (op == CSM_OP_PSH_D);
}


int
csm_op_imd_i64(uint8_t op)
{
    (void) op;
    return 0;
}


int
csm_op_imd_f32(uint8_t op)
{
    (void) op;
    return 0;
}


int
csm_op_imd_f64(uint8_t op)
{
    (void) op;
    return 0;
}


uint8_t
csm_op_imd_width(uint8_t op) {
    if (op <= CSM_OP_THROW) {
        return csm_op_imd_width_table[op];
    }
    return 0;
}


