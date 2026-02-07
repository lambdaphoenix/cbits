#include <assert.h>
#include <stdio.h>
#include "bitvector.h"

static void
test_contains(void)
{
    BitVector *a = bv_new(64);
    BitVector *b = bv_new(5);
    BitVector *c = bv_new(4);

    bv_set_range(a, 10, 4);  // bits 10–13 = 1
    bv_set_range(b, 0, 5);   // bits 0–4 = 1
    bv_set_range(c, 0, 4);   // bits 0–3 = 1

    assert(!bv_contains_subvector(a, b));
    assert(bv_contains_subvector(a, c));

    bv_clear(a, 12);
    assert(!bv_contains_subvector(a, c));

    bv_free(a);
    bv_free(b);
    bv_free(c);
}

int
main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    test_contains();
    printf("test_contains_subvector: OK\n");
    return 0;
}
