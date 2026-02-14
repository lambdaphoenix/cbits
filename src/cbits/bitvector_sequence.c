/**
 * @file bitvector_sequence.c
 * @brief Sequence operations for BitVector.
 *
 * This implements concatenation and repetition of BitVectors. Both operations
 * work at the word level and use shift‑merge logic to support arbitrary bit
 * offsets without falling back to per‑bit loops.
 *
 * All functions assume that BitVector instances allocate one extra 64‑bit word
 * beyond the logical word count (n_words + 1). This ensures that unaligned
 * shift‑merge operations never write out of bounds.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */

#include "bitvector_internal.h"
#include <string.h>

/**
 * @brief Copy the bits of @p src into @p dst at a given bit offset.
 *
 * Performs a fast word‑wise copy. If @p dst_bit_offset is not 64‑bit aligned,
 * the function merges words using left/right shifts.
 *
 * @param src Source BitVector.
 * @param dst Destination BitVector.
 * @param dst_bit_offset Bit offest in @p dst where copying begins.
 * @since 0.3.0
 * @note Sizes are not checked here.
 */
static inline void
bv_copy_bits(const BitVector *src, BitVector *dst, size_t dst_bit_offset)
{
    size_t src_words = src->n_words;

    size_t dst_word_offset = bv_word(dst_bit_offset);
    size_t bit_offset = bv_bit(dst_bit_offset);

    if (bit_offset == 0) {
        memcpy(dst->data + dst_word_offset, src->data,
               src_words * sizeof(uint64_t));
        return;
    }
    for (size_t i = 0; i < src_words; i++) {
        uint64_t w = src->data[i];
        dst->data[dst_word_offset + i] |= (w << bit_offset);
        dst->data[dst_word_offset + i + 1] |= (w >> (64 - bit_offset));
    }
}

BitVector *
bv_concat(const BitVector *a, const BitVector *b)
{
    const size_t n_bits_a = a->n_bits;
    const size_t total_bits = n_bits_a + b->n_bits;

    BitVector *res = bv_new(total_bits);
    if (!res) {
        return NULL;
    }

    bv_copy_bits(a, res, 0);
    bv_copy_bits(b, res, n_bits_a);

    res->rank_dirty = true;
    return res;
}
BitVector *
bv_repeat(const BitVector *bv, const size_t count)
{
    if (count == 0) {
        return bv_new(0);
    }

    const size_t n_bits = bv->n_bits;
    const size_t total_bits = n_bits * count;

    BitVector *res = bv_new(total_bits);
    if (!res) {
        return NULL;
    }

    const size_t words = bv->n_words;
    size_t dst_bit = 0;
    for (size_t r = 0; r < count; r++) {
        bv_copy_bits(bv, res, dst_bit);
        dst_bit += n_bits;
    }
    bv_apply_tail_mask(res);
    res->rank_dirty = true;
    return res;
}
