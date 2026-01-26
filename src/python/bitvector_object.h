#ifndef CBITS_PY_BITVECTOR_OBJECT_H
#define CBITS_PY_BITVECTOR_OBJECT_H

#include "bitvector_methods.h"

/**
 * @brief Wrap a native BitVector in a new PyBitVector Python object.
 * @param bv_data Pointer to an allocated BitVector.
 * @return New reference to a PyBitVector, or NULL on allocation failure.
 */
PyObject *
bv_wrap_new(BitVector *bv_data);

#endif /* CBITS_PY_BITVECTOR_OBJECT_H */