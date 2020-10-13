/* Taken from the below link. The source file's copyright is below. */
/* COPYRIGHT INCLUDED FROM ORIGINAL UNEDITED SOURCE FILE */
/* https://github.com/ztan-ml/fast-hash */
/* The MIT License

   Copyright (C) 2012 Zilong Tan (eric.zltan@gmail.com)

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use, copy,
   modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#include "csm/types.h"

/* Suppress implicit fallthrough warnings on later versions of GCC. */
#ifdef __GNUC__
    #pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif

#define MAGIC_64BIT_1 (0x2127599bf4325c37ULL)
#define MAGIC_64BIT_2 (0x880355f21e6d1965ULL)

/* Compression function for Merkle-Damgard construction. */
#define MIX_ASSIGN(out__, h__)                                              \
    do {                                                                    \
        (h__) ^= (h__) >> 23;                                               \
        (h__) *= MAGIC_64BIT_1;                                             \
        (h__) ^= (h__) >> 47;                                               \
        (out__) = (h__);                                                    \
    } while (0)

csm_u64 fasthash64(const void *buf, csm_u64 len, csm_u64 seed)
{
    const csm_u64 m = MAGIC_64BIT_2;
    const csm_u64 *pos = (const csm_u64 *) buf;
    const csm_u64 *end = pos + (len / 8);
    const unsigned char *pos2 = NULL;
    csm_u64 h = seed ^ (len * m);
    csm_u64 v = 0;
    csm_u64 mixtmp = 0;

    while (pos != end) {
        v  = *pos++;
        MIX_ASSIGN(mixtmp, v);
        h ^= mixtmp;
        h *= m;
    }

    pos2 = (csm_u8*) pos;
    v = 0;

    switch (len & 7) {
    case 7: v ^= (csm_u64) pos2[6] << 48;
    case 6: v ^= (csm_u64) pos2[5] << 40;
    case 5: v ^= (csm_u64) pos2[4] << 32;
    case 4: v ^= (csm_u64) pos2[3] << 24;
    case 3: v ^= (csm_u64) pos2[2] << 16;
    case 2: v ^= (csm_u64) pos2[1] << 8;
    case 1: v ^= (csm_u64) pos2[0];
        MIX_ASSIGN(mixtmp, v);
        h ^= mixtmp; 
        h *= m;
    }

    MIX_ASSIGN(h, h);

    return h;
} 

csm_u32 fasthash32(const void *buf, size_t len, csm_u32 seed)
{
    csm_u64 h = fasthash64(buf, len, seed);
    return h - (h >> 32);
}

