#include "csm/machine/state.h"
#include "csm/types.h"

static csm_descriptor csm_descriptor_array_ = {
    .as.raw = NULL,
    .what = CSM_DESCRIPTOR_BUILTIN_ARRAY
};

/* Singleton descriptor used when creating arrays. */
csm_descriptor *csm_descriptor_array = &csm_descriptor_array_;

