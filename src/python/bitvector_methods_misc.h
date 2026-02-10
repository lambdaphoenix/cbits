/**
 * @file bitvector_methods_misc.h
 * @brief Miscellaneous BitVector Python methods for ``BitVector``.
 *
 * Declares helpers for Python protocol integration:
 * - ``__len__`` (size of the BitVector)
 * - ``__repr__`` and ``__str__`` (string representations)
 * - ``__contains__`` (membership test)
 * - the get/set descriptor table
 *
 * These functions provide the Python‑level convenience and protocol behavior
 * expected from a container‑like type.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_MISC_H
#define CBITS_PY_BITVECTOR_METHODS_MISC_H

#include "bitvector_object.h"

/**
 * @brief Implement ``len(BitVector)``.
 *
 * Returns the number of bits stored in the BitVector.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @retval Py_ssize_t Number of bits.
 */
Py_ssize_t
py_bitvector_len(PyObject *self);
/**
 * @brief Implement ``repr(BitVector)``.
 *
 * Produces a developer‑oriented string representation of the BitVector.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @retval string New Python string on success.
 * @retval NULL on failure (exception set)
 */
PyObject *
py_bitvector_repr(PyObject *self);
/**
 * @brief Implement ``str(BitVector)``.
 *
 * Produces a human‑readable description such as ``"BitVector with N bits"``.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @retval string New Python string on success.
 * @retval NULL on failure (exception set)
 */
PyObject *
py_bitvector_str(PyObject *self);
/**
 * @brief Implement ``value in BitVector``.
 *
 * Checks whether the given BitVector is contained as a contiguous subvector of
 * ``self``. Returns a truth value as an integer.
 *
 * @param self A ``PyBitVectorObject`` instance. (haystack).
 * @param value A ``PyBitVectorObject`` instance (needle).
 * @retval 1 Contained.
 * @retval 0 Not contained.
 * @retval -1 Failure (exception set):
 */
int
py_bitvector_contains(PyObject *self, PyObject *value);

extern PyGetSetDef PyBitVector_getset[];

#endif /* CBITS_PY_BITVECTOR_METHODS_MISC_H */
