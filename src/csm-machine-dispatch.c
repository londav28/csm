#include "csm/machine/dispatch.h"
#include "csm/handlers/unsafe.h"
#include "csm/errorcodes.h"
#include "csm/bytecode/format.h"
#include "csm/machine/state.h"
#include "csm/bytecode/opcodes.h"
#include "csm/stream.h"
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>


/**
 * BIG TODO:
 *
 * Parameterize handlers so that the prep code can access them (regardless of
 * selection) in a generic fashion.
 *
 */


/* Stupid typedefs for convenience. */
typedef struct csm_bc_module module;
typedef struct csm_bc_method method;
typedef struct csm_bc_object object;
typedef struct csm_bc_string string;


/* Assert that offset is within bounds. */
#define BUFFER_CHECK(b, p, a) assert((p - a) >= b)

/* For a given base/pos/amount, assert that within bounds after push. */
#define BUFFER_MOVE_CHECK(b, p, a) p -= a; assert(p >= b)


/* TODO: Replace assert macro with a more meaningful exit code. */
/* TODO: Add to header for frame manipulation. */
struct csm_frame* local_frame_push(struct csm_thread* t, method* m)
{
    struct csm_frame* result;
    uint32_t movesize;

    BUFFER_MOVE_CHECK(t->callstack_bot, t->callstack_pos, 1);

    result = t->callstack_pos;
    result->owner.what = CSM_DESCRIPTOR_BC_METHOD;
    result->owner.as.bc_method = m;

    csm_stream_init(
        &result->stream,
        m->insbytes,
        m->insbytec,
        CSM_STREAM_MODE_LE
    );

    /* Determine if we have enough space for stack/locals/params. */
    movesize = m->limlocal + m->limstack + m->_paramc_;
    BUFFER_CHECK(t->datastack_bot, t->datastack_pos, movesize);

    /* Make space for the local portion of the frame. */
    movesize = m->limlocal + m->_paramc_;
    BUFFER_MOVE_CHECK(t->datastack_bot, t->datastack_pos, movesize);
    result->local_start = t->datastack_pos;

    return result;
}


struct csm_unpacked_op first_op_decode(struct csm_frame* frame)
{
    struct csm_unpacked_op result;

    if (csm_stream_lt(&frame->stream, 1)) {
        /* Change this to a special error hander instead? */
        assert("Unable to decode first opcode in stream!" == 0);
    }

    result.op = csm_stream_u8(&frame->stream);

    /* TODO: Parameterize handlers! */
    result.handler = csm_handlers_unsafe[result.op];
    return result;
}

/* TODO: Add separate dispatch loop to use just for traces? */
int csm_dispatch_basic(struct csm_thread*t, struct csm_bc_method* m)
{
    struct csm_frame* frame;
    struct csm_unpacked_op uop;
    uint64_t decodes = 0;

    frame = local_frame_push(t, m);
    if (frame == NULL) { return CSM_ERR_STARTUP; }

    uop = first_op_decode(frame);

    while (uop.handler != NULL) {
        /*
        TODO: Add flag to record execution traces, IE...
        if (INSTRUCTION_TRACES_ENABLED(thread->machine)) {
            fprintf(machine->tracelog, "%lu : %s\n",
                    decodes,
                    csm_op_mnemonic(uop.op)
            );
        }
        */
        t->last_op = uop.op;
        uop = uop.handler(t);
        decodes++;
    }

    return 0;
}
