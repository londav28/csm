#include "csm/map.h"
#include "csm/memory/malloc.h"
#include "csm/memset.h"
#include "csm/types.h"
#include <assert.h>

#define CSM_MAP_MAX_LOAD_FACTOR         (0.7)
#define CSM_MAP_TABLE_START_SIZE_PRIME  121
#define CSM_MAP_TABLE_START_SIZE        128
#define CSM_MAP_COLLISION_DIAG          0

#if CSM_MAP_COLLISION_DIAG
    #include <inttypes.h>
    #include <stdio.h>
#endif

struct csm_map_node {

    struct csm_map_node *next;
    csm_cell k;
    csm_cell v;

};

/* It's ok if this isn't that performant. Only called to cache primes. */
static csm_u64 get_next_prime_after(csm_u64 after)
{
    csm_u64 n = 0;

    if (after < 2) {
        return 2;
    }

    n = ((after % 2) == 0) ? (after + 1) : after;

    while (CSM_TRUE) {
        int is_prime = CSM_TRUE;
        csm_u64 i = 0;

        for (i = 3; i < (n/2); i++) {
            if ((n % i) == 0) {
                is_prime = CSM_FALSE;
                break;
            }
        }

        if (is_prime && n > after) {
            return n;
        }

        n += 2;
    }

    return 0;
}

/* This function is zero indexed. It is not thread safe. */
/*
static csm_u64 get_nth_prime(csm_u64 n) {
    static csm_u64 prime_table[CSM_MAP_MAX_PRIMES];
    static csm_u64 uncomputed = 0;

    assert(n < CSM_MAP_MAX_PRIMES);

    if (uncomputed == 0) {
        prime_table[uncomputed] = 2;
        uncomputed++;
    }

    while (n >= uncomputed) {
        csm_u64 last = prime_table [uncomputed - 1];
        csm_u64 prime = get_next_prime_after(last);
        prime_table[uncomputed] = prime;
        uncomputed++;
    }

    return prime_table[n];
}
*/

int csm_map_init(csm_map *out, csm_hash hasher, csm_equals equals)
{
    /* Maybe use primes after tuning. */
    csm_u64 prime = get_next_prime_after(CSM_MAP_TABLE_START_SIZE);
    csm_u64 tablesize = CSM_MAP_TABLE_START_SIZE;

    (void) prime;

    /* Attempt to allocate the table. */
    out->table = csm_malloc(tablesize * sizeof(*(out->table)));

    if (out->table == NULL) {
        return 1;
    }

    /* Clear the table memory. */
    csm_memset(out->table, 0, tablesize * sizeof(*out->table));

    /* Set remaining table fields. */
    out->tablesize = tablesize;
    out->size = 0;
    out->hasher = hasher;
    out->equals = equals;

    return 0;
}

void csm_map_fini(csm_map *m)
{
    struct csm_map_node *node = NULL;
    struct csm_map_node *next = NULL;
    csm_u64 i = 0;

    for (i = 0; i < m->tablesize; i++) {
        node = m->table[i];
        while (node != NULL) {
            next = node->next;
            csm_free(node);
            node = next;
        }
    }

    csm_free(m->table);
    m->table = NULL;

    return;
}

static csm_f64 map_get_load_factor(csm_map *m) {
    return (((csm_f64) m->size) / ((csm_f64) m->tablesize));
}

static csm_u64 map_get_next_tablesize(csm_map *m) {
    /* return get_next_prime_after(m->tablesize * 2); */
    return m->tablesize * 2;
}

