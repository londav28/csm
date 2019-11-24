#include "csm/memory/system.h"
#include "csm/types.h"
#include <stdlib.h>
#include <assert.h>

void *csm_sys_malloc(size_t bytes)
{
    if (bytes == 0) {
        assert("Attempt to malloc zero bytes." == 0);
    }

    return malloc(bytes); 
}


void csm_sys_free(void *ptr)
{
    if (ptr == NULL) {
        assert("Attempt to free NULL pointer." == 0);
    }

    free(ptr);

    return;
}
