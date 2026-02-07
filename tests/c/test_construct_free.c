#include <assert.h>
#include <stdio.h>
#include "bitvector.h"

static void
test_construct_free(void)
{
    BitVector *bv = bv_new(0);
    assert(bv != NULL);
    assert(bv->n_bits == 0);
    assert(bv->n_words == 0);
    bv_free(bv);

    for (size_t n = 1; n < 200; n++) {
        bv = bv_new(n);
        assert(bv != NULL);
        assert(bv->n_bits == n);
        assert(bv->n_words == (n + 63) / 64);
        bv_free(bv);
    }
}

int
main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    test_construct_free();
    printf("test_construct_free: OK\n");
    return 0;
}
