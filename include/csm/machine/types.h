#ifndef CSM_INCLUDE_CSM_MACHINE_TYPES_H_
#define CSM_INCLUDE_CSM_MACHINE_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Might just move this header over to the "machine/state.h" header. */
#define CSM_TYPE_NONE                   0
#define CSM_TYPE_INTEGER_8              1
#define CSM_TYPE_INTEGER_16             2
#define CSM_TYPE_INTEGER_32             3
#define CSM_TYPE_INTEGER_64             4
#define CSM_TYPE_FLOAT_32               5
#define CSM_TYPE_FLOAT_64               6
#define CSM_TYPE_OBJECT                 7
#define CSM_TYPE_STRING_CONSTANT        8
#define CSM_TYPE_NULL                   9
#define CSM_TYPE_ARRAY_I8               10
#define CSM_TYPE_ARRAY_I16              11
#define CSM_TYPE_ARRAY_I32              12
#define CSM_TYPE_ARRAY_I64              13
#define CSM_TYPE_ARRAY_F64              14
#define CSM_TYPE_ARRAY_OBJ              15

#ifdef __cplusplus
}
#endif

#endif
