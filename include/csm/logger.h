#ifndef CSM_INCLUDE_CSM_LOGGER_H_
#define CSM_INCLUDE_CSM_LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* TODO: Perhaps come up with file struct? */
#include <stdio.h>

typedef struct csm_logger {
    FILE* out;
    int is_logging_to_stdout;
    int is_logging;
    int level;
    const char* prefix;
} csm_logger;

#ifdef __cplusplus
}
#endif
#endif
