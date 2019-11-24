#ifndef CSM_INCLUDE_CSM_MACHINE_STATE_H_
#define CSM_INCLUDE_CSM_MACHINE_STATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"
#include "csm/bytecode/format.h"
#include "csm/stream.h"

/* So that we don't have to worry about the ordering of things. */
struct csm_native_method;
struct csm_native_object;
struct csm_object_header;
struct csm_object_instance;
struct csm_method_descriptor;
struct csm_object_descriptor;
struct csm_frame;
struct csm_thread;
struct csm_machine;
struct csm_unpacked_op;
struct csm_cell;

/* This format causes some fragmentation, we might switch later. */
typedef struct csm_cell {
    union {

        csm_u8  u8;
        csm_u16 u16;
        csm_u32 u32;
        csm_u64 u64;
        csm_i8 i8;
        csm_i16 i16;
        csm_i32 i32;
        csm_i64 i64;
        csm_f32 f32;
        csm_f64 f64;
        void* raw;

    } as;
} csm_cell;

typedef struct csm_thread {

    struct csm_cell *datastack_bot;
    struct csm_cell *datastack_top;
    struct csm_cell *datastack_pos;
    csm_u32 datastack_size;

    /* We may end up removing this and threading frames later. */
    struct csm_frame *callstack_bot;
    struct csm_frame *callstack_top;
    struct csm_frame *callstack_pos;
    csm_u32 callstack_size;

    /* For debugging purposes. */
    csm_u8 last_op;

    /* Pointer to parent! */
    struct csm_machine *machine;

} csm_thread;

typedef struct csm_machine {

    /* Let's just pretend like LDG & STG don't exist for now. */
    struct csm_cell *globals;

    /* We use this even in a single threaded machine. */
    struct csm_thread *mainthread;

    /* List of all threads, where 0 is mainthread. */
    struct csm_thread **threads;
    csm_u64 threadc;

    /* So that the machine knows where to start. */
    csm_bc_module *start_module;
    csm_bc_method *start_method;

    /* Eventually move this into separate GC profile. */
    csm_u64 gc_pause_count;
    csm_u8 gc_mem_threshold;
    csm_u8 gc_latency_level;

} csm_machine;


typedef struct csm_unpacked_op {

    csm_u8 op;
    struct csm_unpacked_op (*handler)(csm_thread*);

} csm_unpacked_op;

/* Commonly returned by most handlers. */
typedef csm_unpacked_op (*csm_handler)(csm_thread*);

/* Native handlers have a different signature? */
typedef void (*csm_native_handler)(csm_thread*);

typedef enum csm_descriptor_type {

    CSM_DESCRIPTOR_BC_METHOD = 0,
    CSM_DESCRIPTOR_NATIVE_METHOD,
    CSM_DESCRIPTOR_BC_OBJECT,
    CSM_DESCRIPTOR_NATIVE_OBJECT,
    CSM_DESCRIPTOR_BUILTIN_ARRAY,
    CSM_DESCRIPTOR_UNRESOLVED

} csm_descriptor_type;

typedef struct csm_descriptor {
    union {

        csm_bc_method *bc_method;
        struct csm_native_method *native_method;
        csm_bc_object *bc_object;
        struct csm_native_object *native_object;
        void *raw;

    } as;
    int what;

} csm_descriptor;

extern struct csm_descriptor *csm_descriptor_array;

typedef enum csm_array_type {

    CSM_ARRAY_TYPE_I8 = 0,
    CSM_ARRAY_TYPE_I16,
    CSM_ARRAY_TYPE_I32,
    CSM_ARRAY_TYPE_I64,
    CSM_ARRAY_TYPE_F64,
    CSM_ARRAY_TYPE_OBJ

} csm_array_type;

typedef struct csm_array_header {

    csm_i64 length;
    void *data;
    int dimensions;
    int kind;

} csm_array_header;

typedef struct csm_frame {

    struct csm_cell *saved_datastack_pos;
    struct csm_cell *local_start;
    struct csm_descriptor owner;

    /* TODO: If we can use pointer reads directly, we can remove this. */
    csm_stream stream;

} csm_frame;

typedef struct csm_native_method {

    const char *name;
    csm_u8 parameter_count;
    const char *parameter_str;
    int is_void;
    const char *rtype_str;
    int throws_exception;
    csm_native_handler handler;

} csm_native_method;

typedef struct csm_native_object {

    const char *name;
    csm_u8 field_count;
    const char *field_str;
    size_t size;
    csm_native_handler init;
    csm_native_handler fini;

} csm_native_object;

#ifdef __cplusplus
}
#endif
#endif
