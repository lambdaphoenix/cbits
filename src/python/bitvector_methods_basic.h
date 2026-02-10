/**
 * @file bitvector_methods_basic.h
 * @brief Basic BitVector operations exposed to Python.
 *
 * Declares the Python bindings for fundamental bit‑manipulation operations:
 * - single‑bit access: ``get``, ``set``, ``clear``, ``flip``
 * - range operations: ``set_range``, ``clear_range``, ``flip_range``
 *
 * These functions wrap the corresponding C backend routines and update the
 * BitVector’s cached hash when mutations occur.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_BASIC_H
#define CBITS_PY_BITVECTOR_METHODS_BASIC_H

#include "bitvector_object.h"

/**
 * @brief Python binding for ``BitVector.get(index)``.
 *
 * Returns the boolean value of the bit at the given index.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @param arg Python integer specifying the index.
 * @retval bool New Python bool on success.
 * @retval NULL on failure (exception set).
 */
PyObject *
py_bitvector_get(PyObject *self, PyObject *arg);
/**
 * @brief Python binding for ``BitVector.set(index)``.
 *
 * Sets the bit at the given index to True.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @param arg Python integer specifying the index.
 * @retval Py_None on success.
 * @retval NULL on failure (exception set).
 */
PyObject *
py_bitvector_set(PyObject *self, PyObject *arg);
/**
 * @brief Python binding for ``BitVector.clear(index)``.
 *
 * Clears the bit at the given index (sets it to False).
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @param arg Python integer specifying the index.
 * @retval Py_None on success.
 * @retval NULL on failure (exception set).
 */
PyObject *
py_bitvector_clear(PyObject *self, PyObject *arg);
/**
 * @brief Python binding for ``BitVector.flip(index)``.
 *
 * Toggles the bit at the given index.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @param arg Python integer specifying the index.
 * @retval Py_None on success.
 * @retval NULL on failure (exception set).
 */
PyObject *
py_bitvector_flip(PyObject *self, PyObject *arg);
/**
 * @brief Python binding for ``BitVector.set_range(start, length)``.
 *
 * Sets all bits in the half-open range ``[start, start + length)``.
 * Invalidates the hash cache.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @param args Tuple ``(start, length)``.
 * @retval Py_None on success.
 * @retval NULL on failure (exception set).
 * @since 0.2.0
 */
PyObject *
py_bitvector_set_range(PyObject *self, PyObject *args);
/**
 * @brief Python binding for ``BitVector.clear_range(start, length)``.
 *
 * Clears all bits in the half-open range ``[start, start + length)``.
 * Invalidates the hash cache.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @param args Tuple ``(start, length)``.
 * @retval Py_None on success
 * @retval NULL on failure (exception set).
 * @since 0.2.0
 */
PyObject *
py_bitvector_clear_range(PyObject *self, PyObject *args);
/**
 * @brief Python binding for ``BitVector.flip_range(start, length)``.
 *
 * Toggles all bits in the half-open range ``[start, start + length)``.
 * Invalidates the hash cache.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @param args Tuple ``(start, length)``.
 * @retval Py_None on success.
 * @retval NULL on failure (exception set).
 * @since 0.2.0
 */
PyObject *
py_bitvector_flip_range(PyObject *self, PyObject *args);

#endif /* CBITS_PY_BITVECTOR_METHODS_BASIC_H */
