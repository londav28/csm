#include "csm/memory/malloc.h"
#include "csm/bytecode/format.h"
#include "csm/errorcodes.h"
#include "csm/wstream.h"
#include "csm/memset.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>


/**
 * readf__  = Wide stream read function to use.
 * out__    = Variable to store output.
 * ws__     = Wide stream to use.
 * bytes__  = Bytes value to use for safety check.
 * err__    = Variable to store error value.
 * fail__   = Label to jump to on failure.
 */
#define CSM_CHECKED_WREAD(readf__, out__, ws__, bytes__, err__, fail__)     \
    do {                                                                    \
        if (csm_wstream_lt(ws__, bytes__)) {                                \
            err__ = CSM_ERR_EOS;                                            \
            goto fail__;                                                    \
        }                                                                   \
        out__ = readf__(ws__);                                              \
    } while (0)


#define CSM_CHECKED_WREAD_U8(out__, s__, err__, fail__)                     \
    CSM_CHECKED_WREAD(csm_wstream_u8, out__, s__, 1, err__, fail__)


#define CSM_CHECKED_WREAD_U16(out__, s__,  err__, fail__)                   \
    CSM_CHECKED_WREAD(csm_wstream_u16, out__, s__, 2, err__, fail__)


#define CSM_CHECKED_WREAD_U32(out__, s__,  err__, fail__)                   \
    CSM_CHECKED_WREAD(csm_wstream_u32, out__, s__, 4, err__, fail__)


#define CSM_CHECKED_WREAD_U64(out__, s__,  err__, fail__)                   \
    CSM_CHECKED_WREAD(csm_wstream_u64, out__, s__, 8, err__, fail__)


#define CSM_CHECKED_WREAD_I8(out__, s__,  err__, fail__)                    \
    CSM_CHECKED_WREAD(csm_wstream_i8, out__, s__, 1, err__, fail__)


#define CSM_CHECKED_WREAD_I16(out__, s__,  err__, fail__)                   \
    CSM_CHECKED_WREAD(csm_wstream_i16, out__, s__, 2, err__, fail__)


#define CSM_CHECKED_WREAD_I32(out__, s__,  err__, fail__)                   \
    CSM_CHECKED_WREAD(csm_wstream_i32, out__, s__, 4, err__, fail__)


#define CSM_CHECKED_WREAD_I64(out__, s__,  err__, fail__)                   \
    CSM_CHECKED_WREAD(csm_wstream_i64, out__, s__, 8, err__, fail__)


#define CSM_CHECKED_WREAD_F32(out__, s__,  err__, fail__)                   \
    CSM_CHECKED_WREAD(csm_wstream_f32, out__, s__, 4, err__, fail__)


#define CSM_CHECKED_WREAD_F64(out__, s__,  err__, fail__)                   \
    CSM_CHECKED_WREAD(csm_wstream_f64, out__, s__, 8, err__, fail__)


/**
 * pref__   = Object prefix, used to call other functions.
 * fb__     = Variable to store object array.
 * until__  = How many objects to clean up.
 */
#define CSM_DSRL_BLOCK_UNWIND(pref__, fb__, until__)                        \
    do {                                                                    \
        if (fb__) {                                                         \
            uint64_t i = 0;                                                 \
            for (i = 0; i < until__; i++) {                                 \
                deinit_ ## pref__ ## _(fb__ + i);                           \
            }                                                               \
            csm_free(fb__);                                                 \
        }                                                                   \
    } while (0)


/**
 * pref__   = Object prefix, used to call other functions.
 * fc__     = Variable to store object count, is U32.
 * fb__     = Variable to store object array.
 * m__      = The current module.
 * ws__     = Wide stream to use.
 * err__    = The error variable to use.
 * fail__   = Label to jump to on failure.
 */
#define CSM_DSRL_BLOCK(pref__, fc__, fb__, m__, ws__, err__, fail__)        \
    do {                                                                    \
        uint64_t cr = 0;                                                    \
        size_t bytes = 0;                                                   \
        CSM_CHECKED_WREAD_U32(fc__, ws__, err__, fail__);                   \
        fb__ = NULL;                                                        \
        if (fc__ != 0) {                                                    \
            bytes = (fc__ * sizeof(*fb__));                                 \
            fb__ = csm_malloc(bytes);                                       \
            if (fb__ == NULL) {                                             \
                err__ = CSM_ERR_MEMORY;                                     \
                goto fail__;                                                \
            }                                                               \
            csm_memclear(fb__, bytes);                                      \
            for (cr = 0; cr < fc__; cr++) {                                 \
                err__ = dsrl_ ## pref__ ## _((fb__ + cr), m__, ws__);       \
                if (err__) {                                                \
                    CSM_DSRL_BLOCK_UNWIND(pref__, fb__, cr);                \
                    goto fail__;                                            \
                }                                                           \
            }                                                               \
        }                                                                   \
    } while (0)


