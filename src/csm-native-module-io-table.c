#include "csm/native/module/io.h"
#include "csm/machine/state.h"
#include "csm/bytecode/format.h"
#include "csm/types.h"
#include <stdlib.h>
#include <stdio.h>

static csm_native_method nm__io_flush = {
    .name               = "csm:io_flush",
    .parameter_count    = 0,
    .parameter_str      = NULL,
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static csm_native_method nm__io_print_strconst = {
    .name               = "csm:io_print_strconst",
    .parameter_count    = 1,
    .parameter_str      = "X",
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static csm_native_method nm__io_print_b = {
    .name               = "csm:io_print_b",
    .parameter_count    = 1,
    .parameter_str      = "B",
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static csm_native_method nm__io_print_s = {
    .name               = "csm:io_print_s",
    .parameter_count    = 1,
    .parameter_str      = "S",
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static csm_native_method nm__io_print_d = {
    .name               = "csm:io_print_d",
    .parameter_count    = 1,
    .parameter_str      = "D",
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static csm_native_method nm__io_print_q = {
    .name               = "csm:io_print_q",
    .parameter_count    = 1,
    .parameter_str      = "Q",
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static csm_native_method nm__io_print_f = {
    .name               = "csm:io_print_f",
    .parameter_count    = 1,
    .parameter_str      = "F",
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static csm_native_method nm__io_print_a = {
    .name               = "csm:io_print_a",
    .parameter_count    = 1,
    .parameter_str      = "A",
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static csm_native_method nm__io_print_b_ascii = {
    .name               = "csm:io_print_b_ascii",
    .parameter_count    = 1,
    .parameter_str      = "B",
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static csm_native_method nm__io_print_d_utf8 = {
    .name               = "csm:io_print_d_utf8",
    .parameter_count    = 1,
    .parameter_str      = "D",
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static csm_native_method nm__io_print_arr_b_ascii = {
    .name               = "csm:io_print_arr_b_ascii",
    .parameter_count    = 1,
    .parameter_str      = "*B",
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

static csm_native_method nm__io_print_arr_b_utf8 = {
    .name               = "csm:io_print_arr_b_utf8",
    .parameter_count    = 1,
    .parameter_str      = "*B",
    .is_void            = CSM_TRUE,
    .rtype_str          = NULL,
    .throws_exception   = CSM_FALSE,
    .handler            = NULL
};

/* Sadly we can't avoid this! Invoke sometime during setup. */
void csm_native_io_setup(void)
{
    nm__io_flush.handler = csm_native_io_flush;
    nm__io_print_strconst.handler = csm_native_io_print_strconst;
    nm__io_print_b.handler = csm_native_io_print_b;
    nm__io_print_s.handler = csm_native_io_print_s;
    nm__io_print_d.handler = csm_native_io_print_d;
    nm__io_print_q.handler = csm_native_io_print_q;
    nm__io_print_f.handler = csm_native_io_print_f;
    nm__io_print_a.handler = csm_native_io_print_a;
    nm__io_print_b_ascii.handler = csm_native_io_print_b_ascii;
    nm__io_print_d_utf8.handler = csm_native_io_print_d_utf8;
    nm__io_print_arr_b_ascii.handler = csm_native_io_print_arr_b_ascii;
    nm__io_print_arr_b_utf8.handler = csm_native_io_print_arr_b_utf8;

    return;
}

void csm_native_io_teardown(void)
{
    return;
}

/* TODO: Refactor to use libraries later? */
csm_native_method *csm_native_io_methods[] = {

    &nm__io_flush,
    &nm__io_print_strconst,
    &nm__io_print_b,
    &nm__io_print_s,
    &nm__io_print_d,
    &nm__io_print_q,
    &nm__io_print_f,
    &nm__io_print_a,
    &nm__io_print_b_ascii,
    &nm__io_print_d_utf8,
    &nm__io_print_arr_b_ascii,
    &nm__io_print_arr_b_utf8,
    NULL

};

csm_native_object *csm_native_io_objects[] = {

    NULL

};

