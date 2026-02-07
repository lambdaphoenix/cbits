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
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_internal.h"

bool
bv_equal(const BitVector *a, const BitVector *b)
{
    if (a->n_bits != b->n_bits) {
        return false;
    }

    for (size_t i = 0; i < a->n_words; ++i) {
        if (a->data[i] != b->data[i]) {
            return false;
        }
    }
    return true;
}

bool
bv_contains_subvector(const BitVector *A, const BitVector *B)
{
    if (B->n_bits == 0) {
        return true;
    }
    if (B->n_bits > A->n_bits) {
        return false;
    }

    const size_t max_pos = A->n_bits - B->n_bits;
    const unsigned B_tail = (unsigned) (B->n_bits & 63);
    const uint64_t tail_mask = B_tail ? ((1ULL << B_tail) - 1) : UINT64_MAX;

    if (B->n_bits <= 64) {
        uint64_t needle = B->data[0] & tail_mask;
        for (size_t pos = 0; pos <= max_pos; ++pos) {
            size_t w_off = pos >> 6;
            unsigned b_off = pos & 63;

            uint64_t lo = A->data[w_off];
            uint64_t hi = (w_off + 1 < A->n_words) ? A->data[w_off + 1] : 0ULL;
            uint64_t window = (lo >> b_off) | (hi << (64 - b_off));

            if ((window & tail_mask) == needle) {
                return true;
            }
        }
        return false;
    }

    const size_t B_words = B->n_words;
    for (size_t pos = 0; pos <= max_pos; ++pos) {
        const size_t w_off = pos >> 6;
        const unsigned b_off = pos & 63;

        {
            uint64_t lo = A->data[w_off];
            uint64_t hi = (w_off + 1 < A->n_words) ? A->data[w_off + 1] : 0ULL;
            uint64_t aw0 = (lo >> b_off) | (hi << (64 - b_off));
            uint64_t mask0 = (B_words == 1) ? tail_mask : UINT64_MAX;
            if ((aw0 & mask0) != (B->data[0] & mask0)) {
                continue;
            }
        }

        bool match = true;
        for (size_t j = 1; j < B_words; ++j) {
            uint64_t lo = A->data[w_off + j];
            uint64_t hi =
                (w_off + j + 1 < A->n_words) ? A->data[w_off + j + 1] : 0ULL;
            uint64_t wordA = (lo >> b_off) | (hi << (64 - b_off));
            uint64_t mask = (B_words == j + 1) ? tail_mask : UINT64_MAX;

            if ((wordA & mask) != (B->data[j] & mask)) {
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
