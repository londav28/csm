



Example callstack, datastack for thread T below:

<pre>
----------------------
[FRAME              0]
[FRAME              1]
[FRAME              2]
----------------------
|     FRAME N        |
| local N            |
| local 1            |
| local 0 <--- Pointer "local_start" for frame N of thread T      
|--------------------|
| stack 0            |
| stack 1            |
| stack 2            |
| stack 3 <--- Pointer "datastack_pos" for thread T
----------------------
| unused stack       |
| ...                |
----------------------
</pre>
______________________________________________________________________________

Let's say we wanted to make the following function call:

{ .name = "foo_method", .params = 3, .locals = 4, .stacks = 3 }

First, we need to pop 3 parameters:

<pre>
----------------------
[FRAME              0]
[FRAME              1]
[FRAME              2]
----------------------
|     FRAME N        |
| local N            |
| local 1            |
| local 0 <--- Pointer "local_start" for frame N of thread T      
|--------------------|
| stack 0 <--- Pointer "datastack_pos" for thread T

(param 0, param 1, param 2)
</pre>
_____________________________________________________________________________

BUT! Before we can do this, we need to calculate the "local_start" for the
current frame so we have a place to copy them to. Copying parameters is an
unfortunate side effect of the current layout. This is because the semantics
of the VM expect any parameters to be stored as local values.

So to compute the "local_start" of the new frame (N + 1) of thread T:

<pre>
----------------------
[FRAME              0]
[FRAME              1]
[FRAME              2]
----------------------
|     FRAME N        |
| local N            |
| local 1            |
| local 0 <--- Pointer "local_start" for frame N of thread T      
|--------------------|
| stack 0            |
| stack 1            |
| stack 2            |
| stack 3 <--- Pointer "datastack_pos" for thread T
----------------------
| unused stack       |
| ...                |
----------------------
</pre>

There are 3 parameters, we know this from the function's metadata. So to avoid
"bubbles" between frames, we can compute the top of the frame by:

"new_top_of_frame" = "datastack_pos" + .params

Now we've got the top of the frame. Notice that "new_top_of_frame" is the
address of "stack 0", the now top of stack of the last frame.

To get the "local_start", all we have to do is subtract the local count, which
moves the frame pointer down.

"new_local_start" = "new_top_of_frame" - (.locals + .params)

<pre>
----------------------
[FRAME              0]
[FRAME              1]
[FRAME              2]
----------------------
|     FRAME N        |
| local N            |
| local 1            |
| local 0 <--- Pointer "local_start" for frame N of thread T      
|--------------------|
| stack 0 <-- "new_top_of_frame" pointer used in calculation
|--------------------|
|     FRAME N+1      |
| unset 3            |
| unset 2            |
| unset 1 <--- REMEMBER, "datastack_pos" is still set here
| unset 0 <--- Pointer "new_local_start" for N+1
----------------------
| unused stack       |
| ...                |
----------------------
</pre>
_____________________________________________________________________________

Now, the last thing to do is pop and store the method parameters into local
variables for the new frame.

Luckily, the thread's "datastack_pos" pointer is still set to view the top of
stack for the previous frame. Logically, "datastack_pos" is sitting in locals
of the new frame. However, the 3 parameters values on the previous frame's
operand stack haven't been popped off yet, even though we've made space
around them.

A simple for loop that starts at "datastack_pos" and copies the values into
indices at "new_local_start" is all we need.

Finally, we'd set "datastack_pos" to the value of "local_start" for the new
frame. It will be moved down the moment we push a new operand on the stack.

<pre>
----------------------
[FRAME              0]
[FRAME              1]
[FRAME              2]
----------------------
|     FRAME N        |
| local N            |
| local 1            |
| local 0 <--- Pointer "local_start" for frame N of thread T      
|--------------------|
| stack 0            |
|--------------------|
|     FRAME N+1      |
| unset 3            |
| param 2            |
| param 1            |
| param 0 <--- Pointer "new_local_start" for N+1 / "datastack_pos"
----------------------
| unused stack       |
| ...                |
----------------------
</pre>

And that's it! We're done.

A simple algorithm might look like the following:

<pre>
void new_frame_prepare(uint8_t params, uint8_t locals, struct csm_thread* t)
{
    union csm_data_cell* new_top_of_frame;
    uint16_t i;

    /* Move the callstack down one (maybe check for overflow). */
    --CSM_CUR_FRAME(t);

    /* To get the new top of frame, add params to datastack pointer. */
    new_top_of_frame = CSM_DATASTACK_POS(t) + params;

    /* Set local start for current frame by subtacting space from top! */
    CSM_CUR_LOCALS(t) = new_top_of_frame - (locals + params);

    /* Copy in parameters to locals starting at local 0! */
    /* This is why we push parameters from right to left! */
    for (i = 0; i < params; i++) {
        CSM_CUR_LOCALS(t)[i] = CSM_DATASTACK_POS(t)[i];
    }

    /* Properly align datastack pointer. */
    CSM_DATASTACK_POS(t) = CSM_CUR_LOCALS(t);
}
</pre>