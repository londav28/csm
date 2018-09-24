#ifndef CSM_FILE_H_
#define CSM_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>
#include <stdint.h>


int
csm_file_size(size_t *out, const char *name);


int
csm_file_read(const char *name, void *buf, size_t size);


int
csm_file_write(const char *name, void *buf, size_t size);


#ifdef __cplusplus
}
#endif

#endif
