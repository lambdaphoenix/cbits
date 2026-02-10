/**
 * @file src/python/bitvector_methods_misc.h
 * @brief Miscellaneous BitVector Python methods.
 *
 * Declares:
 * - __len__
 * - __repr__, __str__
 * - __contains__
 * - getset table
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_MISC_H
#define CBITS_PY_BITVECTOR_METHODS_MISC_H

#include "bitvector_object.h"

/**
 * @brief __len__(BitVector) → number of bits.
 * @param self A Python PyBitVectorObject instance.
 * @return Number of bits as Py_ssize_t.
 */
Py_ssize_t
py_bitvector_len(PyObject *self);
/**
 * @brief __repr__ for BitVector.
 * @param self A Python PyBitVectorObject instance.
 * @return New Python string describing the object.
 */
PyObject *
py_bitvector_repr(PyObject *self);
/**
 * @brief __str__ for BitVector.
 * @param self A Python PyBitVectorObject instance.
 * @return New Python string "BitVector with X bits".
 */
PyObject *
py_bitvector_str(PyObject *self);
/**
 * @brief __contains__(BitVector, other) → boolean.
 * @param self A Python PyBitVectorObject instance. (haystack).
 * @param value A Python PyBitVectorObject instance (needle).
 * @return 1 if contained, 0 otherwise
 */
int
py_bitvector_contains(PyObject *self, PyObject *value);

extern PyGetSetDef PyBitVector_getset[];

#endif /* CBITS_PY_BITVECTOR_METHODS_MISC_H */
