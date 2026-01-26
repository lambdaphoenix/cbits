/**
 * @file python/binding.c
 * @brief Python C-API bindings for BitVector.
 *
 * Defines the Python-level BitVector type wrapping the C BitVector API,
 * including:
 * - PyBitVector type and lifecycle (tp_new, tp_init, tp_dealloc)
 * - Core BitVector methods (get, set, clear, flip, rank, copy)
 * - Sequence, numeric and richcompare protocols
 *
 * @see include/bitvector.h
 * @author lambdaphoenix
 * @version 0.2.0
 * @copyright Copyright (c) 2025 lambdaphoenix
 */

