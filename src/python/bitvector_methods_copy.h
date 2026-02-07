/**
 * @file src/python/bitvector_methods_copy.h
 * @brief Copy and clone operations for BitVector.
 *
 * Declares:
 * - copy()
 * - __deepcopy__
 *
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_COPY_H
#define CBITS_PY_BITVECTOR_METHODS_COPY_H

#include "bitvector_object.h"

/**
 * @brief Python binding for BitVector.copy() → BitVector.
 * @param self A Python PyBitVector instance.
 * @param ignored Unused.
 * @return New BitVector copy
 */
PyObject *
py_bv_copy(PyObject *self, PyObject *ignored);
/**
 * @brief Python binding for BitVector.__deepcopy__(memo) → BitVector.
 * @param self A Python PyBitVector instance.
 * @param memo
 * @return New BitVector copy
 */
PyObject *
py_bv_deepcopy(PyObject *self, PyObject *memo);

#endif /* CBITS_PY_BITVECTOR_METHODS_COPY_H */
