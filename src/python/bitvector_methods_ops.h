#ifndef CBITS_PY_BITVECTOR_METHODS_OPS_H
#define CBITS_PY_BITVECTOR_METHODS_OPS_H

#include "bitvector_methods.h"

/**
 * @brief __and__(BitVector, BitVector) → BitVector.
 * @param a Left operand.
 * @param b Right operand.
 * @return New BitVector representing bitwise AND; NULL on error.
 */
PyObject *
py_bv_and(PyObject *oA, PyObject *oB);
/**
 * @brief __iand__(BitVector, BitVector) in-place AND.
 * @param a Left operand (modified in place).
 * @param b Right operand.
 * @return Self on success, NULL on error.
 */
PyObject *
py_bv_iand(PyObject *self, PyObject *arg);
/**
 * @brief __or__(BitVector, BitVector) → BitVector.
 * @param a Left operand.
 * @param b Right operand.
 * @return New BitVector representing bitwise OR; NULL on error.
 */
PyObject *
py_bv_or(PyObject *oA, PyObject *oB);
/**
 * @brief __ior__(BitVector, BitVector) in-place OR.
 * @param a Left operand (modified in place).
 * @param b Right operand.
 * @return Self on success, NULL on error.
 */
PyObject *
py_bv_ior(PyObject *self, PyObject *arg);

/**
 * @brief __xor__(BitVector, BitVector) → BitVector.
 * @param a Left operand.
 * @param b Right operand.
 * @return New BitVector representing bitwise XOR; NULL on error.
 */
PyObject *
py_bv_xor(PyObject *oA, PyObject *oB);

/**
 * @brief __ixor__(BitVector, BitVector) in-place XOR.
 * @param a Left operand (modified in place).
 * @param b Right operand.
 * @return Self on success, NULL on error.
 */
PyObject *
py_bv_ixor(PyObject *self, PyObject *arg);
/**
 * @brief __invert__(BitVector) → BitVector.
 * @param self A Python PyBitVector instance.
 * @return New BitVector instance with all bits toggled, NULL on error;
 */
PyObject *
py_bv_invert(PyObject *self);
/**
 * @brief __bool__(BitVector) → boolean.
 * @param self A Python PyBitVector instance.
 * @return 1 if any bit is set, 0 otherwise
 */
int
py_bv_bool(PyObject *self);

#endif /* CBITS_PY_BITVECTOR_METHODS_OPS_H */