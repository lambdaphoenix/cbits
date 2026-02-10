/**
 * @file bitvector_methods_copy.h
 * @brief Copy and clone operations for ``BitVector``.
 *
 * Declares the Python bindings for shallow and deep copy operations:
 * - ``copy()`` - return a new BitVector with identical contents
 * - ``__deepcopy__`` - participate in Python’s ``copy`` module protocol
 *
 * These functions provide standard copying semantics for the BitVector type.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_COPY_H
#define CBITS_PY_BITVECTOR_METHODS_COPY_H

#include "bitvector_object.h"

/**
 * @brief Python binding for ``BitVector.copy()``.
 *
 * Creates and returns a new BitVector with the same bit contents as
 * ``object``.
 *
 * @param object A ``PyBitVectorObject`` instance.
 * @param ignored Unused.
 * @retval copy New ``PyBitVectorObject`` on success.
 * @retval NULL on failure (exception set).
 */
PyObject *
py_bitvector_copy(PyObject *object, PyObject *Py_UNUSED(ignored));
/**
 * @brief Python binding for ``BitVector.__deepcopy__(memo)``.
 *
 * Creates a deep copy of the BitVector and registers it in the ``memo``
 * dictionary used by Python’s ``copy`` module.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @param memo Dictionary used by Python's deepcopy mechanism.
 * @retval copy New ``PyBitVectorObject`` on success.
 * @retval NULL on failure (exception set).
 */
PyObject *
py_bitvector_deepcopy(PyObject *self, PyObject *memo);

#endif /* CBITS_PY_BITVECTOR_METHODS_COPY_H */
