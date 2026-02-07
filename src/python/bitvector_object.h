/**
 * @file src/python/bitvector_object.h
 * @brief Definition of the PyBitVector Python type.
 *
 * Declares:
 * - \ref PyBitVector struct
 * - type objects and type specifications
 * - bv_wrap_new
 *
 * This header is the central definition of the Python BitVector type and is
 * included by all method modules and iterator implementations.
 *
 * @see bitvector_internal.h
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_OBJECT_H
#define CBITS_PY_BITVECTOR_OBJECT_H

#include <Python.h>
#include "bitvector_internal.h"

/**
 * @struct PyBitVector
 * @brief Python object containing a pointer to a native BitVector.
 *
 * Includes a cached hash value @c `hash_cache` to speed up repeated
 * dictionary/set lookups.
 */
typedef struct {
    PyObject_HEAD BitVector *bv; /**< Reference to the BitVector */
    Py_hash_t hash_cache;        /**< Cached hash value or -1 if invalid */
} PyBitVector;

/* Forward declarations */
extern PyTypeObject *PyBitVectorPtr;
extern PyTypeObject *PyBitVectorIterType;

extern PyType_Spec PyBitVector_spec;
extern PyType_Spec PyBitVectorIter_spec;

/**
 * @brief Wrap a native BitVector in a new PyBitVector Python object.
 * @param bv_data Pointer to an allocated BitVector.
 * @return New reference to a PyBitVector, or NULL on allocation failure.
 */
PyObject *
bv_wrap_new(BitVector *bv_data);

#endif /* CBITS_PY_BITVECTOR_OBJECT_H */
