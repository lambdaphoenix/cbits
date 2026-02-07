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
 * @brief Implements BitVector.__getitem__ dispatch for index or slice.
 *
 * Delegates either to py_bv_item (for integer indices) or to py_bv_slice (for
 * slice objects). Raises TypeError for unsupported types.
 *
 * @param self A Python PyBitVector instance.
 * @param arg Index or slice object.
 * @return New reference to a Python bool or PyBitVector; NULL and exception on
 * error.
 */
PyObject *
py_bv_subscript(PyObject *self, PyObject *arg);
/**
 * @brief Implements BitVector.__setitem__ dispatch for index or slice.
 *
 * Delegates to py_bv_ass_item or py_bv_ass_slice depending on type of `arg`.
 * Does not support item deletion (value==NULL).
 *
 * @param self A Python PyBitVector instance.
 * @param arg Index or slice object.
 * @param value Python object to assign (must not be NULL).
 * @return 0 on success; -1 on error (with exception set).
 */
int
py_bv_ass_subscript(PyObject *self, PyObject *arg, PyObject *value);

#endif /* CBITS_PY_BITVECTOR_METHODS_SLICE_H */
