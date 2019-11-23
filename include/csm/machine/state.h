#ifndef CSM_INCLUDE_CSM_MACHINE_STATE_H_
#define CSM_INCLUDE_CSM_MACHINE_STATE_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "csm/types.h"
#include "csm/stream.h"


struct csm_bytecode_module;
struct csm_bytecode_method;
struct csm_bytecode_object;
struct csm_native_method;
struct csm_native_object;
struct csm_stream;
struct csm_object_header;
struct csm_object_instance;
struct csm_method_descriptor;
struct csm_object_descriptor;
struct csm_frame;
struct csm_thread;
struct csm_machine;
struct csm_unpacked_op;
struct csm_cell;


struct csm_unpacked_op {

    uint8_t op;
    struct csm_unpacked_op (*handler)(struct csm_thread*);

};


/* Commonly returned by most handlers. */
typedef struct csm_unpacked_op (*csm_handler)(struct csm_thread*);


/* Native handlers have a different signature? */
typedef void (*csm_native_handler)(struct csm_thread*);


/* This format causes some fragmentation, we might switch later. */
struct csm_cell {
    union {

        uint32_t u32;
        uint64_t u64;
        void *raw;
        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;
        float f32;
        double f64;
        char *str;
        const char *str_const;

    } as;
};


enum csm_descriptor_type {

    CSM_DESCRIPTOR_BC_METHOD = 0,
    CSM_DESCRIPTOR_NATIVE_METHOD,
    CSM_DESCRIPTOR_BC_OBJECT,
    CSM_DESCRIPTOR_NATIVE_OBJECT,
    CSM_DESCRIPTOR_BUILTIN_ARRAY,
    CSM_DESCRIPTOR_UNRESOLVED

};


struct csm_descriptor {

    union {

        struct csm_bc_method *bc_method;
        struct csm_native_method *native_method;
        struct csm_bc_object *bc_object;
        struct csm_native_object *native_object;
        void *raw;

    } as;

    int what;

};


extern struct csm_descriptor *csm_descriptor_array;


enum csm_array_type {

    CSM_ARRAY_TYPE_I8 = 0,
    CSM_ARRAY_TYPE_I16,
    CSM_ARRAY_TYPE_I32,
    CSM_ARRAY_TYPE_I64,
    CSM_ARRAY_TYPE_F64,
    CSM_ARRAY_TYPE_OBJ

};


struct csm_array_header {

    int64_t length;
    void *data;
    int dimensions;
    int kind;

};


struct csm_frame {

    struct csm_cell *saved_datastack_pos;
    struct csm_cell *local_start;
    struct csm_descriptor owner;
    struct csm_stream stream;

};


struct csm_thread {

    struct csm_cell *datastack_bot;
    struct csm_cell *datastack_top;
    struct csm_cell *datastack_pos;
    uint32_t datastack_size;

    /* We may end up removing this and threading frames later. */
    struct csm_frame *callstack_bot;
    struct csm_frame *callstack_top;
    struct csm_frame *callstack_pos;
    uint32_t callstack_size;

    /* For debugging purposes. */
    uint8_t last_op;

    /* Pointer to parent! */
    struct csm_machine* machine;

};


struct csm_machine {

    /* Let's just pretend like LDG & STG don't exist for now. */
    struct csm_cell *globals;

    /* We use this even in a single threaded machine. */
    struct csm_thread *mainthread;

    /* List of all threads, where 0 is mainthread. */
    struct csm_thread **threads;
    uint64_t threadc;

    /* So that the machine knows where to start. */
    struct csm_bc_module *start_module;
    struct csm_bc_method *start_method;

    /* Eventually move this into separate GC profile. */
    uint64_t gc_pause_count;
    uint8_t gc_mem_threshold;
    uint8_t gc_latency_level;


};


struct csm_native_method {

    const char *name;
    uint8_t parameter_count;
    const char *parameter_str;
    int is_void;
    const char *rtype_str;
    int throws_exception;
    csm_native_handler handler;

};


struct csm_native_object {

    const char *name;
    uint8_t field_count;
    const char *field_str;
    size_t size;
    csm_native_handler init;
    csm_native_handler fini;

};


#ifdef __cplusplus
}
#endif

#endif
