#include "csm/map.h"
#include "csm/types.h"
#include <assert.h>

#define RANDOM_64BIT_SEED_1 (0xadf7a9e362e789f7LL)
#define RANDOM_64BIT_SEED_2 (0x01cf3b85996c3da0LL)
#define RANDOM_64BIT_SEED_3 (0xb2d0a3c4615c19caLL)

/* See "src/csm-fasthash-copy.c" for copy of fasthash algorithm. */
extern csm_u64 fasthash64(const void* buf, csm_u64 len, csm_u64 seed);
extern csm_u32 fasthash32(const void* buf, csm_u64 len, csm_u64 seed);

int csm_scalar_equals(csm_cell one, csm_cell two) {
    return one.as.u64 == two.as.u64;
}

csm_u64 csm_scalar_hash(csm_cell k) {
    return csm_scalar_fnv1a64(k);
}

csm_u64 csm_string_hash(csm_cell k) {
    (void) k;
    assert("Not implemented yet!" == NULL);
    return 0;
}

int csm_string_equals(csm_cell one, csm_cell two) {
    (void) one;
    (void) two;
    assert("Not implemented yet!" == NULL);
    return 0;
}

csm_u64 csm_scalar_fasthash64(csm_cell k) {
    return fasthash64(&k, sizeof(k), RANDOM_64BIT_SEED_2);
}

csm_u64 csm_scalar_fasthash32(csm_cell k) {
    return fasthash32(&k, sizeof(k), RANDOM_64BIT_SEED_2);
}

static csm_u64 hashdjb2(const void* buf, csm_u64 len) {
    csm_u64 hash = 5381;
    csm_u8 c = 0;
    csm_u64 i = 0;

    for (i = 0; i < len; i++) {
        c = ((csm_u8*) buf)[i];
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

csm_u64 csm_scalar_djb2(csm_cell k) {
    return hashdjb2(&k, sizeof(k));
}

static csm_u64 hashfnv1a64(const void* buf, csm_u64 len, csm_u64 seed)
{
    const csm_u64 fnv_prime = 1099511628211LL;
    csm_u8 *bp = (csm_u8*) buf;
    csm_u8 *be = bp + len;
    csm_u64 hash = seed;

    while (bp < be) {
        csm_u8 byte = *bp++;
        hash ^= byte;
        hash *= fnv_prime;
    }

    return hash;
}

csm_u64 csm_scalar_fnv1a64(csm_cell k) {
    return hashfnv1a64(&k, sizeof(k), RANDOM_64BIT_SEED_1);
}

