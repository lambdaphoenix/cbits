/**
 * @file src/python/bitvector_methods_rank.c
 * @brief Implementation of rank-related Python methods.
 *
 * Wraps the C backend's rank queries.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_methods_rank.h"
#include "bitvector_parse.h"

PyObject *
py_bitvector_rank(PyObject *self, PyObject *arg)
{
    size_t index;
    if (bv_parse_index(self, arg, &index) < 0) {
        return NULL;
    }

    size_t rank = bv_rank(((PyBitVectorObject *) self)->bv, index);
    return PyLong_FromSize_t(rank);
}
