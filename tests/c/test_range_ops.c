#include <assert.h>
#include <stdio.h>
#include "bitvector.h"

static void
test_ranges(void)
{
    BitVector *bv = bv_new(200);

    bv_set_range(bv, 10, 20);
    for (size_t i = 0; i < 200; i++) {
        assert(bv_get(bv, i) == (i >= 10 && i < 30));
    }

    bv_clear_range(bv, 15, 10);
    for (size_t i = 15; i < 25; i++) {
        assert(bv_get(bv, i) == 0);
    }

    bv_flip_range(bv, 0, 10);
    for (size_t i = 0; i < 10; i++) {
        assert(bv_get(bv, i) == 1);
    }

    bv_free(bv);
}

int
main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    test_ranges();
    printf("test_range_ops: OK\n");
    return 0;
}
