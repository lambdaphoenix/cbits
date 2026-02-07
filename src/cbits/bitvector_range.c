/**
 * @file src/cbits/bitvector_range.c
 * @brief Range-based bit operations for BitVector.
 *
 * This module implements:
 * - \ref bv_set_range
 * - \ref bv_clear_range
 * - \ref bv_flip_range
 *
 * These operations manipulate contiguous bit ranges.
 *
 * @see bitvector_internal.h
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_internal.h"

/**
 * @brief Clamp a bit-range to the valid bounds of the BitVector.
 *
 * Adjusts *start and *len so the range lies fully inside the vector:
 * - If the vector is empty or the range is empty, *len becomes 0.
 * - If *start is outside the vector, *len becomes 0.
 * - If the range exceeds the vector end, *len is shortened accordingly.
 *
 * After normalization, the range is guaranteed to satisfy:
 *  0 <= *start <= n_bits
 *  0 <= *len <= n_bits - *start
 *
 * @param bv Pointer to the BitVector
 * @param start Input/output: start index of the range
 * @param len Input/output: length of the range
 * @since 0.2.0
 */
static inline void
bv__normalize_range(BitVector *bv, size_t *start, size_t *len)
{
    if (!bv || bv->n_bits == 0 || *len == 0) {
        *len = 0;
        return;
    }
    if (*start >= bv->n_bits) {
        *len = 0;
        return;
    }
    if (*start + *len > bv->n_bits) {
        *len = bv->n_bits - *start;
    }
}

void
bv_set_range(BitVector *bv, size_t start, size_t len)
{
    bv__normalize_range(bv, &start, &len);
    if (!len) {
        return;
    }
    size_t end = start + len;
    size_t w_start = start >> 6;
    size_t w_end = (end - 1) >> 6;
    unsigned off_start = start & 63;
    unsigned off_end = end & 63;

    if (w_start == w_end) {
        unsigned span = (unsigned) (end - start);
        uint64_t mask = (span == 64)
                            ? UINT64_MAX
                            : ((UINT64_C(1) << span) - 1) << off_start;
        bv->data[w_start] |= mask;
    }
    else {
        bv->data[w_start] |= ~0ULL << off_start;
        for (size_t w = w_start + 1; w < w_end; ++w) {
            bv->data[w] = ~0ULL;
        }
        if (off_end) {
            bv->data[w_end] |= (UINT64_C(1) << off_end) - 1;
        }
        else {
            bv->data[w_end] = ~0ULL;
        }
    }
    bv_apply_tail_mask(bv);
    bv->rank_dirty = true;
}

void
bv_clear_range(BitVector *bv, size_t start, size_t len)
{
    bv__normalize_range(bv, &start, &len);
    if (!len) {
        return;
    }
    size_t end = start + len;
    size_t w_start = start >> 6;
    size_t w_end = (end - 1) >> 6;
    unsigned off_start = start & 63;
    unsigned off_end = end & 63;

    if (w_start == w_end) {
        unsigned span = (unsigned) (end - start);
        uint64_t mask = (span == 64)
                            ? UINT64_MAX
                            : ((UINT64_C(1) << span) - 1) << off_start;
        bv->data[w_start] &= ~mask;
    }
    else {
        bv->data[w_start] |= ~0ULL << off_start;
        for (size_t w = w_start + 1; w < w_end; ++w) {
            bv->data[w] = 0ULL;
        }
        if (off_end) {
            bv->data[w_end] &= ~((UINT64_C(1) << off_end) - 1);
        }
        else {
            bv->data[w_end] = 0ULL;
        }
    }
    bv->rank_dirty = true;
}

void
bv_flip_range(BitVector *bv, size_t start, size_t len)
{
    bv__normalize_range(bv, &start, &len);
    if (!len) {
        return;
    }
    size_t end = start + len;
    size_t w_start = start >> 6;
    size_t w_end = (end - 1) >> 6;
    unsigned off_start = start & 63;
    unsigned off_end = end & 63;

    if (w_start == w_end) {
        unsigned span = (unsigned) (end - start);
        uint64_t mask = (span == 64)
                            ? UINT64_MAX
                            : ((UINT64_C(1) << span) - 1) << off_start;
        bv->data[w_start] ^= mask;
    }
    else {
        bv->data[w_start] ^= ~0ULL << off_start;
        for (size_t w = w_start + 1; w < w_end; ++w) {
            bv->data[w] ^= ~0ULL;
        }
        if (off_end) {
            bv->data[w_end] ^= (UINT64_C(1) << off_end) - 1;
        }
        else {
            bv->data[w_end] ^= ~0ULL;
        }
    }
    bv_apply_tail_mask(bv);
    bv->rank_dirty = true;
}
