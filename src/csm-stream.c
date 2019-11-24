#include "csm/stream.h"
#include "csm/types.h"

void csm_stream_init(csm_stream *s, void* buf, csm_u32 size, int fe)
{
    s->buf = buf;
    s->size = size;
    s->pos = 0;
    s->mode = fe;

    return;
}

void csm_stream_restart(csm_stream *s)
{
    s->pos = 0;
    return;
}

int csm_stream_lt(csm_stream *s, csm_u32 bytes)
{
    return (s->pos + bytes) > s->size;
}

void *csm_stream_get_buf(csm_stream *s)
{
    return s->buf;
}

void *csm_stream_get_adjusted_buf(csm_stream *s)
{
    return ((csm_u8*) s->buf) + s->pos;
}

csm_u32 csm_stream_get_pos(csm_stream *s)
{
    return s->pos;
}

int csm_stream_check_rel(csm_stream* s, csm_i32 adjust)
{
    (void) s;
    (void) adjust;
    return 0;
}

int csm_stream_check_abs(csm_stream *s, csm_u32 adjust)
{
    (void) s;
    (void) adjust;
    return 0;
}

void csm_stream_shift_rel(csm_stream *s, csm_i32 adjust)
{
    s->pos += adjust;
    return;
}

void csm_stream_shift_abs(csm_stream *s, csm_u32 adjust)
{
    s->pos = adjust;
    return;
}

csm_u8 csm_stream_u8(csm_stream *s)
{
    return ((csm_u8*) s->buf)[s->pos++];
}

csm_u16 csm_stream_u16(csm_stream *s)
{
    csm_u8 hi, lo;
    lo = csm_stream_u8(s);
    hi = csm_stream_u8(s);
    return ((csm_u16) hi << 8) | lo;
}

csm_u32 csm_stream_u32(csm_stream *s)
{
    csm_u16 hi, lo;
    lo = csm_stream_u16(s);
    hi = csm_stream_u16(s);
    return ((csm_u32) hi << 16) | lo;
}

csm_u64 csm_stream_u64(csm_stream *s)
{
    csm_u32 hi, lo;
    lo = csm_stream_u32(s);
    hi = csm_stream_u32(s);
    return ((csm_u64) hi << 32) | lo;
}

csm_i8 csm_stream_i8(csm_stream *s)
{
    return (csm_i8) csm_stream_u8(s);
}

csm_i16 csm_stream_i16(csm_stream *s)
{
    return (csm_i16) csm_stream_u16(s);
}

csm_i32 csm_stream_i32(csm_stream *s)
{
    return (csm_i32) csm_stream_u32(s);
}

csm_i64 csm_stream_i64(csm_stream *s)
{
    return (csm_i64) csm_stream_u64(s);
}

csm_f32 csm_stream_f32(csm_stream *s)
{
    return (csm_f32) csm_stream_u32(s);
}

csm_f64 csm_stream_f64(csm_stream *s)
{
    return (csm_f64) csm_stream_u64(s);
}

