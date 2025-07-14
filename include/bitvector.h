/// @file include/bitvector.h
/// @brief Packed BitVector struct
///
/// This header decleares the BitVector C-API:
///
/// - creation, copy, free
///
/// - bit get/set/clear/flip
///
/// - rank and subvector-containment queries
/// @author lambdaphoenix
/// @copyright 2025 lambdaphoenix
#ifndef CBITS_BITVECTOR_H
#define CBITS_BITVECTOR_H

#include "compat.h"
#include <stdbool.h>

/// @def BV_ALIGN
/// @brief Alignment in bytes for all BitVector allocations.
#define BV_ALIGN 64
/// @def BV_WORDS_SUPER_SHIFT
/// @brief Number of bits to shift a word index to compute a superblock index.
#define BV_WORDS_SUPER_SHIFT 3
/// @def BV_WORDS_SUPER
/// @brief Number of 64-bit worde per superblock.
#define BV_WORDS_SUPER (1u << BV_WORDS_SUPER_SHIFT)

/// @brief Compute the word index for a given bit position.
/// @param pos bit index
/// @return Index of the 64-bit word containing that bit.
static inline size_t
bv_word(const size_t pos)
{
    return pos >> 6;
}
/// @brief Compute the bit offset within its 64-bit word.
/// @param pos bit index
/// @return Offset in [0...63] inside the 64-bit word.
static inline size_t
bv_bit(const size_t pos)
{
    return pos & 63;
}

/// @struct BitVector
/// @brief Packed array of bits with support for rank/select operations.
typedef struct {
    /// @brief Aligned array of 64-bit words storing bits.
    uint64_t *data;
    /// @brief Total number of bits.
    size_t n_bits;
    /// @brief Number of 64-bit words allocated.
    size_t n_words;
    /// @brief Prefix-sum of popcounts at superblock granularity.
    size_t *super_rank;
    /// @brief Prefix-sum of popcounts at block granularity.
    uint16_t *block_rank;
    ///@brief Flag indicating that rank tables need rebuilding.
    bool rank_dirty;
} BitVector;

/// @brief Allocate a new BitVector with all bits cleared
/// @param n_bits Number of bits
/// @return Pointer to the new BitVector
BitVector *
bv_new(size_t n_bits);
/// @brief Make a copy of an existing BitVector.
///
/// The copy shares no memory with the source; all bits and rank tables are
/// reinitialized.
/// @param src Pointer to the source BitVector
/// @return Newly allocated BitVector copy
BitVector *
bv_copy(const BitVector *src);
/// @brief Free all memory associated with a BitVector
/// @param bv Pointer to the BitVector to free
void
bv_free(BitVector *bv);

/// @brief Build or rebuild the rank tables for a BitVector.
///
/// This populates super_rank[] and block_rank[] to support O(1) rank queries.
/// After this call, rank_dirty is cleared.
/// @param bv Pointer to the BitVector whose tables to build
void
bv_build_rank(BitVector *bv);

/// @brief Get the bit value at a given position.
/// @param bv Pointer to the BitVector
/// @param pos Bit index
/// @return 0 or 1 depending on the bit value
static inline int
bv_get(const BitVector *bv, const size_t pos)
{
    return (bv->data[bv_word(pos)] >> bv_bit(pos)) & 1;
}
/// @brief Set the bit at a given position
///
/// Marks the rank table dirty so it will be rebuilt on next rank query.
/// @param bv Pointer to the BitVector
/// @param pos Bit index
static inline void
bv_set(BitVector *bv, const size_t pos)
{
    uint64_t mask = 1ULL << bv_bit(pos);
    cbits_atomic_fetch_or(&bv->data[bv_word(pos)], mask);
    bv->rank_dirty = true;
}
/// @brief Clear the bit at a given position
///
/// Marks the rank table dirty so it will be rebuilt on next rank query.
/// @param bv Pointer to the BitVector
/// @param pos Bit index
static inline void
bv_clear(BitVector *bv, const size_t pos)
{
    uint64_t mask = ~(1ULL << bv_bit(pos));
    cbits_atomic_fetch_and(&bv->data[bv_word(pos)], mask);
    bv->rank_dirty = true;
}
/// @brief Toggle (flip) the bit at a given position
///
/// Marks the rank table dirty so it will be rebuilt on next rank query.
/// @param bv Pointer to the BitVector
/// @param pos Bit index
static inline void
bv_flip(BitVector *bv, const size_t pos)
{
    uint64_t mask = 1ULL << bv_bit(pos);
    cbits_atomic_fetch_xor(&bv->data[bv_word(pos)], mask);
    bv->rank_dirty = true;
}

/// @brief Compute the rank (number of set bits) up to a position.
///
/// If the internal rank tables are dirty, they will be rebuilt.
/// @param bv Pointer to the BitVector
/// @param pos Bit index
/// @return Number of bits set in range [0...pos]
size_t
bv_rank(BitVector *bv, const size_t pos);

/// @brief Test equality of two BitVectors.
///
/// Only vectors with the same length can compare equal.
/// @param a First BitVector
/// @param b Second BitVector
/// @return true if length and all words are identical, false otherwise
bool
bv_equal(const BitVector *a, const BitVector *b);
/// @brief Check weather B appears as a contiguous sub-bitvector of A.
///
/// That is, whether there exists an offset i in A such that A[i..i+|B|-1] ==
/// B[0..|B|-1].
/// @param a Haystack BitVector
/// @param b Needle BitVector
/// @return true if b is contained in a, false otherwise
bool
bv_contains_subvector(const BitVector *a, const BitVector *b);

#endif /* CBITS_BITVECTOR_H */
