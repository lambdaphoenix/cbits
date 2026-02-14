/**
 * @file bitvector.h
 * @brief Public C API for the BitVector data structure.
 *
 * Declares the stable, external-facing API for working with BitVectors:
 * - construction and destruction (@ref bv_new, @ref bv_copy, @ref bv_free)
 * - single-bit operations (@ref bv_get, @ref bv_set, @ref bv_clear, @ref
 * bv_flip)
 * - range operations (@ref bv_set_range, @ref bv_clear_range, @ref
 * bv_flip_range)
 * - rank queries (@ref bv_build_rank, @ref bv_rank)
 * - comparison and subvector search (@ref bv_equal, @ref
 * bv_contains_subvector)
 *
 * The public API is intentionally minimal. Internal helpers, inline
 * fast‑paths, and low‑level utilities are defined separately in @ref
 * bitvector_internal.h.
 *
 * BitVector provides a compact, cache‑friendly bit array with optional
 * auxiliary rank tables enabling O(1) prefix‑popcount queries.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_BITVECTOR_H
#define CBITS_BITVECTOR_H

#include <stdbool.h>
#include "compat.h"

/**
 * @def BV_ALIGN
 * @brief Alignment (in bytes) for all BitVector allocations.
 *
 * Ensures that the underlying word array is aligned for efficient SIMD and
 * cache‑line access.
 */
#define BV_ALIGN 64
/**
 * @def BV_WORDS_SUPER_SHIFT
 * @brief Log2 of the number of 64‑bit words per superblock.
 *
 * Used to compute superblock indices via bit‑shifts rather than division.
 */
#define BV_WORDS_SUPER_SHIFT 3
/**
 * @def BV_WORDS_SUPER
 * @brief Number of 64-bit worde in a superblock.
 */
#define BV_WORDS_SUPER (1u << BV_WORDS_SUPER_SHIFT)

/**
 * @brief Packed bit array with rank-support structures.
 *
 * Stores bits in an aligned array of 64‑bit words and maintains auxiliary
 * superblock‑ and block‑level prefix popcount tables for constant‑time rank
 * queries.
 */
typedef struct {
    uint64_t *data;       /**< Aligned array of 64-bit words storing bits. */
    size_t n_bits;        /**< Total number of bits. */
    size_t n_words;       /**< Number of 64-bit words allocated in @c data.*/
    size_t *super_rank;   /**< Superblock-level prefix popcounts. */
    uint16_t *block_rank; /**< Block-level prefix popcunts. */
    bool rank_dirty;      /**< Indicates rank tables must be rebuilt. */
} BitVector;

/**
 * @brief Allocate a new BitVector with all bits cleared.
 * @param n_bits Number of bits to allocate.
 * @retval BitVector* Newly allocated BitVector.
 * @retval NULL Allocation failure.
 */
BitVector *
bv_new(size_t n_bits);
/**
 * @brief Make a copy of an existing BitVector.
 *
 * The copy shares no memory with the source; all bits and rank tables are
 * reinitialized.
 * @param src Pointer to the source BitVector
 * @retval BitVector* Newly allocated BitVector copy.
 * @retval NULL Failure.
 */
BitVector *
bv_copy(const BitVector *src);
/**
 * @brief Free all memory associated with a BitVector
 * @param bv Pointer to the BitVector to free
 */
void
bv_free(BitVector *bv);

/**
 * @brief Set all bits in the half-open range [start, start+len).
 *
 * Marks the rank table dirty so it will be rebuilt on next rank query.
 * @param bv Pointer to the BitVector
 * @param start Start bit index
 * @param len Number of bits to set
 * @since 0.2.0
 */
void
bv_set_range(BitVector *bv, size_t start, size_t len);
/**
 * @brief Clear all bits in the half-open range [start, start+len).
 *
 * Marks the rank table dirty so it will be rebuilt on next rank query.
 * @param bv Pointer to the BitVector
 * @param start Start bit index
 * @param len Number of bits to clear
 * @since 0.2.0
 */
