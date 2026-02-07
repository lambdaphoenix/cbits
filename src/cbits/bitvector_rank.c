/**
 * @file src/cbits/bitvector_rank.c
 * @brief Rank table construction and rank queries.
 *
 * This module implements:
 * - \ref bv_build_rank
 * - \ref bv_rank
 *
 * This module isolates the rank subsystem from the core BitVector logic and
 * integrates with the popcount dispatch mechanism provided by \ref compat.h.
 *
 * @see compat.h
 * @see bitvector_internal.h
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_internal.h"

void
bv_build_rank(BitVector *bv)
{
    if (!bv) {
        return;
    }
    if (bv->n_bits == 0) {
        bv->rank_dirty = false;
        return;
    }

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
    if (!bv || bv->n_bits == 0) {
        return 0;
    }
    size_t p = pos;
    if (pos >= bv->n_bits) {
        p = bv->n_bits - 1;
    }

    if (bv->rank_dirty) {
        bv_build_rank(bv);
    }

    const size_t word_index = bv_word(p);
    const size_t bit_index = bv_bit(p);

    const size_t super_index = word_index >> BV_WORDS_SUPER_SHIFT;
    const size_t base = bv->super_rank[super_index];
    const size_t block = bv->block_rank[word_index];

    const uint64_t word = bv->data[word_index];
    const uint64_t mask =
        (bit_index == 63) ? ~0ULL : ((1ULL << (bit_index + 1)) - 1);
    const size_t in_word = cbits_popcount64(word & mask);
    return base + block + in_word;
}
