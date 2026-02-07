#include <assert.h>
#include <stdio.h>
#include "bitvector.h"

static void
test_equal(void)
{
    BitVector *a = bv_new(64);
    BitVector *b = bv_new(64);

    assert(bv_equal(a, b));

    bv_set(a, 10);
    assert(!bv_equal(a, b));

    bv_set(b, 10);
    assert(bv_equal(a, b));

    BitVector *c = bv_new(32);
    assert(!bv_equal(a, c));

    bv_free(a);
    bv_free(b);
    bv_free(c);
}

int
main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    test_equal();
    printf("test_equal: OK\n");
    return 0;
}
