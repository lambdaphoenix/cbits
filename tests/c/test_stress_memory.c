#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bitvector.h"
#include "bitvector_internal.h"

static void stress_alloc_free(void) {
    for (int i = 0; i < 5000; i++) {
        size_t n = (rand() % 5000) + 1;
        BitVector *bv = bv_new(n);
        assert(bv != NULL);

        bv_set_range(bv, 0, n);
        bv_clear_range(bv, 0, n);

        bv_free(bv);
    }
}

static void stress_random_ops(void) {
    BitVector *bv = bv_new(4096);

    for (int i = 0; i < 20000; i++) {
        size_t pos = rand() % 4096;

        switch (rand() % 4) {
            case 0: bv_set(bv, pos); break;
            case 1: bv_clear(bv, pos); break;
            case 2: bv_flip(bv, pos); break;
            case 3: {
                size_t start = rand() % 4096;
                size_t len = rand() % (4096 - start);
                bv_set_range(bv, start, len);
                break;
            }
        }

        size_t r = bv_rank(bv, pos);
        assert(r <= pos + 1);
    }

    bv_free(bv);
}

static void stress_copy_free(void) {
    BitVector *a = bv_new(1024);
    bv_set_range(a, 100, 200);

    for (int i = 0; i < 2000; i++) {
        BitVector *b = bv_copy(a);
        assert(bv_equal(a, b));
        bv_free(b);
    }

    bv_free(a);
}

static void stress_subvector(void) {
    BitVector *a = bv_new(2048);
    BitVector *b = bv_new(64);

    for (int i = 0; i < 2048; i++)
        if (rand() & 1) bv_set(a, i);

    for (int i = 0; i < 64; i++)
        if (rand() & 1) bv_set(b, i);

    bv_contains_subvector(a, b);

    bv_free(a);
    bv_free(b);
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    srand((unsigned)time(NULL));

    stress_alloc_free();
    stress_random_ops();
    stress_copy_free();
    stress_subvector();

    printf("test_stress_memory: OK\n");
    return 0;
}
