/**
 * @file src/python/bitvector_methods_basic.c
 * @brief Implementation of basic BitVector Python methods.
 *
 * Provides Python bindings for single‑bit access and mutation: get, set,
 * clear, flip, and the corresponding range operations. All arguments are
 * validated through shared parsing helpers.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_methods_basic.h"
#include "bitvector_parse.h"

PyObject *
py_bitvector_get(PyObject *self, PyObject *arg)
{
    size_t index;
    if (bv_parse_index(self, arg, &index) < 0) {
        return NULL;
    }

    int bit = bv__get_inline(((PyBitVectorObject *) self)->bv, index);
    return PyBool_FromLong(bit);
}

PyObject *
py_bitvector_set(PyObject *self, PyObject *arg)
{
    size_t index;
    if (bv_parse_index(self, arg, &index) < 0) {
        return NULL;
    }

    bv__set_inline(((PyBitVectorObject *) self)->bv, index);
    ((PyBitVectorObject *) self)->hash_cache = -1;
    Py_RETURN_NONE;
}

PyObject *
py_bitvector_clear(PyObject *self, PyObject *arg)
{
    size_t index;
    if (bv_parse_index(self, arg, &index) < 0) {
        return NULL;
    }

    bv__clear_inline(((PyBitVectorObject *) self)->bv, index);
    ((PyBitVectorObject *) self)->hash_cache = -1;
    Py_RETURN_NONE;
}

PyObject *
py_bitvector_flip(PyObject *self, PyObject *arg)
{
    size_t index;
    if (bv_parse_index(self, arg, &index) < 0) {
        return NULL;
    }

    bv__flip_inline(((PyBitVectorObject *) self)->bv, index);
    ((PyBitVectorObject *) self)->hash_cache = -1;
    Py_RETURN_NONE;
}

PyObject *
py_bitvector_set_range(PyObject *self, PyObject *args)
{
    size_t start, len;
    if (bv_parse_tuple(self, args, &start, &len) < 0) {
        return NULL;
    }
    bv_set_range(((PyBitVectorObject *) self)->bv, start, len);
    ((PyBitVectorObject *) self)->hash_cache = -1;
    Py_RETURN_NONE;
}

PyObject *
py_bitvector_clear_range(PyObject *self, PyObject *args)
{
    size_t start, len;
    if (bv_parse_tuple(self, args, &start, &len) < 0) {
        return NULL;
    }
    bv_clear_range(((PyBitVectorObject *) self)->bv, start, len);
    ((PyBitVectorObject *) self)->hash_cache = -1;
    Py_RETURN_NONE;
}

PyObject *
py_bitvector_flip_range(PyObject *self, PyObject *args)
{
    size_t start, len;
    if (bv_parse_tuple(self, args, &start, &len) < 0) {
        return NULL;
    }
    bv_flip_range(((PyBitVectorObject *) self)->bv, start, len);
    ((PyBitVectorObject *) self)->hash_cache = -1;
    Py_RETURN_NONE;
}
