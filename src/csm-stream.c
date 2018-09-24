#include "csm/stream.h"


void
csm_stream_init(struct csm_stream* s, void* buf, uint32_t size, int fe)
{
    s->buf = buf;
    s->size = size;
    s->pos = 0;
    s->mode = fe;
}


void
csm_stream_restart(struct csm_stream* s)
{
    s->pos = 0;
}


int
csm_stream_lt(struct csm_stream* s, uint32_t bytes)
{
    return (s->pos + bytes) > s->size;
}


void*
csm_stream_get_buf(struct csm_stream* s)
{
    return s->buf;
}


void*
csm_stream_get_adjusted_buf(struct csm_stream* s)
{
    return ((uint8_t*) s->buf) + s->pos;
}


uint32_t
csm_stream_get_pos(struct csm_stream* s)
{
    return s->pos;
}


int
csm_stream_check_rel(struct csm_stream* s, int32_t adjust)
{
    (void) s;
    (void) adjust;
    return 0;
}


int
csm_stream_check_abs(struct csm_stream* s, uint32_t adjust)
{
    (void) s;
    (void) adjust;
    return 0;
}


void
csm_stream_shift_rel(struct csm_stream* s, int32_t adjust)
{
    s->pos += adjust;
}


void
csm_stream_shift_abs(struct csm_stream* s, uint32_t adjust)
{
    s->pos = adjust;
}


uint8_t
csm_stream_u8(struct csm_stream* s)
{
    return ((uint8_t*) s->buf)[s->pos++];
}


uint16_t
csm_stream_u16(struct csm_stream* s)
{
    uint8_t hi, lo;
    lo = csm_stream_u8(s);
    hi = csm_stream_u8(s);
    return ((uint16_t) hi << 8) | lo;
}


uint32_t
csm_stream_u32(struct csm_stream* s)
{
    uint16_t hi, lo;
    lo = csm_stream_u16(s);
    hi = csm_stream_u16(s);
    return ((uint32_t) hi << 16) | lo;
}


uint64_t
csm_stream_u64(struct csm_stream* s)
{
    uint32_t hi, lo;
    lo = csm_stream_u32(s);
    hi = csm_stream_u32(s);
    return ((uint64_t) hi << 32) | lo;
}


int8_t
csm_stream_i8(struct csm_stream* s)
{
    return (int8_t) csm_stream_u8(s);
}


int16_t
csm_stream_i16(struct csm_stream* s)
{
    return (int16_t) csm_stream_u16(s);
}


int32_t
csm_stream_i32(struct csm_stream* s)
{
    return (int32_t) csm_stream_u32(s);
}


int64_t
csm_stream_i64(struct csm_stream* s)
{
    return (int64_t) csm_stream_u64(s);
}


float
csm_stream_f32(struct csm_stream* s)
{
    return (float) csm_stream_u32(s);
}


double
csm_stream_f64(struct csm_stream* s)
{
    return (double) csm_stream_u64(s);
}