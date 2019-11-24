#include "csm/wstream.h"
#include "csm/types.h"

void csm_wstream_init(csm_wstream *s, void* buf, csm_u64 size, int fe)
{
    s->buf = buf;
    s->size = size;
    s->pos = 0;
    s->mode = fe;

    return;
}

void csm_wstream_restart(csm_wstream *s)
{
    s->pos = 0;
    return;
}

int csm_wstream_lt(csm_wstream *s, csm_u64 bytes)
{
    return (s->pos + bytes) > s->size;
}

void *csm_wstream_get_buf(csm_wstream *s)
{
    return s->buf;
}

void *csm_wstream_get_adjusted_buf(csm_wstream *s)
{
    return ((csm_u8*) s->buf) + s->pos;
}

csm_u64 csm_wstream_get_pos(csm_wstream *s)
{
    return s->pos;
}

int csm_wstream_check_rel(csm_wstream *s, csm_i64 adjust)
{
    (void) s;
    (void) adjust;
    return 0;
}

int csm_wstream_check_abs(csm_wstream *s, csm_u64 adjust)
{
    (void) s;
    (void) adjust;
    return 0;
}

void csm_wstream_shift_rel(csm_wstream *s, csm_i64 adjust)
{
    s->pos += adjust;
    return;
}

void csm_wstream_shift_abs(csm_wstream *s, csm_u64 adjust)
{
    s->pos = adjust;
    return;
}

csm_u8 csm_wstream_u8(csm_wstream *s)
{
    return ((csm_u8*) s->buf)[s->pos++];
}

csm_u16 csm_wstream_u16(csm_wstream *s)
{
    csm_u8 hi, lo;
    lo = csm_wstream_u8(s);
    hi = csm_wstream_u8(s);
    return ((csm_u16) hi << 8) | lo;
}

csm_u32 csm_wstream_u32(csm_wstream *s)
{
    csm_u16 hi, lo;
    lo = csm_wstream_u16(s);
    hi = csm_wstream_u16(s);
    return ((csm_u32) hi << 16) | lo;
}

csm_u64 csm_wstream_u64(csm_wstream *s)
{
    csm_u32 hi, lo;
    lo = csm_wstream_u32(s);
    hi = csm_wstream_u32(s);
    return ((csm_u64) hi << 32) | lo;
}

csm_i8 csm_wstream_i8(csm_wstream *s)
{
    return (csm_i8) csm_wstream_u8(s);
}

csm_i16 csm_wstream_i16(csm_wstream *s)
{
    return (csm_i16) csm_wstream_u16(s);
}

csm_i32 csm_wstream_i32(csm_wstream *s)
{
    return (csm_i32) csm_wstream_u32(s);
}

csm_i64 csm_wstream_i64(csm_wstream *s)
{
    return (csm_i32) csm_wstream_u64(s);
}

csm_f32 csm_wstream_f32(csm_wstream *s)
{
    return (csm_f32) csm_wstream_u32(s);
}

csm_f64 csm_wstream_f64(csm_wstream *s)
{
    return (csm_f64) csm_wstream_u64(s);
}

