#ifndef CSM_NATIVE_MODULE_IO_H_
#define CSM_NATIVE_MODULE_IO_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "csm/types.h"
#include "csm/machine/state.h"


/* Can use to reduce the search space. */
extern const char* csm_native_io_prefix;


/* Search tables. */
extern struct csm_native_method* csm_native_io_methods[];
extern struct csm_native_object* csm_native_io_objects[];


/* Call before execution. */
void csm_native_io_setup(void);
void csm_native_io_teardown(void);


/* csm_io_flush :: Flush the standard output stream. */
void
csm_native_io_flush(struct csm_thread* t);


/* csm_io_print_strconst :: Print string constant. */
void
csm_native_io_print_strconst(struct csm_thread* t);


/* csm_io_print_b :: Print int8. */
void
csm_native_io_print_b(struct csm_thread* t);


/* csm_io_print_s :: Print int16. */
void
csm_native_io_print_s(struct csm_thread* t);


/* csm_io_print_d :: Print int32. */
void
csm_native_io_print_d(struct csm_thread* t);


/* csm_io_print_q :: Print int64. */
void
csm_native_io_print_q(struct csm_thread* t);


/* csm_io_print_f :: Print flt64. */
void
csm_native_io_print_f(struct csm_thread* t);


/* csm_io_print_a :: Print object. */
void
csm_native_io_print_a(struct csm_thread* t);


/* csm_io_print_b_ascii :: Print int8 as ASCII. */
void
csm_native_io_print_b_ascii(struct csm_thread* t);


/* csm_io_print_d_utf8 :: Print int32 as UTF8. */
void
csm_native_io_print_d_utf8(struct csm_thread* t);


/* csm_io_print_arr_b_ascii :: Print array of int8 as ASCII. */
void
csm_native_io_print_arr_b_ascii(struct csm_thread* t);


/* csm_io_print_arr_b_utf8 :: Print array of int8 as UTF8. */
void
csm_native_io_print_arr_b_utf8(struct csm_thread* t);


#ifdef __cplusplus
}
#endif

#endif
