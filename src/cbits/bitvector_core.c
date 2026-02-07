/**
 * @file src/cbits/bitvector_core.c
 * @brief Core BitVector construction and basic bit operations.
 *
 * This module implements the fundamental BitVector API:
 * - \ref bv_new, \ref bv_copy, \ref bv_free
 * - single bit operations (\ref bv_get, \ref bv_set, \ref bv_clear, \ref
 * bv_flip)
 *
 * @see bitvector_internal.h
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_internal.h"
#include <string.h>

/**
 * @brief Compute how many 64-bit words are needed to store a given number of
 * bits.
 *
 * Internally rounds up: any remainder bits occupy another full word.
 *
 * @param n_bits Number of bits to store.
 * @return Number of 64-bit words required.
 */
static inline size_t
words_for_bits(const size_t n_bits)
{
    return (n_bits + 63) >> 6;
}

BitVector *
bv_new(size_t n_bits)
{
    BitVector *bv = cbits_malloc_aligned(sizeof(BitVector), BV_ALIGN);
    if (!bv) {
        return NULL;
    }
    bv->n_bits = n_bits;
    bv->n_words = words_for_bits(n_bits);
    bv->rank_dirty = true;

    if (n_bits == 0) {
        bv->data = NULL;
        bv->super_rank = NULL;
        bv->block_rank = NULL;
        return bv;
    }

    bv->data = cbits_malloc_aligned(bv->n_words * sizeof(uint64_t), BV_ALIGN);
    if (!bv->data) {
        cbits_free_aligned(bv);
        return NULL;
    }
    memset(bv->data, 0, bv->n_words * sizeof(uint64_t));

    size_t n_super =
        (bv->n_words + BV_WORDS_SUPER - 1) >> BV_WORDS_SUPER_SHIFT;
    bv->super_rank = cbits_malloc_aligned(n_super * sizeof(size_t), BV_ALIGN);
    if (!bv->super_rank) {
        cbits_free_aligned(bv->data);
        cbits_free_aligned(bv);
        return NULL;
    }
    bv->block_rank =
        cbits_malloc_aligned(bv->n_words * sizeof(uint16_t), BV_ALIGN);
    if (!bv->block_rank) {
        cbits_free_aligned(bv->super_rank);
        cbits_free_aligned(bv->data);
        cbits_free_aligned(bv);
        return NULL;
    }
    return bv;
}

BitVector *
bv_copy(const BitVector *src)
{
    if (!src) {
        return NULL;
    }

    BitVector *dst = bv_new(src->n_bits);
    if (!dst) {
        return NULL;
    }

    if (src->n_bits == 0) {
        dst->rank_dirty = src->rank_dirty;
        return dst;
    }

    memcpy(dst->data, src->data, src->n_words * sizeof(uint64_t));
    bv_apply_tail_mask(dst);
    return dst;
}

void
bv_free(BitVector *bv)
{
    if (!bv) {
        return;
    }
    cbits_free_aligned(bv->block_rank);
    cbits_free_aligned(bv->super_rank);
    cbits_free_aligned(bv->data);
    cbits_free_aligned(bv);
}

int
bv_get(const BitVector *bv, const size_t pos)
{
    if (!bv || pos >= bv->n_bits) {
        return -1;
    }
    return bv__get_inline(bv, pos);
}
void
bv_set(BitVector *bv, const size_t pos)
{
    if (!bv || pos >= bv->n_bits) {
        return;
    }
    bv__set_inline(bv, pos);
}

void
bv_clear(BitVector *bv, const size_t pos)
{
    if (!bv || pos >= bv->n_bits) {
        return;
    }
    bv__clear_inline(bv, pos);
}

void
bv_flip(BitVector *bv, const size_t pos)
{
    if (!bv || pos >= bv->n_bits) {
        return;
    }
    bv__flip_inline(bv, pos);
}
