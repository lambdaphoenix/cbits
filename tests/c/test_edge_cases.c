#include <assert.h>
#include <stdio.h>
#include "bitvector.h"
#include "bitvector_internal.h"

static void
test_zero_length(void)
{
    BitVector *bv = bv_new(0);
    assert(bv != NULL);
    assert(bv_rank(bv, 0) == 0);
    bv_free(bv);
}

static void
test_tail_mask(void)
{
    BitVector *bv = bv_new(70);
    bv_set_range(bv, 0, 70);
    bv_apply_tail_mask(bv);

    for (size_t i = 64; i < 70; i++) {
        assert(bv_get(bv, i) == 1);
    }

    uint64_t last = bv->data[bv->n_words - 1];
    assert((last >> 6) == 0);

    bv_free(bv);
}

static void
test_copy_independence(void)
{
    BitVector *a = bv_new(64);
    bv_set_range(a, 10, 10);

    BitVector *b = bv_copy(a);
    assert(bv_equal(a, b));

    bv_clear(a, 12);
    assert(!bv_equal(a, b));

    bv_free(a);
    bv_free(b);
}

int
main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    test_zero_length();
    test_tail_mask();
    test_copy_independence();
    printf("test_edge_cases: OK\n");
    return 0;
}
