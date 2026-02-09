/**
 * @file src/python/bitvector_methods_slice.h
 * @brief Slicing support for BitVector.
 *
 * Declares:
 * - __getitem__ for slice objects
 * - __setitem__ for slice assignment
 *
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_SLICE_H
#define CBITS_PY_BITVECTOR_METHODS_SLICE_H

#include "bitvector_object.h"

/**
 * @brief Implements BitVector.__getitem__, returns the bit at position i.
 *
 * This function checks bounds and returns the corresponding Python boolean
 * (True/False). On out-of-range access it raises IndexError.
 *
 * @param self A Python PyBitVectorObject instance.
 * @param i Index to access
 * @return New reference to Py_True or Py_False on success; NULL and IndexError
 * on failure.
 */
PyObject *
py_bitvector_item(PyObject *object, Py_ssize_t i);
/**
 * @brief Implements BitVector.__setitem__ for a single index.
 *
 * Sets or clears the bit at position i based on the truth value of
 * `value`. Raises IndexError if the index is out of range.
 *
 * @param self A Python PyBitVectorObject instance.
 * @param i Index of the bit to assign.
 * @param value Python object interpreted as boolean.
 * @return 0 on success; -1 on error (with exception set).
 */
int
py_bitvector_ass_item(PyObject *object, Py_ssize_t i, PyObject *value);
/**
 * @brief Implements BitVector.__getitem__ dispatch for index or slice.
 *
 * Delegates either to py_bitvector_item (for integer indices) or to
 * py_bitvector_slice (for slice objects). Raises TypeError for unsupported
 * types.
 *
 * @param self A Python PyBitVectorObject instance.
 * @param arg Index or slice object.
 * @return New reference to a Python bool or PyBitVectorObject; NULL and
 * exception on error.
 */
PyObject *
py_bitvector_subscript(PyObject *self, PyObject *arg);
/**
 * @brief Implements BitVector.__setitem__ dispatch for index or slice.
 *
 * Delegates to py_bitvector_ass_item or py_bitvector_ass_slice depending on
 * type of `arg`. Does not support item deletion (value==NULL).
 *
 * @param self A Python PyBitVectorObject instance.
 * @param arg Index or slice object.
 * @param value Python object to assign (must not be NULL).
 * @return 0 on success; -1 on error (with exception set).
 */
int
py_bitvector_ass_subscript(PyObject *self, PyObject *arg, PyObject *value);

#endif /* CBITS_PY_BITVECTOR_METHODS_SLICE_H */
