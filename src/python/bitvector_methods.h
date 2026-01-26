#ifndef CBITS_PY_BITVECTOR_METHODS_H
#define CBITS_PY_BITVECTOR_METHODS_H

#include <Python.h>
#include "bitvector_internal.h"

/**
 * @struct PyBitVector
 * @brief Python object containing a pointer to a native BitVector.
 *
 * Includes a cached hash value @c 'hash_cache' to speed up repeated
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

#endif /* CBITS_PY_BITVECTOR_METHODS_H */