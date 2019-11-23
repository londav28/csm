#ifndef CSM_INCLUDE_CSM_STREAM_H_
#define CSM_INCLUDE_CSM_STREAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csm/types.h"

/* This is the endianness of the file, not of your system! */
typedef enum csm_stream_mode {

    CSM_STREAM_MODE_LE = 0,
    CSM_STREAM_MODE_BE

} csm_stream_mode;

typedef struct csm_stream {
    
    void* buf;
    csm_u32 size;
    csm_u32 pos;
    int mode;

} csm_stream;

void csm_stream_init(csm_stream* s, void *data, csm_u32 size, int fe);

void csm_stream_restart(csm_stream* s);

void *csm_stream_get_buf(csm_stream* s);

void *csm_stream_get_adjusted_buf(csm_stream* s);

csm_u32 csm_stream_get_size(csm_stream* s);

csm_u32 csm_stream_get_pos(csm_stream* s);

csm_u32 csm_stream_get_mode(csm_stream* s);

int csm_stream_lt(csm_stream* s, csm_u32 bytes);

int csm_stream_check_rel(csm_stream* s, csm_i32 adjust);

int csm_stream_check_abs(csm_stream* s, csm_u32 adjust);

void csm_stream_shift_rel(csm_stream* s, csm_i32 adjust);

void csm_stream_shift_abs(csm_stream* s, csm_u32 adjust);

csm_u8 csm_stream_u8(csm_stream* s);

csm_u16 csm_stream_u16(csm_stream* s);

csm_u32 csm_stream_u32(csm_stream *s);

csm_u64 csm_stream_u64(csm_stream *s);

csm_i8 csm_stream_i8(csm_stream *s);

csm_i16 csm_stream_i16(csm_stream *s);

csm_i32 csm_stream_i32(csm_stream *s);

csm_i64 csm_stream_i64(csm_stream *s);

csm_f32 csm_stream_f32(csm_stream *s);

csm_f64 csm_stream_f64(csm_stream *s);

#ifdef __cplusplus
}
#endif
#endif
