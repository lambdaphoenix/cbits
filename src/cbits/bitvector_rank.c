#include "bitvector_internal.h"

void
bv_build_rank(BitVector *bv)
{
    size_t super_total = 0;
    const size_t n_words = bv->n_words;
    const size_t n_super =
        (n_words + BV_WORDS_SUPER - 1) >> BV_WORDS_SUPER_SHIFT;

    for (size_t i = 0; i < n_super; ++i) {
        const size_t base = i << BV_WORDS_SUPER_SHIFT;
        const size_t end =
            base + BV_WORDS_SUPER < n_words ? base + BV_WORDS_SUPER : n_words;

        bv->super_rank[i] = super_total;
        if (end - base == BV_WORDS_SUPER) {
            super_total += cbits_popcount_block(&bv->data[base]);
        }
        else {
            size_t w = base;
            for (; w + 3 < end; w += 4) {
                cbits_prefetch(&bv->data[w + 16]);

                super_total += cbits_popcount64(bv->data[w]);
                super_total += cbits_popcount64(bv->data[w + 1]);
                super_total += cbits_popcount64(bv->data[w + 2]);
                super_total += cbits_popcount64(bv->data[w + 3]);
            }
            for (; w < end; ++w) {
                super_total += cbits_popcount64(bv->data[w]);
            }
        }

        size_t acc = 0;
        size_t w = base;
        for (; w + 3 < end; w += 4) {
            bv->block_rank[w] = (uint16_t) acc;
            acc += cbits_popcount64(bv->data[w]);
            bv->block_rank[w + 1] = (uint16_t) acc;
            acc += cbits_popcount64(bv->data[w + 1]);
            bv->block_rank[w + 2] = (uint16_t) acc;
            acc += cbits_popcount64(bv->data[w + 2]);
            bv->block_rank[w + 3] = (uint16_t) acc;
            acc += cbits_popcount64(bv->data[w + 3]);
        }
        for (; w < end; ++w) {
            bv->block_rank[w] = (uint16_t) acc;
            acc += cbits_popcount64(bv->data[w]);
        }
    }
    bv->rank_dirty = false;
}

size_t
bv_rank(BitVector *bv, const size_t pos)
{
    if (bv->rank_dirty) {
        bv_build_rank(bv);
    }
    size_t w = pos >> 6;
    size_t off = pos & 63;
    size_t s = bv->super_rank[w >> BV_WORDS_SUPER_SHIFT];
    size_t b = bv->block_rank[w];
    uint64_t mask = (1ULL << (off + 1)) - 1;
    uint64_t part = bv->data[w] & mask;
    return s + b + (size_t) cbits_popcount64(part);
}