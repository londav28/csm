#include "csm/machine/bridge.h"
#include "csm/native/module/io.h"
#include "csm/native/module/mem.h"
#include "csm/machine/state.h"
#include "csm/bytecode/format.h"
#include "csm/types.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

const char *csm_native_symbol_prefix = "csm";

static csm_descriptor unresolved = {
    .as.raw = NULL,
    .what = CSM_DESCRIPTOR_UNRESOLVED
};

/* Copy of the fetch code used in bytecode display. */
static csm_bc_string *get_bytecode_string(csm_bc_module *m, csm_u32 idx) {
    if (idx >= m->strc) { return NULL; }
    return m->strs + idx;
}

/* Check to see if symbol has the double underscore prefix. */
int csm_symbol_has_reserved_prefix(csm_bc_string *str)
{
    size_t length = strlen(csm_native_symbol_prefix);
    return !strncmp(csm_native_symbol_prefix, str->data, length);
}

/* TODO: Something is wrong with my pointer arithmetic here. */
static struct csm_descriptor search_native_methods_snail(
            csm_bc_string *s,
            csm_thread *t
){
    /* TODO: We should create a more centralized method later. */
    csm_native_method **tablelist[] = {
        csm_native_io_methods,
        csm_native_mem_methods,
        NULL
    };

    csm_native_method ***tables = tablelist;
    csm_native_method **table = NULL;
    csm_native_method *method = NULL;
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
static csm_descriptor search_bc_methods_snail(
            csm_bc_string *s,
            csm_bc_module *m
){
    csm_u32 i = 0;

    for (i = 0; i < m->methodc; i++) {
        csm_bc_method *f = m->methods + i;
        csm_bc_string *other = NULL;

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

/* TODO: Something is wrong with my pointer arithmetic here. */
static struct csm_descriptor search_native_objects_snail(
            csm_bc_string *s,
            csm_thread *t
){
    /* TODO: We should create a more centralized method later. */
    csm_native_object **tablelist[] = {
        csm_native_io_objects,
        csm_native_mem_objects,
        NULL
    };


    csm_native_object ***tables = tablelist;
    csm_native_object **table = NULL;
    csm_native_object *object = NULL;
    (void) t;

    assert("Not implemented yet!" == NULL);

    while ((table = *tables) != NULL) {
        while ((object = *table) != NULL) {    
            if (!strncmp(s->data, object->name, s->length)) {
                struct csm_descriptor result = {
                    .as.native_object = object,
                    .what = CSM_DESCRIPTOR_NATIVE_OBJECT
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
static csm_descriptor search_bc_objects_snail(
            csm_bc_string *s,
            csm_bc_module *m
){
    csm_u32 i = 0;

    for (i = 0; i < m->objectc; i++) {
        csm_bc_object *o = m->objects + i;
        csm_bc_string *other = NULL;

        other = get_bytecode_string(m, o->name);

        if (strncmp(s->data, other->data, s->length) == 0) {
            struct csm_descriptor result = {
                .as.bc_object = o,
                .what = CSM_DESCRIPTOR_BC_OBJECT
            };

            return result;
        }
    }

    return unresolved;
}

/* Absolute slowmode bridge for native/bytecode methods. */
csm_descriptor csm_bridge_snail(
            int kind,
            csm_u32 idx,
            csm_thread *t
) {
    int has_reserved_prefix = CSM_FALSE;
    csm_bc_module *m = NULL;
    csm_bc_string *s = NULL;
    csm_descriptor result;

    /* TODO: Change for multi-module lookup. */
    m = t->machine->start_module;
    s = get_bytecode_string(m, idx);

    if (s == NULL) {
        return unresolved;
    }

    has_reserved_prefix = csm_symbol_has_reserved_prefix(s);

    /* Specialize the lookup. */
    if (has_reserved_prefix) {
        switch (kind) {
        case CSM_BRIDGE_METHOD:
            kind = CSM_BRIDGE_NATIVE_METHOD;
            break;
        case CSM_BRIDGE_OBJECT:
            kind = CSM_BRIDGE_NATIVE_OBJECT;
            break;
        }
    } else {
        switch (kind) {
        case CSM_BRIDGE_METHOD:
            kind = CSM_BRIDGE_BC_METHOD;
            break;
        case CSM_BRIDGE_OBJECT:
            kind = CSM_BRIDGE_BC_OBJECT;
            break;
        }
    }

    assert(kind != CSM_BRIDGE_METHOD);
    assert(kind != CSM_BRIDGE_OBJECT);

    /* Perform the appropriate lookup. */
    switch (kind) {
    case CSM_BRIDGE_NATIVE_METHOD:
        result = search_native_methods_snail(s, t);
        break;
    case CSM_BRIDGE_NATIVE_OBJECT:
        result = search_native_objects_snail(s, t);
        break;
    case CSM_BRIDGE_BC_METHOD:
        result = search_bc_methods_snail(s, m);
        break;
    case CSM_BRIDGE_BC_OBJECT:
        result = search_bc_objects_snail(s, m);
        break;
    default:
        assert("Should not reach here!" == NULL);
        break;
    }


    return result;
}

