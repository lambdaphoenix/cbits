/**
 * @file src/python/bitvector_methods_copy.c
 * @brief Implementation of copy and clone operations.
 *
 * Uses bv_copy from the C backend to duplicate BitVector instances.
 *
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_methods_copy.h"
#include "bitvector_object.h"

PyObject *
py_bv_copy(PyObject *self, PyObject *ignored)
{
    BitVector *copy = bv_copy(((PyBitVector *) self)->bv);
    if (!copy) {
        PyErr_SetString(PyExc_MemoryError,
                        "Failed to allocate BitVector in copy()");
        return NULL;
    }
    return bv_wrap_new(copy);
}

PyObject *
py_bv_deepcopy(PyObject *self, PyObject *memo)
{
    PyObject *copy = py_bv_copy(self, NULL);
    if (!copy) {
        return NULL;
    }
    if (memo && PyDict_Check(memo)) {
        if (PyDict_SetItem(memo, self, copy) < 0) {
            Py_DECREF(copy);
            return NULL;
        }
    }
    return copy;
}