void
bv_clear_range(BitVector *bv, size_t start, size_t len);
/**
 * @brief Toggle (flip) all bits in the half-open range [start, start+len).
 *
 * Marks the rank table dirty so it will be rebuilt on next rank query.
 * @param bv Pointer to the BitVector
 * @param start Start bit index
 * @param len Number of bits to flip
 * @since 0.2.0
 */
void
bv_flip_range(BitVector *bv, size_t start, size_t len);

/**
 * @brief Build or rebuild the rank tables for a BitVector.
 *
 * This populates @c super_rank[] and @c block_rank[] to support O(1) rank
 * queries. After this call, @c bv->rank_dirty is cleared.
 * @param bv Pointer to the BitVector whose tables to build
 */
void
bv_build_rank(BitVector *bv);
/**
 * @brief Compute the rank (number of set bits) up to a position.
 *
 * If the internal rank tables are dirty, they will be rebuilt.
 * @param bv Pointer to the BitVector
 * @param pos Bit index
 * @return Number of bits set in range @c [0...pos)
 */
size_t
bv_rank(BitVector *bv, const size_t pos);

/**
 * @brief Test equality of two BitVectors.
 *
 * Only vectors with the same length can compare equal.
 * @param a First BitVector
 * @param b Second BitVector
 * @return @c true if length and all words are identical, @c false otherwise
 */
bool
bv_equal(const BitVector *a, const BitVector *b);
/**
 * @brief Check weather B appears as a contiguous sub-bitvector of A.
 *
 * That is, whether there exists an offset i in A such that <tt> A[i..i+|B|-1]
 * == B[0..|B|-1] </tt>.
 * @param a Haystack BitVector
 * @param b Needle BitVector
 * @return @c true if b is contained in a, @c false otherwise
 */
bool
bv_contains_subvector(const BitVector *a, const BitVector *b);

/**
 * @brief Get the bit value at a given position.
 * @param bv Pointer to the BitVector
 * @param pos Bit index
 * @return @c 0 or @c 1 depending on the bit value
 */
int
bv_get(const BitVector *bv, const size_t pos);
/**
 * @brief Set the bit at a given position (set to 1)
 *
 * Marks the rank table dirty so it will be rebuilt on next rank query.
 * @param bv Pointer to the BitVector
 * @param pos Bit index
 */
void
bv_set(BitVector *bv, const size_t pos);
/**
 * @brief Clear the bit at a given position (set to 0)
 *
 * Marks the rank table dirty so it will be rebuilt on next rank query.
 * @param bv Pointer to the BitVector
 * @param pos Bit index
 */
void
bv_clear(BitVector *bv, const size_t pos);
/**
 * @brief Toggle (flip) the bit at a given position
 *
 * Marks the rank table dirty so it will be rebuilt on next rank query.
 * @param bv Pointer to the BitVector
 * @param pos Bit index
 */
void
bv_flip(BitVector *bv, const size_t pos);
/**
 * @brief Concatenate two BitVectors into a new BitVector.
 *
 * Produces a new BitVector whose bits are ``A`` followed by ``B``. Returns
 * ``NULL`` on allocation failure.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @retval object New BitVector on success.
 * @retval NULL on failure.
 * @since 0.3.0
 */
BitVector *
bv_concat(const BitVector *a, const BitVector *b);
/**
 * @brief Repeat a BitVector ``n`` times.
 *
 * Produces a new BitVector whose bit pattern is the original repeated
 * ``count`` times. Returns ``NULL`` on allocation failure.
 *
 * @param bv BitVector instance.
 * @param count Repeat count.
 * @retval object New BitVector on success.
 * @retval NULL on error (exception set).
 * @since 0.3.0
 */
BitVector *
bv_repeat(const BitVector *bv, const size_t count);

#endif /* CBITS_BITVECTOR_H */