/* Internal typedefs used for convenience. */
typedef struct csm_bc_module module;
typedef struct csm_bc_method method;
typedef struct csm_bc_object object;
typedef struct csm_bc_string string;
typedef struct csm_bc_ete ete;
typedef struct csm_wstream wstream;


/* NOTE: Abide by specific naming conventions for dsrl and cleanup! */
static int dsrl_method_(method* out, module* m, wstream* ws);
static int dsrl_object_(object* out, module* m, wstream* ws);
static int dsrl_string_(string* out, module* m, wstream* ws);
static int dsrl_int64_(int64_t* out, module* m, wstream* ws);
static int dsrl_flt64_(double* out, module* m, wstream* ws);
static int dsrl_ete_(ete* out, module *m, wstream* ws);
static void deinit_method_(method* dead); 
static void deinit_object_(object* dead);
static void deinit_string_(string* dead);
static void deinit_int64_(int64_t* dead);
static void deinit_flt64_(double* dead);
static void deinit_ete_(ete* dead);


static int 
dsrl_method_(method* out, module* m, wstream* ws)
{
    uint64_t i = 0;
    int err = 0;

    (void) m;

    CSM_CHECKED_WREAD_U8(out->status_0, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U8(out->status_1, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U32(out->name, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U32(out->debugsymbol, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U32(out->sigblock, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U8(out->limstack, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U8(out->limlocal, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U32(out->insbytec, ws, err, _unwind_0);

    if (csm_wstream_lt(ws, out->insbytec)) {
        return CSM_ERR_EOS;
    }
    
    out->insbytes = csm_malloc(out->insbytec);
    
    if (out->insbytes == NULL) {
        return CSM_ERR_MEMORY;
    }

    /* TODO: Account for endianness by swapping here!   */
    /* TODO: Have to make an alloc when we copy?        */
    for (i = 0; i < out->insbytec; i++) {
        out->insbytes[i] = csm_wstream_u8(ws);
    }

    CSM_DSRL_BLOCK(ete, out->etec, out->etes, m, ws, err, _unwind_1);

    return 0;

_unwind_1:
    csm_free(out->insbytes);
_unwind_0:
    return err;
}


static int
dsrl_object_(object* out, module* m, wstream* ws)
{
    int err = 0;

    (void) m;

    CSM_CHECKED_WREAD_U8(out->status_0, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U32(out->name, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U32(out->fieldblock, ws, err, _unwind_0);

    return 0;

_unwind_0:
    return err;
}


static int
dsrl_string_(string* out, module* m, wstream* ws)
{
    uint64_t i = 0;
    int err = 0;

    (void) m;

    CSM_CHECKED_WREAD_U32(out->length, ws, err, _unwind_0);
    
    if (csm_wstream_lt(ws, out->length)) {
        return CSM_ERR_EOS;
    } else if (out->length == 0) {
        out->data = NULL;
        return 0;
    }

    /* NOTE: To get rid of malloc, we can't rely on NULL terminators. */
    out->data = csm_malloc(out->length + 1);

    if (out->data == NULL) {
        return CSM_ERR_MEMORY;
    }

    out->data[out->length] = 0;

    for (i = 0; i < out->length; i++) {
        out->data[i] = csm_wstream_i8(ws);
    }
    
    return 0;

_unwind_0:
    return err;
}


static int
dsrl_int64_(int64_t* out, module* m, wstream* ws)
{
    int64_t temp = 0;
    int err = 0;

    (void) m;

    CSM_CHECKED_WREAD_I64(temp, ws, err, _unwind_0);
    *out = temp;
    
    return 0;

_unwind_0:
    return err;
}


static int
dsrl_flt64_(double* out, module* m, wstream* ws)
{
    double temp = 0;
    int err = 0;

    (void) m;

    CSM_CHECKED_WREAD_F64(temp, ws, err, _unwind_0);
    *out = temp;
    
    return 0;

_unwind_0:
    return err;
}


static int
dsrl_ete_(ete* out, module* m, wstream* ws)
{
    int err = 0;

    (void) m;

    CSM_CHECKED_WREAD_U32(out->type, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U32(out->start, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U32(out->end, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U32(out->target, ws, err, _unwind_0);

    return 0;

_unwind_0:
    return err;
}


static void
deinit_method_(method* dead)
{
    if (dead == NULL) { return; }
    if (dead->insbytec != 0) { csm_free(dead->insbytes); }
    CSM_DSRL_BLOCK_UNWIND(ete, dead->etes, dead->etec);
    return;
}


static void
deinit_object_(object* dead) { (void) dead; return; }



static void
deinit_string_(string* dead)
{
    if (dead == NULL) { return; }
    if (dead->length) { csm_free(dead->data); }
    return;
}


static void
deinit_int64_(int64_t* dead) { (void) dead; return; }


static void
deinit_flt64_(double* dead) { (void) dead; return; }


static void
deinit_ete_(ete* dead) { (void) dead; return; }


static int
check_magic_(wstream* ws)
{
    static const char* csm_magic = "csmx";
    size_t i = 0;
    int err = 0;

    for (i = 0; i < strlen(csm_magic); i++) {
        int8_t c = 0;

        CSM_CHECKED_WREAD_I8(c, ws, err, _unwind_0);
        
        if (c != csm_magic[i]) {
            return CSM_ERR_MAGIC;
        }
    }

    return 0;

_unwind_0:
    return err;
}


int
csm_bc_module_init(struct csm_bc_module* m, void* buf, size_t size)
{
    struct csm_wstream* ws;
    struct csm_wstream wstream;
    int err = 0;

    if (size == 0) { return CSM_ERR_EOS; }

    csm_memclear(m, sizeof(*m));
    ws = &wstream;
    
    csm_wstream_init(ws, buf, size, CSM_WSTREAM_MODE_LE);
    err = check_magic_(ws);
    
    if (err) { return err; }

    m->bufsize = size;
    m->buf = buf;

    CSM_CHECKED_WREAD_U8(m->status_0, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U8(m->status_1, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U8(m->status_2, ws, err, _unwind_0);
    CSM_CHECKED_WREAD_U8(m->status_3, ws, err, _unwind_0);

    CSM_DSRL_BLOCK(method, m->methodc, m->methods, m, ws, err, _unwind_0); 
    CSM_DSRL_BLOCK(object, m->objectc, m->objects, m, ws, err, _unwind_1);
    CSM_DSRL_BLOCK(string, m->strc, m->strs, m, ws, err, _unwind_2);
    CSM_DSRL_BLOCK(int64, m->int64c, m->int64s, m, ws, err, _unwind_3);
    CSM_DSRL_BLOCK(flt64, m->flt64c, m->flt64s, m, ws, err, _unwind_4);

    return 0;

_unwind_4:
    CSM_DSRL_BLOCK_UNWIND(int64, m->int64s, m->int64c);
_unwind_3:
    CSM_DSRL_BLOCK_UNWIND(string, m->strs, m->strc);
_unwind_2:
    CSM_DSRL_BLOCK_UNWIND(object, m->objects, m->objectc);
_unwind_1:
    CSM_DSRL_BLOCK_UNWIND(method, m->methods, m->methodc);
_unwind_0:
    return err;
}


int
csm_bc_module_init_file(struct csm_bc_module* m, const char* name)
{
    (void) m;
    (void) name;

    assert("Not implemented yet." == 0);
    return 0;
}


/* TODO: Add support for intern64/monostream, when we do that. */
void csm_bc_module_deinit(struct csm_bc_module* m)
{
    /* TODO: Do we free buffer here? */
    (void) m->buf;

    CSM_DSRL_BLOCK_UNWIND(method, m->methods, m->methodc);
    CSM_DSRL_BLOCK_UNWIND(object, m->objects, m->objectc);
    CSM_DSRL_BLOCK_UNWIND(string, m->strs, m->strc);
    CSM_DSRL_BLOCK_UNWIND(int64, m->int64s, m->int64c);
    CSM_DSRL_BLOCK_UNWIND(flt64, m->flt64s, m->flt64c);
}

