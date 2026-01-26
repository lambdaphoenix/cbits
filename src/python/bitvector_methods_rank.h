#ifndef CBITS_PY_BITVECTOR_METHODS_RANK_H
#define CBITS_PY_BITVECTOR_METHODS_RANK_H

#include "bitvector_methods.h"

/**
 * @brief Python binding for BitVector.rank(index) → bool.
 * @param self A Python PyBitVector instance.
 * @param args Array of Python arguments.
 * @param n_args Number of arguments expected (should be 1).
 * @return Number of bits set in range [0...pos]
 */
PyObject *
py_bv_rank(PyObject *self, PyObject *arg);

#endif /* CBITS_PY_BITVECTOR_METHODS_RANK_H */