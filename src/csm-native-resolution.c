#include "csm/native/resolution.h"
#include "csm/native/module/io.h"
#include "csm/native/module/mem.h"
#include "csm/machine/state.h"
#include "csm/bytecode/format.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>


const char* csm_native_symbol_prefix = "csm";


static struct csm_descriptor unresolved = {
    .as.raw = NULL,
    .what = CSM_DESCRIPTOR_UNRESOLVED
};


/* Copy of the fetch code used in bytecode display. */
static struct csm_bc_string* get_bytecode_string(
            struct csm_bc_module* m,
            uint32_t idx
){
    if (idx >= m->strc)
        return NULL;
    return m->strs + idx;
}


/* TODO: Change the reserved prefix to "csm_"? */
/* Check to see if symbol has the double underscore prefix. */
int csm_method_has_reserved_prefix(struct csm_bc_string* str)
{
    size_t length = strlen(csm_native_symbol_prefix);
    return !strncmp(csm_native_symbol_prefix, str->data, length);
}


// TODO: Something is wrong with my pointer arithmetic here.
static struct csm_descriptor search_native_methods_snail(
            struct csm_bc_string* s,
            struct csm_thread* t
){
    /* TODO: We should create a more centralized method later. */
    struct csm_native_method** tablelist[] = {
        csm_native_io_methods,
        csm_native_mem_methods,
        NULL
    };

    struct csm_native_method*** tables = tablelist;
    struct csm_native_method** table = NULL;
    struct csm_native_method* method = NULL;
    (void) t;

    while ((table = *tables) != NULL) {
        while ((method = *table) != NULL) {    
            if (!strncmp(s->data, method->name, s->length)) {
                struct csm_descriptor result = {
                    .as.native_method = method,
                    .what = CSM_DESCRIPTOR_NATIVE_METHOD
                };

                return result;
            }
            table++;
        }
        tables++;
    }

    return unresolved;
}


/* TODO: This method shares a lot of code with the above, can change? */
static struct csm_descriptor search_bc_methods_snail(
            struct csm_bc_string* s,
            struct csm_bc_module* m
){
    uint32_t i = 0;

    for (i = 0; i < m->methodc; i++) {
        struct csm_bc_method* f = m->methods + i;
        struct csm_bc_string* other = NULL;

        other = get_bytecode_string(m, f->name);

        if (strncmp(s->data, other->data, s->length) == 0) {
            struct csm_descriptor result = {
                .as.bc_method = f,
                .what = CSM_DESCRIPTOR_BC_METHOD
            };
            return result;
        }
    }

    return unresolved;
}

/* Absolute slowmode bridge for native/bytecode methods. */
struct csm_descriptor csm_method_bridge_snail(
            uint32_t idx,
            struct csm_thread* t
) {
    struct csm_bc_module* m = NULL;
    struct csm_bc_string* s = NULL;
    struct csm_descriptor result;

    m = t->machine->start_module;
    s = get_bytecode_string(m, idx);
    
    if (s == NULL) {
        return unresolved;
    }

    if (csm_method_has_reserved_prefix(s)) {
        result = search_native_methods_snail(s, t);
    } else {
        result = search_bc_methods_snail(s, m);
    }

    return result;
}

/* Absolute slowmode bridge for native/bytecode objects. */
struct csm_descriptor csm_object_bridge_snail(
            uint32_t idx,
            struct csm_thread* t
) {
    struct csm_descriptor result; 
    (void) idx;
    (void) t;

    result = unresolved;

    return result;
}