# TODO

- Create two types of datastack cells (one with label, one without), as well
    as two different stack frame types (one for each).

- Create a routine which allocates the appropriate stack buffers depending
    on which one you want (IE, if I select a set of dynamic handlers, then
    I need the one with type labels).

- Create a set of macros / functions used by native methods that manage
    the stack appropriately regardless of whether dynamic type labels
    are used or not.

- Wrap up "stdint.h" types in our own typedefs, that way if we ever want
    to remove / replace them we can do so in a controlled manner.

- Rename "csm_data_cell" to "csm_type_cell" and move the union inside it
    to global scope - this becomes the new "csm_data_cell".

^ Write new frame macros to manage either datatype.

^ Cretae new macros/functions in "machine/frame.h" to manage both.

- Test a bytecode call with 0 arguments that returns void.

- Test a bytecode call with 0 arguments that returns non-void.

- Test a bytecode call with 2 arguments that returns void.

- Test a bytecode call with 2 arguments that returns non-void.

- Change SF layout/CC so that locals index negatively (downwards) from
    local_start, and function arguments are pushed from left to right,
    <pre>
    (-0) Arg 0 -> Local 0
    (-1) Arg 1 -> Local 1
    (-2) Arg 2 -> Local 2
    ...
    </pre>

## DONE

@ 01.01.2019
- Rename "csm_stream" functions to get rid of the "_r_" crap.

@01.01.2019
- Refactor to use shared library and "unit binary" compilation model.

@01.01.2019
- Rename "machine/gc.h" to "machine/memory.h".
^ Made a new header folder, split into "base/gc/untracked.h"

@01.01.2019
- Renamed bytecode container structs as "csm_bc_(name)" to shorten them.

@01.01.2019
- Unify method descriptors and object descriptors into a single "descriptor"
    object.

