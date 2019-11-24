#include "csm/memset.h"
#include <string.h>

void *csm_memset(void *buf, int c, size_t size) {
    return memset(buf, c, size);
}

void *csm_memclear(void *buf, size_t size) {
    return memset(buf, 0, size);
}

