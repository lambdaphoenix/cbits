#include <assert.h>
#include <stdio.h>
#include "bitvector.h"

static void
test_single_bits(void)
{
    BitVector *bv = bv_new(128);

    for (size_t i = 0; i < 128; i++) {
        assert(bv_get(bv, i) == 0);
    }

    bv_set(bv, 5);
    assert(bv_get(bv, 5) == 1);

    bv_clear(bv, 5);
    assert(bv_get(bv, 5) == 0);

    bv_flip(bv, 7);
    assert(bv_get(bv, 7) == 1);
    bv_flip(bv, 7);
    assert(bv_get(bv, 7) == 0);

    bv_free(bv);
}

int
main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    test_single_bits();
    printf("test_single_bit_ops: OK\n");
    return 0;
}
