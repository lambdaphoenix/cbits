/**
 * @file bitvector_methods_slice.h
 * @brief Slicing and indexed access support for ``BitVector``.
 *
 * Declares the functions that implement ``__getitem__`` and ``__setitem__``
 * for both integer indices and slice objects. These functions provide the core
 * Python‑level indexing and slicing behavior for the BitVector type.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_SLICE_H
#define CBITS_PY_BITVECTOR_METHODS_SLICE_H

#include "bitvector_object.h"

/**
 * @brief Implement ``BitVector.__getitem__`` for a single integer index.
 *
 * Validates the index, checks bounds, and returns the corresponding bit as a
 * Python boolean. Raises ``IndexError`` on out‑of‑range access.
 *
 * @param object A ``PyBitVectorObject`` instance.
 * @param i Index to access
 * @retval bool New reference to ``Py_True`` or ``Py_False`` on success.
 * @retval NULL on failure (exception set).
 */
PyObject *
py_bitvector_item(PyObject *object, Py_ssize_t i);
/**
 * @brief Implement ``BitVector.__setitem__`` for a single integer index.
 *
 * Sets or clears the bit at position ``i`` based on the truth value of
 * ``value``. Raises ``IndexError`` if the index is out of range.
 *
 * @param object A ``PyBitVectorObject`` instance.
 * @param i Index of the bit to assign.
 * @param value Python object interpreted as boolean.
 * @retval 0 Success.
 * @retval -1 Failure (exception set).
 */
int
py_bitvector_ass_item(PyObject *object, Py_ssize_t i, PyObject *value);
/**
 * @brief Implement ``BitVector.__getitem__`` dispatch for index or slice.
 *
 * Dispatches to ``py_bitvector_item`` for integer indices or to the slice
 * handler for slice objects. Raises ``TypeError`` for unsupported types.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @param arg Integer index or slice object.
 * @retval value New reference to a Python bool or a new ``PyBitVectorObject``.
 * @retval NULL on failure (exception set).
 */
PyObject *
py_bitvector_subscript(PyObject *self, PyObject *arg);
/**
 * @brief Implements ``BitVector.__setitem__`` dispatch for index or slice.
 *
 * Dispatches to ``py_bitvector_ass_item`` or the slice assignment handler
 * depending on the type of ``arg``. Deletion (``value == NULL``) is not
 * supported.
 *
 * @param self A Python PyBitVectorObject instance.
 * @param arg Index or slice object.
 * @param value Python object to assign (must not be NULL).
 * @retval 0 Success.
 * @retval -1 Failure (exception set).
 */
int
py_bitvector_ass_subscript(PyObject *self, PyObject *arg, PyObject *value);

#endif /* CBITS_PY_BITVECTOR_METHODS_SLICE_H */
