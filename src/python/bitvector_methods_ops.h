/**
 * @file bitvector_methods_ops.h
 * @brief Bitwise operations for ``BitVector``.
 *
 * Declares the Python bindings for all bitwise operators supported by the
 * BitVector type:
 * - ``__and__``, ``__or__``, ``__xor__``, ``__invert__``
 * - in‑place variants (``__iand__``, ``__ior__``, ``__ixor__``)
 * - truth‑value testing (``__bool__``)
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_OPS_H
#define CBITS_PY_BITVECTOR_METHODS_OPS_H

#include "bitvector_object.h"

/**
 * @brief Implement ``A & B``.
 *
 * Performs a bitwise AND between two BitVectors of equal length.
 *
 * @param oA Left operand.
 * @param oB Right operand.
 * @retval and New ``PyBitVectorObject`` on success.
 * @retval NULL on failure (exception set).
 */
PyObject *
py_bitvector_and(PyObject *oA, PyObject *oB);
/**
 * @brief Implement ``A &= B`` (in‑place AND).
 *
 * Modifies ``self`` in place. Both operands must have equal length.
 *
 * @param self Left operand (modified in place).
 * @param arg Right operand.
 * @retval self on success
 * @retval NULL on failure (exception set).
 */
PyObject *
py_bitvector_iand(PyObject *self, PyObject *arg);
/**
 * @brief Implement ``A | B``.
 *
 * Performs a bitwise OR between two BitVectors of equal length.
 *
 * @param oA Left operand.
 * @param oB Right operand.
 * @retval or New ``PyBitVectorObject`` on success.
 * @retval NULL on failure (exception set). */
PyObject *
py_bitvector_or(PyObject *oA, PyObject *oB);
/**
 * @brief Implement ``A |= B`` (in‑place OR).
 *
 * Modifies ``self`` in place. Both operands must have equal length.
 *
 * @param self Left operand (modified in place).
 * @param arg Right operand.
 * @retval self on success.
 * @retval NULL on failure (exception set).
 */
PyObject *
py_bitvector_ior(PyObject *self, PyObject *arg);
/**
 * @brief Implement ``A ^ B``.
 *
 * Performs a bitwise XOR between two BitVectors of equal length.
 *
 * @param oA Left operand.
 * @param oB Right operand.
 * @retval xor New ``PyBitVectorObject`` on success.
 * @retval NULL on failure (exception set). */
PyObject *
py_bitvector_xor(PyObject *oA, PyObject *oB);
/**
 * @brief Implement ``A ^= B`` (in‑place XOR).
 *
 * Modifies ``self`` in place. Both operands must have equal length.
 *
 * @param self Left operand (modified in place).
 * @param arg Right operand.
 * @retval self on success.
 * @retval NULL on failure (exception set).
 */
PyObject *
py_bitvector_ixor(PyObject *self, PyObject *arg);
/**
 * @brief Implement ``~A``.
 *
 * Returns a new BitVector where every bit of ``self`` is inverted.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @retval invert New ``PyBitVectorObject`` on success.
 * @retval NULL on failure (exception set). */
PyObject *
py_bitvector_invert(PyObject *self);
/**
 * @brief Implement ``bool(BitVector)``.
 *
 * A BitVector is considered true if at least one bit is set.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @retval 1 At least one bit is set.
 * @retval 0 All bits are zero.
 */
int
py_bitvector_bool(PyObject *self);

#endif /* CBITS_PY_BITVECTOR_METHODS_OPS_H */
