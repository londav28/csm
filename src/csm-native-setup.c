#include "csm/native/module/io.h"
#include "csm/native/module/mem.h"
#include "csm/native/module/sys.h"

void csm_native_modules_setup(void)
{
    csm_native_io_setup();   
    csm_native_mem_setup();

    return;
}

void csm_native_modules_teardown(void)
{
    csm_native_io_teardown();
    csm_native_mem_teardown();

    return;
}