static void map_expand_rehash_table(csm_map *m) {
    struct csm_map_node **old_table = m->table;
    csm_u64 old_size = m->tablesize;
    struct csm_map_node *old_node = NULL;
    struct csm_map_node *next = NULL;
    csm_u64 i = 0;

    m->tablesize = map_get_next_tablesize(m);
    m->table = csm_malloc(m->tablesize * sizeof(*m->table));
    assert(m->table != NULL);

    csm_memset(m->table, 0, m->tablesize * sizeof(*m->table));

    for (i = 0; i < old_size; i++) {

        /* Notice that we update via "next". */
        old_node = old_table[i];
        while (old_node != NULL) {
            csm_u64 j = m->hasher(old_node->k) % m->tablesize;
            struct csm_map_node **link= m->table + j;

            /* Store so we can still walk the chain. */
            next = old_node->next;
            old_node->next = NULL;

            while (*link != NULL) {
                link = &(*link)->next;
            }

            *link = old_node;
            old_node = next;
        }
    }

    csm_free(old_table);

    return;
}

int csm_map_get(csm_map *m, csm_cell k, csm_cell *out)
{
    struct csm_map_node *node = NULL;
    int result = CSM_FALSE;
    csm_u64 i = 0;

    /* Compute the start node index and fetch it. */
    i = m->hasher(k) % m->tablesize;
    node = m->table[i];

    for (; node != NULL; node = node->next) {
        result = m->equals(k, node->k);
        if (result && out != NULL) {
            *out = node->v;
            break;
        }
    }

    return result;
}

int csm_map_put(csm_map *m, csm_cell k, csm_cell v, csm_cell *out)
{
    struct csm_map_node **link = NULL;
    int result = CSM_FALSE;
    csm_u64 i = 0;
    csm_f64 load = map_get_load_factor(m);

    if (load >= CSM_MAP_MAX_LOAD_FACTOR) {
        map_expand_rehash_table(m);
    }

    /* Compute the hash node index and fetch it. */
    i = m->hasher(k) % m->tablesize;
    link = m->table + i;

    /*
        What are we doing here is walking through the chain indirectly via
        the _address_ of a given node's "next" pointer. The address of the
        "next" pointer we start with is actually the one contained in the
        table itself. We check a node by dereferencing the link.
        If it's not the one we want, we update the link by taking the
        address of the "next" pointer contained in the node that we just
        inspected.

        A bit mind boggling, but the advantage of this approach is that
        regardless of whether we find a node or not, the link will
        point to the correct "node" field to update (where this might
        be within a node, or in the table itself).
    */
    while (*link != NULL) {
        result = m->equals(k, (*link)->k);
        if (result) {
            if (out != NULL) {
                *out = (*link)->v;
            }

            (*link)->k = k;
            (*link)->v = v;
            break;
        }
        link = &((*link)->next);
    }

    /* No key found, so insert a new node via the link. */
    if (!result) {
        (*link) = csm_malloc(sizeof(struct csm_map_node));
        assert(*link != NULL);
        (*link)->next = NULL;
        (*link)->k = k;
        (*link)->v = v;
        m->size++;
    }

    return result;
}

/* TODO: Shrink the table? */
int csm_map_remove(csm_map *m, csm_cell k, csm_cell *out)
{
    struct csm_map_node **link = NULL;
    int result = CSM_FALSE;
    csm_u64 i = 0;

    /* Compute the hash node index and fetch it. */
    i = m->hasher(k) % m->tablesize;
    link = m->table + i;

    /* Search for the key to remove. */
    while (*link != NULL) {
        result = m->equals(k, (*link)->k);
        if (result && out != NULL) {
            *out = (*link)->v;
            break;
        }
        link = &((*link)->next);
    }

    if (result) {
        struct csm_map_node *node = *link;
        *link = node->next;
        csm_free(node);
        m->size--;
    }

    return result;
}

size_t csm_map_size(csm_map *m)
{
    return m->size;
}

void csm_map_clear(csm_map *m)
{
    struct csm_map_node *node = NULL;
    struct csm_map_node *next = NULL;
    csm_u64 i = 0;

    for (i = 0; i < m->tablesize; i++) {
        node = m->table[i];

        while (node != NULL) {
            next = node->next;
            csm_free(node);
            node = next;
            m->size--;
        }

        m->table[i] = NULL;
    }

    assert(m->size == 0);

    return;
}

