#ifndef CSM_INCLUDE_CSM_WSTREAM_H_
#define CSM_INCLUDE_CSM_WSTREAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"

/* This is the endianness of the file, not of your system! */
typedef enum {

    CSM_WSTREAM_MODE_LE = 0,
    CSM_WSTREAM_MODE_BE

} csm_wstream_mode;

/* Same exact interface as "csm_stream", just in 64 bits. */
typedef struct {
    
    void* buf;
    csm_u64 size;
    csm_u64 pos;
    int mode;

} csm_wstream;

void csm_wstream_init(csm_wstream* s, void *data, csm_u64 size, int fe);

void csm_wstream_restart(csm_wstream* s);

void *csm_wstream_get_buf(csm_wstream* s);

void *csm_wstream_get_adjusted_buf(csm_wstream* s);

csm_u64 csm_wstream_get_size(csm_wstream* s);

csm_u64 csm_wstream_get_pos(csm_wstream* s);

int csm_wstream_get_mode(csm_wstream* s);

int csm_wstream_lt(csm_wstream* s, uint64_t bytes);

/* Returns true if the stream would go out-of-bounds. */
int csm_wstream_check_rel(csm_wstream* s, csm_i64 adjust);

/* Returns true if the stream would go out-of-bounds. */
int csm_wstream_check_abs(csm_wstream* s, csm_u64 adjust);

/* Performs relative (from current position) shift, no check. */
void csm_wstream_shift_rel(csm_wstream* s, csm_i64 adjust);

/* Performs absolute (from start of stream) shift, no check. */
void csm_wstream_shift_abs(csm_wstream* s, csm_u64 adjust);

csm_u8 csm_wstream_u8(csm_wstream* s);

csm_u16 csm_wstream_u16(csm_wstream* s);

csm_u32 csm_wstream_u32(csm_wstream *s);

csm_u64 csm_wstream_u64(csm_wstream *s);

csm_i8 csm_wstream_i8(csm_wstream *s);

csm_i16 csm_wstream_i16(csm_wstream *s);

csm_i32 csm_wstream_i32(csm_wstream *s);

csm_i64 csm_wstream_i64(csm_wstream *s);

csm_f32 csm_wstream_f32(csm_wstream *s);

csm_f64 csm_wstream_f64(csm_wstream *s);

#ifdef __cplusplus
}
#endif
#endif
