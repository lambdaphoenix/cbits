/**
 * @file bitvector_parse.h
 * @brief Argument-parsing helpers for BitVector Python methods.
 *
 * Provides small, centralized utilities for validating and normalizing
 * BitVector method arguments:
 * - \ref bv_parse_index — validate and normalize a single index
 * - \ref bv_parse_tuple — parse ``(start, length)`` tuples
 *
 * Consolidates error handling and range checking so that all BitVector
 * operations follow consistent semantics.
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
 *
 * Accepts a Python integer, normalizes negative indices relative to the
 * BitVector length, and checks bounds.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @param arg Python argument expected to be an integer.
 * @param p_index Output pointer receiving the validated index.
 * @retval 0 Success; ``*p_index``is set.
 * @retval -1 Failure; a Python exception is set.
 *
 * @note Negative indices are translated using Python slice semantics.
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
 * @brief Parse and validate a ``(start, length)`` tuple.
 *
 * Extracts two non‑negative integers and verifies that the range fits within
 * the BitVector.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @param args Python tuple ``(start, length)``.
 * @param p_start Output pointer for start index.
 * @param p_len Output pointer for length.
 * @retval 0 Success; outputs are set.
 * @retval -1 Failure; a Python exception is set.
 * @since 0.2.0
 *
 * @note Both values must be non‑negative; overflow and range violations are
 * checked explicitly.
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
