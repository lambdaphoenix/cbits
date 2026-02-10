/**
 * @file bitvector_methods_rank.h
 * @brief Rank-related Python methods for ``BitVector``.
 *
 * Declares the Python binding for the ``BitVector.rank`` method, which counts
 * the number of bits set to True in the prefix range ``[0..index[``.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_RANK_H
#define CBITS_PY_BITVECTOR_METHODS_RANK_H

#include "bitvector_object.h"

/**
 * @brief Python binding for ``BitVector.rank(index)``.
 *
 * Parses the index argument, applies Python’s negative‑index normalization,
 * validates bounds, and returns the number of set bits in the half-open range
 * ``[0..index)``.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @param arg Python argument representing the index.
 * @retval int New Python integer on success.
 * @retval NULL on failure (exception set).
 */
PyObject *
py_bitvector_rank(PyObject *self, PyObject *arg);

#endif /* CBITS_PY_BITVECTOR_METHODS_RANK_H */
