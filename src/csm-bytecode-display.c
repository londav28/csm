#include "csm/bytecode/format.h"
#include "csm/stream.h"
#include "csm/bytecode/opcodes.h"
#include <stdio.h>
#include <inttypes.h>
#include <string.h>


typedef struct csm_bc_module module;
typedef struct csm_bc_method method;
typedef struct csm_bc_object object;
typedef struct csm_bc_string string;


const char* tb_ =   "  ";


/* TODO: This version is not thread safe. */
int
csm_op_mnemonic_max_length(void)
{
    const char* loop = NULL;
    static int set = 0;
    static int max = 0;
    int count = 0;

    if (set) { return max; }

    do {
        loop = csm_op_mnemonic(count++);
        if (loop == NULL) { break; }
        int check = strlen(loop);
        if (check > max) { max = check; }
    } while (loop);

    return max;
}


static void print_spaces(int amount)
{
    int i = 0;
    for (i = 0; i < amount; i++) {
        putc(' ', stdout);
    }
}


static string* get_string(module* m, uint32_t idx)
{
    if (idx >= m->strc) { return NULL; }
    return m->strs + idx;
}


static void display_string_constant(module* m, uint32_t idx)
{
    string* str = get_string(m, idx);

    if (str) {
        /* May want to calculate the cutoff instead? */
        if (str->length < 64) {
            printf("%s", str->data);
        } else {
            printf("<%" PRIu32 ">", idx);
        }
        return;
    }

    printf("bad-index: %" PRIu32, idx);
    return;
}

/* TODO: Parameterize this so we can dump to any file? */
static void display_instruction(
            uint32_t ofs,
            module* m,
            uint8_t op,
            struct csm_stream* s
){
    uint8_t width = 0;
    int padding = 0;
    int len = 0;
    int maxlen = 0;
    const char* mne = NULL;

    /* Fetch instruction mnemonic. */
    mne = csm_op_mnemonic(op);

    /* Display instruction mnemonic. */
    printf("%s0x%04" PRIx32 "    0x%02" PRIx8 ":  %s", tb_, ofs, op, mne);

    /* Ensure argument of correct width follows. */
    width = csm_op_imd_width(op);

    if (width == 0) {
        printf("\n");
        return;
    }

    if (csm_stream_lt(s, width)) {
        printf("EOS decoding immediate of width %" PRIx8 "\n", width);
        return;
    }

    /* Align mnemonics with spaces % 4. */
    len = strlen(mne);

    maxlen = csm_op_mnemonic_max_length();

    padding = ((maxlen % 4)) + (maxlen - len);
    print_spaces(padding);

    /* Big ugly block to print out immediates appropriately. */
    /* TODO: Convert this to a switch case? */
    if (csm_op_imd_u8(op)) {
        printf("%" PRIu8, csm_stream_u8(s));
    } else if (csm_op_imd_u16(op)) {
        printf("%" PRIu16, csm_stream_u16(s));
    } else if (csm_op_imd_u32(op)) {
        uint32_t idx = csm_stream_u32(s);
        if (csm_op_is_jump(op)) {
            printf("0x%" PRIx32, idx);
        } else {
            display_string_constant(m, idx);
        }
    } else if (csm_op_imd_u64(op)) {
        printf("%" PRIu64, csm_stream_u64(s));
    } else if (csm_op_imd_i8(op)) {
        printf("%" PRId8, csm_stream_i8(s));
    } else if (csm_op_imd_i16(op)) {
        printf("%" PRId16, csm_stream_i16(s));
    } else if (csm_op_imd_i32(op)) {
        printf("%" PRId32, csm_stream_i32(s));
    } else if (csm_op_imd_i64(op)) {
        printf("%" PRId64, csm_stream_i64(s));
    } else if (csm_op_imd_f32(op)) {
        printf("%f", csm_stream_f32(s));
    } else if (csm_op_imd_f64(op)) {
        printf("%f", csm_stream_f64(s));
    }

    /* Terminating newline then flush the stream. */
    fputc(0x0A, stdout);
    fflush(stdout);

    return;
}


static void display_instruction_stream(module *m, method* f)
{
    struct csm_stream* s = NULL;
    struct csm_stream stream;
    uint8_t op = 0;
    uint32_t offset = 0;

    (void) m;

    offset = 0;
    s = &stream;
    csm_stream_init(&stream, f->insbytes, f->insbytec, CSM_STREAM_MODE_LE);

    while (offset < f->insbytec) {
        if (csm_stream_lt(s, 1)) {
            printf("EOS decoding instruction, offset %" PRIu32, offset);
            return;
        }

        op = csm_stream_u8(s);

        display_instruction(offset, m, op, s);

        offset++;
        offset += csm_op_imd_width(op);
    }

    return;
}


static void
display_etes(module* m, method* f)
{
    uint32_t i = 0;

    for (i = 0; i < f->etec; i++) {
        struct csm_bc_ete* ete = f->etes + i;

        printf("-- Displaying exception table entry %" PRIu32 "\n", i);
        printf("  > Type: ");
        display_string_constant(m, ete->type);
        putc('\n', stdout);
        printf("  > Start: 0x%04" PRIu32 "\n", ete->start);
        printf("  > End: 0x%04" PRIu32 "\n", ete->end);
        printf("  > Target: 0x%04" PRIu32 "\n", ete->target);
    }

    return;
}


static void display_method(uint32_t* n, module* m, method* f)
{
    printf("-- Displaying method %" PRIu32 " --\n", *n);

    printf("-- Symbol: ");
    display_string_constant(m, f->name);
    printf("\n");

    printf("-- Debug symbol number: %" PRIu32 "\n", f->debugsymbol);

    printf("-- Signature block: ");
    display_string_constant(m, f->sigblock);
    printf("\n");

    printf("-- Stack limit: %" PRIu8 "\n", f->limstack);
    printf("-- Local limit: %" PRIu8 "\n", f->limlocal);

    printf("-- Instruction stream (%" PRIu32 " bytes):\n", f->insbytec);
    display_instruction_stream(m, f);

    if (f->etec == 0) { return; }

    printf("-- Exception table entries (%" PRIu32 "):\n", f->etec);
    display_etes(m, f);

}


static void display_object(uint32_t* n, module* m, object* o)
{
    printf("-- Displaying object %" PRIu32 " --\n", *n);

    printf("-- Symbol: ");
    display_string_constant(m, o->name);
    printf("\n");

    printf( "-- Field block: ");
    display_string_constant(m, o->fieldblock);
    printf("\n");
}


void csm_bc_display(struct csm_bc_module* m)
{
    uint32_t i = 0;

    printf("-- File size is: %" PRIu64 "\n", m->bufsize);

    printf("-- Methods: %" PRIu32 "\n", m->methodc);
    printf("-- Objects: %" PRIu32 "\n", m->objectc);
    printf("-- Strings: %" PRIu32 "\n", m->strc);
    printf("\n");

    for (i = 0; i < m->methodc; i++) {
        display_method(&i, m, m->methods + i);
        printf("\n");
    }

    for (i = 0; i < m->objectc; i++) {
        display_object(&i, m, m->objects + i);
        printf("\n");
    }

    printf("%s\n", "-- Displaying strings --");

    for (i = 0; i < m->strc; i++) {
        printf("%s%" PRIu32 ": ", tb_, i);
        printf("%s", m->strs[i].data);
        printf("\n");
    }
}