#ifndef CBITS_PY_BITVECTOR_METHODS_COPY_H
#define CBITS_PY_BITVECTOR_METHODS_COPY_H

#include "bitvector_methods.h"

/**
 * @brief Python binding for BitVector.copy() → BitVector.
 * @param self A Python PyBitVector instance.
 * @param UNUSED
 * @return New BitVector copy
 */
PyObject *
py_bv_copy(PyObject *self, PyObject *ignored);
/**
 * @brief Python binding for BitVector.__deepcopy__(memo) → BitVector.
 * @param self A Python PyBitVector instance.
 * @param memo
 * @return New BitVector copy
 */
PyObject *
py_bv_deepcopy(PyObject *self, PyObject *memo);

#endif /* CBITS_PY_BITVECTOR_METHODS_COPY_H */