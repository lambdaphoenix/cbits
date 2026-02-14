#include <assert.h>
#include <stdio.h>
#include "bitvector.h"

static void
test_concat_basic(void)
{
    BitVector *a = bv_new(5);
    BitVector *b = bv_new(3);

    bv_set(a, 0);
    bv_set(a, 2);
    bv_set(a, 4);

    bv_set(b, 1);

    BitVector *c = bv_concat(a, b);
    assert(c != NULL);
    assert(c->n_bits == 8);

    assert(bv_get(c, 0) == 1);
    assert(bv_get(c, 1) == 0);
    assert(bv_get(c, 2) == 1);
    assert(bv_get(c, 3) == 0);
    assert(bv_get(c, 4) == 1);

    assert(bv_get(c, 5) == 0);
    assert(bv_get(c, 6) == 1);
    assert(bv_get(c, 7) == 0);

    bv_free(a);
    bv_free(b);
    bv_free(c);
}

static void
test_concat_unaligned(void)
{
    BitVector *a = bv_new(70);
    BitVector *b = bv_new(70);

    bv_set(a, 0);
    bv_set(a, 69);

    bv_set(b, 1);
    bv_set(b, 68);

    BitVector *c = bv_concat(a, b);
    assert(c != NULL);
    assert(c->n_bits == 140);

    assert(bv_get(c, 0) == 1);
    assert(bv_get(c, 69) == 1);
    assert(bv_get(c, 70 + 1) == 1);
    assert(bv_get(c, 70 + 68) == 1);

    bv_free(a);
    bv_free(b);
    bv_free(c);
}

static void
test_repeat_basic(void)
{
    BitVector *a = bv_new(4);
    bv_set(a, 0);
    bv_set(a, 3);

    BitVector *r = bv_repeat(a, 3);
    assert(r != NULL);
    assert(r->n_bits == 12);

    int expected[] = {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1};

    for (int i = 0; i < 12; i++) {
        assert(bv_get(r, i) == expected[i]);
    }

    bv_free(a);
    bv_free(r);
}

static void
test_repeat_zero(void)
{
    BitVector *a = bv_new(10);
    BitVector *r = bv_repeat(a, 0);

    assert(r != NULL);
    assert(r->n_bits == 0);

    bv_free(a);
    bv_free(r);
}

int
main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);

    test_concat_basic();
    printf("test_concat_basic: OK\n");

    test_concat_unaligned();
    printf("test_concat_unaligned: OK\n");

    test_repeat_basic();
    printf("test_repeat_basic: OK\n");

    test_repeat_zero();
    printf("test_repeat_zero: OK\n");

    return 0;
}
