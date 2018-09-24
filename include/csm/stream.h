#ifndef CSM_STREAM_H_
#define CSM_STREAM_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>
#include <stdint.h>


/* This is the endianness of the file, not of your system! */
enum csm_stream_mode {

    CSM_STREAM_MODE_LE = 0,
    CSM_STREAM_MODE_BE

};


struct csm_stream {
    
    void* buf;
    uint32_t size;
    uint32_t pos;
    int mode;

};


void
csm_stream_init(struct csm_stream* s, void *data, uint32_t size, int fe);


void
csm_stream_restart(struct csm_stream* s);


void*
csm_stream_get_buf(struct csm_stream* s);


void*
csm_stream_get_adjusted_buf(struct csm_stream* s);


uint32_t
csm_stream_get_size(struct csm_stream* s);


uint32_t
csm_stream_get_pos(struct csm_stream* s);


uint32_t
csm_stream_get_mode(struct csm_stream* s);


int
csm_stream_lt(struct csm_stream* s, uint32_t bytes);


int
csm_stream_check_rel(struct csm_stream* s, int32_t adjust);


int
csm_stream_check_abs(struct csm_stream* s, uint32_t adjust);


void
csm_stream_shift_rel(struct csm_stream* s, int32_t adjust);


void
csm_stream_shift_abs(struct csm_stream* s, uint32_t adjust);


uint8_t
csm_stream_u8(struct csm_stream* s);


uint16_t
csm_stream_u16(struct csm_stream* s);


uint32_t
csm_stream_u32(struct csm_stream *s);


uint64_t
csm_stream_u64(struct csm_stream *s);


int8_t
csm_stream_i8(struct csm_stream *s);


int16_t
csm_stream_i16(struct csm_stream *s);


int32_t
csm_stream_i32(struct csm_stream *s);


int64_t
csm_stream_i64(struct csm_stream *s);


float
csm_stream_f32(struct csm_stream *s);


double
csm_stream_f64(struct csm_stream *s);


#ifdef __cplusplus
}
#endif

#endif