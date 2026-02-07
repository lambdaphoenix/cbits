/**
 * @file src/python/bitvector_iter.h
 * @brief Iterator type for BitVector.
 *
 * Declares:
 * - py_bv_iter
 * - py_bviter_iternext, py_bviter_dealloc
 *
 * Used by \ref bitvector_object.c to expose iteration type.
 *
 * @see bitvector_object.h
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_ITER_H
#define CBITS_PY_BITVECTOR_ITER_H

#include "bitvector_object.h"

/**
 * @brief Create and return a new BitVector iterator.
 *
 * Implements the tp_iter slot. Allocates a fresh PyBitVectorIter, initializes
 * its state, and returns it.
 *
 * @param self A Python PyBitVector instance.
 * @return New iterator object or NULL on allocation failure.
 */
PyObject *
py_bv_iter(PyObject *self);
/**
 * @brief Return the next bit as a Python boolean.
 *
 * Reads one bit from the internal buffer and shifts it out. If all bits have
 * been yielded, raises StopIteration.
 *
 * @param self A PyBitVectorIter instance.
 * @return Py_True or Py_False on success; NULL with StopIteration set at
 * end-of-iteration.
 */
PyObject *
py_bviter_iternext(PyObject *self);
/**
 * @brief Deallocate a BitVector iterator object.
 *
 * Releases the reference to the parent PyBitVector and frees the iterator
 * struct.
 *
 * @param self A PyBitVectorIter instance.
 */
void
py_bviter_dealloc(PyObject *self);

#endif /* CBITS_PY_BITVECTOR_ITER_H */
