/**
 * @file src/python/bitvector_methods_rank.h
 * @brief Rank-related Python methods for BitVector.
 *
 * Declares:
 * - rank
 *
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_RANK_H
#define CBITS_PY_BITVECTOR_METHODS_RANK_H

#include "bitvector_object.h"

/**
 * @brief Python binding for BitVector.rank(index) → bool.
 * @param self A Python PyBitVector instance.
 * @param arg Python argument.
 * @return Number of bits set in range [0...pos]
 */
PyObject *
py_bv_rank(PyObject *self, PyObject *arg);

#endif /* CBITS_PY_BITVECTOR_METHODS_RANK_H */
