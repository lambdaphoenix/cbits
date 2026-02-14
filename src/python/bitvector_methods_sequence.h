/**
 * @file bitvector_methods_sequence.h
 * @brief Sequence protocol helpers for BitVector (concat and repeat).
 *
 * Implements the sq_concat and sq_repeat slots for the BitVector type:
 * - concatenation: A + B
 * - repetition: A * n
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_SEQUENCE_H
#define CBITS_PY_BITVECTOR_METHODS_SEQUENCE_H

#include "bitvector_object.h"

/**
 * @brief Implement ``A + B`` for BitVector objects.
 *
 * Concatenates the bits of two BitVectors into a new BitVector of length
 * ``len(A) + len(B)``. Both operands must be BitVector instances; otherwise
 * ``NotImplemented`` is returned.
 *
 * @param oA Left operand.
 * @param oB Right operand.
 * @retval object New BitVector on success.
 * @retval Py_NotImplemented for unsupported operand types.
 * @retval NULL on failure (exception set).
 * @since 0.3.0
 */
PyObject *
py_bitvector_concat(PyObject *oA, PyObject *oB);
/**
 * @brief Implement ``A * n`` (sequence repeat) for BitVector.
 *
 * Returns a new BitVector whose bit pattern is the original BitVector repeated
 * ``n`` times. If ``n <= 0``, an empty BitVector is returned.
 *
 * @param self BitVector instance.
 * @param count Repeat count.
 * @retval object New BitVector on success.
 * @retval NULL on error (exception set).
 * @since 0.3.0
 */
Py_hash_t
py_bitvector_repeat(PyObject *self, Py_ssize_t count);

#endif /* CBITS_PY_BITVECTOR_METHODS_SEQUENCE_H */
