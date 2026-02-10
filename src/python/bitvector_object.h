/**
 * @file bitvector_object.h
 * @brief Definition of the ``PyBitVector`` Python type.
 *
 * Declares the core Python wrapper type for native ``BitVector`` objects.
 * This header provides:
 * - \ref PyBitVectorObject - the Python object structure
 * - the type specification used to create the Python type
 * - ``bitvector_wrap_new`` - a helper for constructing new wrapper objects
 *
 * It is included by all BitVector method implementations and iterator
 * components, forming the central definition of the Python‑level BitVector
 * API.
 *
 * @see bitvector_internal.h
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_OBJECT_H
#define CBITS_PY_BITVECTOR_OBJECT_H

#include "bitvector_internal.h"
#include "cbits_state.h"

/**
 * @brief Python object wrapping a native ``BitVector`` instance.
 *
 * Stores a pointer to the underlying native BitVector and maintains a cached
 * hash value to accelerate repeated dictionary and set lookups.
 */
typedef struct {
    PyObject_HEAD BitVector *bv; /**< Reference to the underlying BitVector */
    Py_hash_t hash_cache;        /**< Cached hash value or -1 if invalid */
#if PY_VERSION_HEX < 0x030C0000
    PyObject *weakreflist; /**< List of weak references */
#endif
} PyBitVectorObject;

extern PyType_Spec PyBitVector_spec;

/**
 * @brief Wrap a native BitVector in a new ``PyBitVector`` Python object.
 *
 * Allocates a new Python wrapper object and assigns ownership of the provided
 * ``BitVector`` pointer to it.
 *
 * @param type The ``PyTypeObject``representing the BitVector type.
 * @param bv_data Pointer to an already-allocated native BitVector.
 * @retval new_object A new reference to a ``PyBitVector`` instance.
 * @retval NULL on allocation failure.
 *
 * @note The caller transfers ownership of ``bv_data`` to the returned object.
 */
PyObject *
bitvector_wrap_new(PyTypeObject *type, BitVector *bv_data);

#endif /* CBITS_PY_BITVECTOR_OBJECT_H */
