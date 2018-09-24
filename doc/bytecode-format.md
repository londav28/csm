# BYTECODE FORMAT

## DEFAULT

The default bytecode file format.

<pre>

file =
    u8*4        = magic string "csmx"
    module      = file contents

module =
    u8          = status byte 0
    u8          = status byte 1
    u8          = status byte 2
    u8          = status byte 3
    u32         = method count
    [method]    = methods
    u32         = object count
    [object]    = objects
    u32         = string count
    [string]    = strings
    u32         = int64 count
    [i64]       = int64s
    u32         = flt64 count
    [f64]       = flt64s

method =
    u8          = status byte 0
    u8          = status byte 1
    u32         = name string
    u32         = debugging symbol
    u32         = signature block
    u8          = stack limit
    u8          = local limit
    u32         = ins byte count
    [u8]        = ins bytes
    u32         = exception table entry count
    [ete]       = exception table entries

object =
    u8          = status byte 0
    u32         = name string
    u32         = field block

string =
    u32         = string length
    [u8]        = string bytes

ete =
    u32         = index
    u32         = start
    u32         = end
    u32         = target


</pre>
