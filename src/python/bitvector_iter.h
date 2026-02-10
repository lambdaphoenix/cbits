/**
 * @file bitvector_iter.h
 * @brief Iterator type for ``BitVector``.
 *
 * Declares the iterator type and constructor used to support Python’s
 * iteration protocol for BitVector objects:
 * - ``py_bv_iter`` (tp_iter implementation)
 * - iterator methods such as ``tp_iternext`` and ``tp_dealloc``
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
 * @brief Create and a new BitVector iterator.
 *
 * Implements the ``tp_iter`` slot for BitVector. Allocates and initializes a
 * fresh iterator object whose internal state tracks the current bit index.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @retval iter New iterator object on success
 * @retval NULL on allocation failure (ecxception set).
 */
PyObject *
py_bitvector_iter(PyObject *self);

#endif /* CBITS_PY_BITVECTOR_ITER_H */
