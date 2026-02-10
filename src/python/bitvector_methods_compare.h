/**
 * @file src/python/bitvector_methods_compare.h
 * @brief Equality and hashing methods for BitVector.
 *
 * Declares:
 * - rich comparison (__eq__, __ne__)
 * - __hash__ implementation
 *
 * These functions wrap the C backend’s equality check and provide a cached
 * hash suitable for Python dictionaries and sets.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_COMPARE_H
#define CBITS_PY_BITVECTOR_METHODS_COMPARE_H

#include "bitvector_object.h"

/**
 * @brief Rich comparison (== and !=) for BitVector.
 * @param a First operant.
 * @param b Second operant.
 * @param op Comparison operation (Py_EQ or Py_NE).
 * @return Py_True or Py_False on success; Py_RETURN_NOTIMPLEMENTED if
 * unsupported.
 */
PyObject *
py_bitvector_richcompare(PyObject *a, PyObject *b, int op);
/**
 * @brief __hash__ for a BitVector object.
 *
 * Computes a hash over the vector’s packed bit data using Python’s internal
 * _Py_HashBytes helper. The result is cached in the object until the BitVector
 * is mutated.
 *
 * @param self A Python PyBitVectorObject instance.
 * @return A Py_hash_t value derived from the bit‐pattern contents.
 */
Py_hash_t
py_bitvector_hash(PyObject *self);

#endif /* CBITS_PY_BITVECTOR_METHODS_COMPARE_H */
