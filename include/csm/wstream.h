#ifndef CSM_WSTREAM_H_
#define CSM_WSTREAM_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>
#include <stdint.h>


/* This is the endianness of the file, not of your system! */
enum csm_wstream_mode {

    CSM_WSTREAM_MODE_LE = 0,
    CSM_WSTREAM_MODE_BE

};


/* Same exact interface as "csm_stream", just in 64 bits. */
struct csm_wstream {
    
    void* buf;
    uint64_t size;
    uint64_t pos;
    int mode;

};


void
csm_wstream_init(struct csm_wstream* s, void *data, uint64_t size, int fe);


void
csm_wstream_restart(struct csm_wstream* s);


void*
csm_wstream_get_buf(struct csm_wstream* s);


void*
csm_wstream_get_adjusted_buf(struct csm_wstream* s);


uint64_t
csm_wstream_get_size(struct csm_wstream* s);


uint64_t
csm_wstream_get_pos(struct csm_wstream* s);


int
csm_wstream_get_mode(struct csm_wstream* s);


int
csm_wstream_lt(struct csm_wstream* s, uint64_t bytes);


/* Returns true if the stream would go out-of-bounds. */
int
csm_wstream_check_rel(struct csm_wstream* s, int64_t adjust);


/* Returns true if the stream would go out-of-bounds. */
int
csm_wstream_check_abs(struct csm_wstream* s, uint64_t adjust);


/* Performs relative (from current position) shift, no check. */
void
csm_wstream_shift_rel(struct csm_wstream* s, int64_t adjust);


/* Performs absolute (from start of stream) shift, no check. */
void
csm_wstream_shift_abs(struct csm_wstream* s, uint64_t adjust);


uint8_t
csm_wstream_u8(struct csm_wstream* s);


uint16_t
csm_wstream_u16(struct csm_wstream* s);


uint32_t
csm_wstream_u32(struct csm_wstream *s);


uint64_t
csm_wstream_u64(struct csm_wstream *s);


int8_t
csm_wstream_i8(struct csm_wstream *s);


int16_t
csm_wstream_i16(struct csm_wstream *s);


int32_t
csm_wstream_i32(struct csm_wstream *s);


int64_t
csm_wstream_i64(struct csm_wstream *s);


float
csm_wstream_f32(struct csm_wstream *s);


double
csm_wstream_f64(struct csm_wstream *s);


#ifdef __cplusplus
}
#endif

#endif