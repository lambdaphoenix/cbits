#ifndef CBITS_PY_BITVECTOR_METHODS_BASIC_H
#define CBITS_PY_BITVECTOR_METHODS_BASIC_H

#include "bitvector_methods.h"

/**
 * @brief Python binding for BitVector.get(index) → bool.
 * @param self A Python PyBitVector instance.
 * @param arg Python argument.
 * @return true is bit is set, false otherwise
 */
PyObject *
py_bv_get(PyObject *self, PyObject *arg);
/**
 * @brief Python binding for BitVector.set(index).
 * @param self A Python PyBitVector instance.
 * @param arg Python argument.
 * @return None on success, NULL on error.
 */
PyObject *
py_bv_set(PyObject *self, PyObject *arg);
/**
 * @brief Python binding for BitVector.clear(index).
 * @param self A Python PyBitVector instance.
 * @param args Python argument.
 * @return None on success, NULL on error.
 */
PyObject *
py_bv_clear(PyObject *self, PyObject *arg);
/**
 * @brief Python binding for BitVector.flip(index).
 * @param self A Python PyBitVector instance.
 * @param arg Python argument.
 * @return None on success, NULL on error.
 */
PyObject *
py_bv_flip(PyObject *self, PyObject *arg);

/**
 * @brief Python binding for BitVector.set_range(start, length).
 *
 * Calls bv_set_range() to set all bits in the half‑open range
 * [start, start+length). Marks the hash cache invalid and returns None.
 *
 * @param self A Python PyBitVector instance.
 * @param args Tuple (start, length).
 * @return Py_None on success, NULL on error (with exception set).
 * @since 0.2.0
 */
PyObject *
py_bv_set_range(PyObject *self, PyObject *args);
/**
 * @brief Python binding for BitVector.clear_range(start, length).
 *
 * Calls bv_clear_range() to clear all bits in the half‑open range
 * [start, start+length). Marks the hash cache invalid and returns None.
 *
 * @param self A Python PyBitVector instance.
 * @param args Tuple (start, length).
 * @return Py_None on success, NULL on error (with exception set).
 * @since 0.2.0
 */
PyObject *
py_bv_clear_range(PyObject *self, PyObject *args);
/**
 * @brief Python binding for BitVector.flip_range(start, length).
 *
 * Calls bv_flip_range() to toggle all bits in the half‑open range
 * [start, start+length). Marks the hash cache invalid and returns None.
 *
 * @param self A Python PyBitVector instance.
 * @param args Tuple (start, length).
 * @return Py_None on success, NULL on error (with exception set).
 * @since 0.2.0
 */
PyObject *
py_bv_flip_range(PyObject *self, PyObject *args);


#endif /* CBITS_PY_BITVECTOR_METHODS_BASIC_H */