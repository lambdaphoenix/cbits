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
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_ITER_H
#define CBITS_PY_BITVECTOR_ITER_H

#include "bitvector_object.h"

extern PyType_Spec PyBitVectorIter_spec;

/**
 * @brief Create and return a new BitVector iterator.
 *
 * Implements the tp_iter slot. Allocates a fresh PyBitVectorIter, initializes
 * its state, and returns it.
 *
 * @param self A Python PyBitVectorObject instance.
 * @return New iterator object or NULL on allocation failure.
 */
PyObject *
py_bitvector_iter(PyObject *self);

#endif /* CBITS_PY_BITVECTOR_ITER_H */
