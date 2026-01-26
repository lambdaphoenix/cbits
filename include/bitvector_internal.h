#ifndef CBITS_BITVECTOR_INTERNAL_H
#define CBITS_BITVECTOR_INTERNAL_H

#include "bitvector.h"

/**
 * @brief Compute the word index for a given bit position.
 * @param pos bit index
 * @return Index of the 64-bit word containing that bit.
 */
static inline size_t
bv_word(const size_t pos)
{
    return pos >> 6;
}
/**
 * @brief Compute the bit offset within its 64-bit word.
 * @param pos bit index
 * @return Offset in [0...63] inside the 64-bit word.
 */
static inline size_t
bv_bit(const size_t pos)
{
    return pos & 63;
}

/**
 *  @brief Internal inline version of bv_get().
 *  @param bv Pointer to the BitVector
 *  @param pos Bit index
 *  @return @c 0 or @c 1 depending on the bit value
 *  @since 0.3.0 
 */
static inline int
bv__get_inline(const BitVector *bv, const size_t pos)
{
    return (bv->data[bv_word(pos)] >> bv_bit(pos)) & 1;
}
/**
 *  @brief Internal inline version of bv_set().
 *  @param bv Pointer to the BitVector
 *  @param pos Bit index
 *  @since 0.3.0 
 */
static inline void
bv__set_inline(BitVector *bv, const size_t pos)
{
    uint64_t mask = 1ULL << bv_bit(pos);
    bv->data[bv_word(pos)] |= mask;
    bv->rank_dirty = true;
}
/**
 *  @brief Internal inline version of bv_clear().
 *  @param bv Pointer to the BitVector
 *  @param pos Bit index
 *  @since 0.3.0 
 */
static inline void
bv__clear_inline(BitVector *bv, const size_t pos)
{
    uint64_t mask = ~(1ULL << bv_bit(pos));
    bv->data[bv_word(pos)] &= mask;
    bv->rank_dirty = true;
}
/**
 *  @brief Internal inline version of bv_flip().
 *  @param bv Pointer to the BitVector
 *  @param pos Bit index
 *  @since 0.3.0 
 */
static inline void
bv__flip_inline(BitVector *bv, const size_t pos)
{
    uint64_t mask = 1ULL << bv_bit(pos);
    bv->data[bv_word(pos)] ^= mask;
    bv->rank_dirty = true;
}

/**
 * @brief Mask off any excess bits in the last word of a BitVector.
 * @param bv Pointer to an allocated BitVector.
 * @since 0.1.2
 */
static inline void
bv_apply_tail_mask(BitVector *bv)
{
    if (!bv->n_words) {
        return;
    }
    unsigned tail = (unsigned) (bv->n_bits & 63);
    if (tail) {
        uint64_t mask = (UINT64_C(1) << tail) - 1;
        bv->data[bv->n_words - 1] &= mask;
    }
}

#endif /* CBITS_BITVECTOR_INTERNAL_H */