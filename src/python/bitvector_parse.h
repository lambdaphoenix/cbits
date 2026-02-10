/**
 * @file src/python/bitvector_parse.h
 * @brief Argument parsing helpers for BitVector Python methods.
 *
 * Provide:
 * - \ref bv_parse_index "bv_parse_index": validate and normalize a single
 * index
 * - \ref bv_parse_tuple "bv_parse_tuple": parse (start, len) tuples
 *
 * Centralizes error handling and index normalization for all method modules.
 *
 * @see bitvector_object.h
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_PARSE_H
#define CBITS_PY_BITVECTOR_PARSE_H

#include "bitvector_object.h"

/**
 * @brief Parse and validate a single index argument.
 * @param self A Python PyBitVectorObject instance.
 * @param arg Python argument.
 * @param p_index Output pointer to store the validated index.
 * @return 0 on success (p_index set), -1 on failure (exception set).
 */
inline int
bv_parse_index(PyObject *self, PyObject *arg, size_t *p_index)
{
    if (!PyLong_Check(arg)) {
        PyErr_SetString(PyExc_TypeError, "BitVector index must be an integer");
        return -1;
    }
    Py_ssize_t index = PyLong_AsSsize_t(arg);
    if (index == -1 && PyErr_Occurred()) {
        return -1;
    }
    PyBitVectorObject *bvself = (PyBitVectorObject *) self;
    size_t n_bits = bvself->bv->n_bits;
    if (index < 0) {
        index += (Py_ssize_t) n_bits;
    }
    if (index < 0 || index >= n_bits) {
        PyErr_SetString(PyExc_IndexError, "BitVector index out of range");
        return -1;
    }
    *p_index = (size_t) index;
    return 0;
}

/**
 * @brief Parse and validate a (start, length) range tuple.
 * @param self A Python PyBitVectorObject instance.
 * @param args   Python argument tuple (start, length).
 * @param p_start Output pointer for start index.
 * @param p_len   Output pointer for length.
 * @return 0 on success (outputs set), -1 on failure (exception set).
 * @since 0.2.0
 */
inline int
bv_parse_tuple(PyObject *self, PyObject *args, size_t *p_start, size_t *p_len)
{
    Py_ssize_t start, len;
    if (!PyArg_ParseTuple(args, "nn", &start, &len)) {
        return -1;
    }
    if (start < 0 || len < 0) {
        PyErr_SetString(PyExc_ValueError,
                        "start and length must be non-negative");
        return -1;
    }
    PyBitVectorObject *bvself = (PyBitVectorObject *) self;
    if ((size_t) start + (size_t) len > bvself->bv->n_bits) {
        PyErr_SetString(PyExc_IndexError, "BitVector range out of bounds");
        return -1;
    }
    *p_start = (size_t) start;
    *p_len = (size_t) len;
    return 0;
}

#endif /* CBITS_PY_BITVECTOR_PARSE_H */
