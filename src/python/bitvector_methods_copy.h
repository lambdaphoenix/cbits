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
 * @param self A Python PyBitVectorObject instance.
 * @param ignored Unused.
 * @return New BitVector copy
 */
PyObject *
py_bitvector_copy(PyObject *object, PyObject *Py_UNUSED(ignored));
/**
 * @brief Python binding for BitVector.__deepcopy__(memo) → BitVector.
 * @param self A Python PyBitVectorObject instance.
 * @param unused
 * @return New BitVector copy
 */
PyObject *
py_bitvector_deepcopy(PyObject *self, PyObject *unused);

#endif /* CBITS_PY_BITVECTOR_METHODS_COPY_H */
