#ifndef CBITS_PY_BITVECTOR_ITER_H
#define CBITS_PY_BITVECTOR_ITER_H

#include "bitvector_methods.h"

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