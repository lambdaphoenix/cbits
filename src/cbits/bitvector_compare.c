/**
 * @file src/cbits/bitvector_compare.c
 * @brief BitVector comparison and subvector search.
 *
 * This module implements:
 * - \ref bv_equal
 * - \ref bv_contains_subvector
 *
 * @see bitvector_internal.h
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_internal.h"
#include <string.h>

bool
bv_equal(const BitVector *a, const BitVector *b)
{
    if (a == b) {
        return true;
    }
    if (!a || !b) {
        return false;
    }
    if (a->n_bits != b->n_bits) {
        return false;
    }
    if (a->n_bits == 0) {
        return true;
    }
    return memcmp(a->data, b->data, a->n_words * sizeof(uint64_t)) == 0;
}

bool
bv_contains_subvector(const BitVector *a, const BitVector *b)
{
    if (!a || !b) {
        return false;
    }
    if (b->n_bits == 0) {
        return true;
    }
    if (b->n_bits > a->n_bits) {
        return false;
    }

    const size_t max_pos = a->n_bits - b->n_bits;
    const unsigned b_tail = (unsigned) (b->n_bits & 63);
    const uint64_t tail_mask = b_tail ? ((1ULL << b_tail) - 1) : UINT64_MAX;

    if (b->n_bits <= 64) {
        uint64_t needle = b->data[0] & tail_mask;
        for (size_t pos = 0; pos <= max_pos; ++pos) {
            size_t w_off = pos >> 6;
            unsigned b_off = pos & 63;

            uint64_t lo = a->data[w_off];
            uint64_t hi = (w_off + 1 < a->n_words) ? a->data[w_off + 1] : 0ULL;
            uint64_t window = (lo >> b_off) | (hi << (64 - b_off));

            if ((window & tail_mask) == needle) {
                return true;
            }
        }
        return false;
    }

    const size_t b_words = b->n_words;
    for (size_t pos = 0; pos <= max_pos; ++pos) {
        const size_t w_off = pos >> 6;
        const unsigned b_off = pos & 63;

        {
            uint64_t lo = a->data[w_off];
            uint64_t hi = (w_off + 1 < a->n_words) ? a->data[w_off + 1] : 0ULL;
            uint64_t aw0 = (lo >> b_off) | (hi << (64 - b_off));
            uint64_t mask0 = (b_words == 1) ? tail_mask : UINT64_MAX;
            if ((aw0 & mask0) != (b->data[0] & mask0)) {
                continue;
            }
        }

        bool match = true;
        for (size_t j = 1; j < b_words; ++j) {
            uint64_t lo = a->data[w_off + j];
            uint64_t hi =
                (w_off + j + 1 < a->n_words) ? a->data[w_off + j + 1] : 0ULL;
            uint64_t worda = (lo >> b_off) | (hi << (64 - b_off));
            uint64_t mask = (b_words == j + 1) ? tail_mask : UINT64_MAX;

            if ((worda & mask) != (b->data[j] & mask)) {
                match = false;
                break;
            }
        }
        if (match) {
            return true;
        }
    }
    return false;
}
