#include "csm/wstream.h"


void
csm_wstream_init(struct csm_wstream* s, void* buf, uint64_t size, int fe)
{
    s->buf = buf;
    s->size = size;
    s->pos = 0;
    s->mode = fe;
}


void
csm_wstream_restart(struct csm_wstream* s)
{
    s->pos = 0;
}


int
csm_wstream_lt(struct csm_wstream* s, uint64_t bytes)
{
    return (s->pos + bytes) > s->size;
}


void*
csm_wstream_get_buf(struct csm_wstream* s)
{
    return s->buf;
}


void*
csm_wstream_get_adjusted_buf(struct csm_wstream* s)
{
    return ((uint8_t*) s->buf) + s->pos;
}


uint64_t
csm_wstream_get_pos(struct csm_wstream* s)
{
    return s->pos;
}


int
csm_wstream_check_rel(struct csm_wstream* s, int64_t adjust)
{
    (void) s;
    (void) adjust;
    return 0;
}


int
csm_wstream_check_abs(struct csm_wstream* s, uint64_t adjust)
{
    (void) s;
    (void) adjust;
    return 0;
}


void
csm_wstream_shift_rel(struct csm_wstream* s, int64_t adjust)
{
    s->pos += adjust;
}


void
csm_wstream_shift_abs(struct csm_wstream* s, uint64_t adjust)
{
    s->pos = adjust;
}


uint8_t
csm_wstream_u8(struct csm_wstream* s)
{
    return ((uint8_t*) s->buf)[s->pos++];
}


uint16_t
csm_wstream_u16(struct csm_wstream* s)
{
    uint8_t hi, lo;
    lo = csm_wstream_u8(s);
    hi = csm_wstream_u8(s);
    return ((uint16_t) hi << 8) | lo;
}


uint32_t
csm_wstream_u32(struct csm_wstream* s)
{
    uint16_t hi, lo;
    lo = csm_wstream_u16(s);
    hi = csm_wstream_u16(s);
    return ((uint32_t) hi << 16) | lo;
}


uint64_t
csm_wstream_u64(struct csm_wstream* s)
{
    uint32_t hi, lo;
    lo = csm_wstream_u32(s);
    hi = csm_wstream_u32(s);
    return ((uint64_t) hi << 32) | lo;
}


int8_t
csm_wstream_i8(struct csm_wstream* s)
{
    return (int8_t) csm_wstream_u8(s);
}


int16_t
csm_wstream_i16(struct csm_wstream* s)
{
    return (int16_t) csm_wstream_u16(s);
}


int32_t
csm_wstream_i32(struct csm_wstream* s)
{
    return (int32_t) csm_wstream_u32(s);
}


int64_t
csm_wstream_i64(struct csm_wstream* s)
{
    return (int64_t) csm_wstream_u64(s);
}


float
csm_wstream_f32(struct csm_wstream* s)
{
    return (float) csm_wstream_u32(s);
}


double
csm_wstream_f64(struct csm_wstream* s)
{
    return (double) csm_wstream_u64(s);
}