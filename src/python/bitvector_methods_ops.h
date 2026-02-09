/**
 * @file src/python/bitvector_methods_ops.h
 * @brief Bitwise operations for BitVector.
 *
 * Declares:
 * - __and__, __or__, __xor__, __invert__
 * - in-place variants
 *
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_OPS_H
#define CBITS_PY_BITVECTOR_METHODS_OPS_H

#include "bitvector_object.h"

/**
 * @brief __and__(BitVector, BitVector) → BitVector.
 * @param oA Left operand.
 * @param oB Right operand.
 * @return New BitVector representing bitwise AND; NULL on error.
 */
PyObject *
py_bitvector_and(PyObject *oA, PyObject *oB);
/**
 * @brief __iand__(BitVector, BitVector) in-place AND.
 * @param self Left operand (modified in place).
 * @param arg Right operand.
 * @return Self on success, NULL on error.
 */
PyObject *
py_bitvector_iand(PyObject *self, PyObject *arg);
/**
 * @brief __or__(BitVector, BitVector) → BitVector.
 * @param oA Left operand.
 * @param oB Right operand.
 * @return New BitVector representing bitwise OR; NULL on error.
 */
PyObject *
py_bitvector_or(PyObject *oA, PyObject *oB);
/**
 * @brief __ior__(BitVector, BitVector) in-place OR.
 * @param self Left operand (modified in place).
 * @param arg Right operand.
 * @return Self on success, NULL on error.
 */
PyObject *
py_bitvector_ior(PyObject *self, PyObject *arg);
/**
 * @brief __xor__(BitVector, BitVector) → BitVector.
 * @param oA Left operand.
 * @param oB Right operand.
 * @return New BitVector representing bitwise XOR; NULL on error.
 */
PyObject *
py_bitvector_xor(PyObject *oA, PyObject *oB);
/**
 * @brief __ixor__(BitVector, BitVector) in-place XOR.
 * @param self Left operand (modified in place).
 * @param arg Right operand.
 * @return Self on success, NULL on error.
 */
PyObject *
py_bitvector_ixor(PyObject *self, PyObject *arg);
/**
 * @brief __invert__(BitVector) → BitVector.
 * @param self A Python PyBitVectorObject instance.
 * @return New BitVector instance with all bits toggled, NULL on error;
 */
PyObject *
py_bitvector_invert(PyObject *self);
/**
 * @brief __bool__(BitVector) → boolean.
 * @param self A Python PyBitVectorObject instance.
 * @return 1 if any bit is set, 0 otherwise
 */
int
py_bitvector_bool(PyObject *self);

#endif /* CBITS_PY_BITVECTOR_METHODS_OPS_H */
