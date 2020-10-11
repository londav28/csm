#include "csm/bytecode/format.h"
#include "csm/bytecode/postinit.h"
#include "csm/bytecode/display.h"
#include "csm/file.h"
#include "csm/machine/state.h"
#include "csm/machine/init.h"
#include "csm/machine/start.h"
#include "csm/memory/malloc.h"
#include "csm/memory/gc.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static
int load_bytecode(struct csm_bc_module* module, const char* name)
{
    size_t size = 0;
    uint8_t *buffer = NULL;
    int err = 0;

    if (csm_file_size(&size, name)) {
        printf("Failed to load file %s\n", name);
        return 1;
    }

    buffer = csm_malloc(size);

    if (buffer == NULL) {
        perror("Failed to allocate buffer memory");
        return 1;
    }

    if (csm_file_read(name, buffer, size)) {
        perror("Failed to read file into buffer");
        return 1;
    }

    err = csm_bc_module_init(module, buffer, size);
    if (err) {
        printf("Failed to deserialize file, code %d\n", err);
        return 1;
    }

    csm_free(buffer);

    return 0;
}

static void test_load_run_bytecode(const char* name)
{
    struct csm_bc_module module;
    struct csm_machine machine;
    int err = 0;

    if (load_bytecode(&module, name)) {
        return;
    }

    if (csm_bc_module_postinit(&module)) {
        printf("%s\n", "Failed to post-initialize module");
        return;
    }

    err = csm_machine_init_basic(&machine, NULL);
    if (err) {
        goto _unwind_0;
    }

    /* Call the basic run loop. */
    err = csm_start_basic(&machine, &module);
    if (err) {
        goto _unwind_1;
    }

    /* Don't forget to clean up! */
    csm_machine_cleanup_basic(&machine);

    /* Also clean up the module. */
    csm_bc_module_deinit(&module);

    return;

_unwind_1:

    csm_machine_cleanup_basic(&machine);

_unwind_0:

    csm_bc_module_deinit(&module);

    printf("Failed to display bytecode with error: %d\n", err);

    return;
}

int
main(int argc, char** argv)
{
    char* name = argv[1];

    if (argc < 2) {
        printf("%s\n", "Provide a bytecode file to run.");
        return 0;
    }

    if (argc > 2) {
        printf("%d %s\n", (argc - 2), "additional arguments ignored");
    }

    test_load_run_bytecode(name);

    printf("%s\n", "-- Displaying malloc stats...");
    csm_malloc_stats();

    printf("%s\n", "-- Displaying gc alloc stats...");
    csm_gc_stats();
 
    return 0;
}

