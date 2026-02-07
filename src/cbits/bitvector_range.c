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

void
bv_set_range(BitVector *bv, size_t start, size_t len)
{
    if (len == 0) {
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
    if (len == 0) {
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
    if (len == 0) {
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
