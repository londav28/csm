#include "csm/bytecode/postinit.h"
#include "csm/bytecode/format.h"
#include "csm/errorcodes.h"

#define ATTEMPT_AND_JUMP_ON_ERROR(method__, arg__, err__, end__)            \
    do {                                                                    \
        err__ = method__(arg__);                                            \
        if (err__) {                                                        \
            goto end__;                                                     \
        }                                                                   \
    } while (0)

#define CSM_ARG_COUNT_MAX 255
#define CSM_ARG_SEP_CHAR '/'
#define CSM_RET_SEP_CHAR ':'

static int method_post_paramc(csm_bc_method* m);
static int method_post_params(csm_bc_method* m);
static int method_post_rtype(csm_bc_method* m);
static int method_post_insc(csm_bc_method* m);
static int method_post_name(csm_bc_method* m);

static int method_post_paramc(csm_bc_method* m) {

    /* TODO: Perhaps make defines for these? */
    struct csm_bc_string argstr;
    csm_u32 i = 0;
    csm_u32 argcount = 1;


    /* Error if marked as zero-args and sigblock index is not zero. */
    if (m->is_zero_arg && m->sigblock) {
        return CSM_ERR_LOGICAL;
    }

    /* Nothing to do if marked as zero-args. */
    if (m->is_zero_arg) {
        m->post_paramc = 0;
        return 0;
    }

    /* Error if invalid string index. */
    if (csm_bc_get_string(&argstr, m->module, m->sigblock)) {
        return CSM_ERR_LOGICAL;
    }

    for (i = 0; i < argstr.length; i++) {
        char cursor = argstr.data[i];
        int is_last_sep = 0;

        /* Error if we start with an argument separator. */
        if (i == 0 && cursor == CSM_ARG_SEP_CHAR) {
            return CSM_ERR_LOGICAL;
        }

        /* Error if we hit two separators in a row. */
        if (is_last_sep && cursor == CSM_ARG_SEP_CHAR) {
            return CSM_ERR_LOGICAL;
        }

        if (is_last_sep && cursor != CSM_ARG_SEP_CHAR) {
            is_last_sep = CSM_FALSE;
            argcount++;
            continue;
        }

        if (cursor == CSM_ARG_SEP_CHAR) {
            is_last_sep = CSM_TRUE;
        }
    }

    /* Error if we've exceeded the max argument count. */
    if (argcount >= CSM_ARG_COUNT_MAX) {
        return CSM_ERR_LOGICAL;
    }

    /* Write out param count as a side effect. */
    m->post_paramc = argcount;

    return CSM_ERR_NONE; 
}

static int method_post_params(csm_bc_method* m)
{
    struct csm_bc_tlabel *labels = NULL;
    (void) labels;
    (void) m;
    return CSM_ERR_NONE;
}

static int method_post_rtype(csm_bc_method* m)
{
    (void) m;
    return CSM_ERR_NONE;
}

static int method_post_insc(csm_bc_method* m)
{
    (void) m;
    return CSM_ERR_NONE;
}

static int method_post_name(csm_bc_method* m)
{
    (void) m;
    return CSM_ERR_NONE;
}

int csm_bc_module_postinit(csm_bc_module* m)
{
    csm_bc_method *method = NULL;
    csm_bc_object *object = NULL;
    int err = CSM_ERR_NONE;
    csm_u64 i = 0;

    /* Loop and post-initialize methods. */
    for (i = 0; i < m->methodc; i++) {
        method = m->methods + i;
        err = csm_bc_method_postinit(method);
        if (err) {
            return err;
        }
    }

    /* Loop and post-initialize objects. */
    for (i = 0; i < m->objectc; i++) {
        object = m->objects + i;
        err = csm_bc_object_postinit(object);
        if (err) {
            return err;
        }
    }

    return err;
}

int csm_bc_method_postinit(csm_bc_method* m)
{
    int err = CSM_ERR_NONE;

    ATTEMPT_AND_JUMP_ON_ERROR(method_post_paramc, m, err, _end);
    ATTEMPT_AND_JUMP_ON_ERROR(method_post_params, m, err, _end);
    ATTEMPT_AND_JUMP_ON_ERROR(method_post_rtype, m, err, _end);
    ATTEMPT_AND_JUMP_ON_ERROR(method_post_insc, m, err, _end);
    ATTEMPT_AND_JUMP_ON_ERROR(method_post_name, m, err, _end);

    /* Set the post flag. */
    m->is_post = 1;

_end:
    return err;
}

int csm_bc_object_postinit(csm_bc_object* o)
{
    (void) o;
    return CSM_ERR_NONE;
}

