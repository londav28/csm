#include "csm/file.h"
#include <stdio.h>

/* Slow, but portable I suppose. Using SEEK_END is not portable. */
int csm_file_size(size_t* out, const char* name) {
    size_t result = 0;
    FILE* fp = NULL;

    fp = fopen(name, "rb");

    if (fp == NULL) {
        /* We should replace this with error codes later. */
        return 1;
    }

    while (fgetc(fp) != EOF) { result++; }

    fclose(fp);
    *out = result;
    
    return 0;
}


int csm_file_read(const char* name, void* buf, size_t size)
{
    FILE* fp = NULL;
    size_t bytes = 0;

    fp = fopen(name, "rb");
    if (fp == NULL) {
        return 1;
    }

    bytes = fread(buf, 1, size, fp);
    fclose(fp);

    if (bytes != size) {
        return 1;
    }
    
    return 0;
}


int csm_file_write(const char* name, void* buf, size_t size)
{
    FILE* fp = NULL;
    size_t bytes = 0;

    fp = fopen(name, "wb");
    if (fp == NULL) {
        return 1;
    }

    bytes = fwrite(buf, 1, size, fp);
    fclose(fp);

    if (bytes != size) {
        return 1;
    }

    return 0;
}