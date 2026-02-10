/**
 * @file bitvector_methods_compare.h
 * @brief Equality and hashing methods for ``BitVector``.
 *
 * Declares the Python bindings for:
 * - rich comparison (``__eq__`` and ``__ne__``)
 * - ``__hash__`` implementation with caching
 *
 * These functions wrap the C backend’s equality logic and compute a stable,
 * cached hash suitable for use in Python dictionaries and sets.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_COMPARE_H
#define CBITS_PY_BITVECTOR_METHODS_COMPARE_H

#include "bitvector_object.h"

/**
 * @brief Implement rich comparison for BitVector.
 *
 * Supports ``==`` and ``!=``. For unsupported operations, returns
 * ``Py_NotImplemented``.
 *
 * @param a First operant.
 * @param b Second operant.
 * @param op Comparison operation (``Py_EQ`` or ``Py_NE``).
 * @retval bool ``Py_True`` or ``Py_False`` on success.
 * @retval Py_RETURN_NOTIMPLEMENTED for unsupported comparisons.
 */
PyObject *
py_bitvector_richcompare(PyObject *a, PyObject *b, int op);
/**
 * @brief Implement ``hash(BitVector)``.
 *
 * Computes a hash over the packed bit data using Python’s internal
 * ``_Py_HashBytes``. The result is cached in the BitVector object until the
 * underlying data is mutated.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @return A ``Py_hash_t`` value derived from the bit‐pattern.
 */
Py_hash_t
py_bitvector_hash(PyObject *self);

#endif /* CBITS_PY_BITVECTOR_METHODS_COMPARE_H */
