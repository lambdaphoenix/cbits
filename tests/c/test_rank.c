#include <assert.h>
#include <stdio.h>
#include "bitvector.h"

static void
test_rank(void)
{
    BitVector *bv = bv_new(100);

    bv_set_range(bv, 0, 50);
    assert(bv->rank_dirty == true);

    assert(bv_rank(bv, 0) == 1);
    assert(bv_rank(bv, 49) == 50);
    assert(bv_rank(bv, 99) == 50);
    assert(bv->rank_dirty == false);

    bv_set(bv, 60);
    assert(bv->rank_dirty == true);
    assert(bv_rank(bv, 99) == 51);

    bv_free(bv);
}

int
main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    test_rank();
    printf("test_rank: OK\n");
    return 0;
}
