# POSSIBLE ERRORS

- In "csm_bytecode_serialize.c", we make use of malloc and memset. These types
    make use of "size_t", but method/object/string counts can be either u32
    or u64. The width of size_t varies with platform, so we could have
    possible loss of data.

    Obviously, we cannot malloc more than size_t bytes. The issue is in the
    conversion - I'll have to think about what to do, but for now just go
    ahead and make the conversions and note that the issue exists.

    It would be very hard to overflow size_t on a 32/64 bit platform in this
    scenario unless you tried to force it.

