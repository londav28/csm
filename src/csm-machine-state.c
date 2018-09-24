#include "csm/machine/state.h"
#include <stddef.h>


static struct csm_descriptor csm_descriptor_array_ = {
    .as.raw = NULL,
    .what = CSM_DESCRIPTOR_BUILTIN_ARRAY
};


/* Singleton descriptor used when creating arrays. */
struct csm_descriptor *csm_descriptor_array = &csm_descriptor_array_;