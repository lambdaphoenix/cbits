/**
 * @file src/python/bitvector_methods_copy.c
 * @brief Implementation of copy and clone operations.
 *
 * Uses bv_copy from the C backend to duplicate BitVector instances.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_methods_copy.h"
#include "bitvector_object.h"

PyObject *
py_bitvector_copy(PyObject *object, PyObject *Py_UNUSED(ignored))
{
    cbits_state *state = find_cbits_state_by_type(Py_TYPE(object));
    PyBitVectorObject *self = (PyBitVectorObject *) object;

    BitVector *copy = bv_copy(self->bv);
    if (!copy) {
        PyErr_SetString(PyExc_MemoryError,
                        "Failed to allocate BitVector in copy()");
        return NULL;
    }

    return bitvector_wrap_new(state->PyBitVectorType, copy);
}

PyObject *
py_bitvector_deepcopy(PyObject *self, PyObject *memo)
{
    if (memo == NULL) {
        PyErr_SetString(PyExc_TypeError, "__deepcopy__ missing memo argument");
        return NULL;
    }

    PyObject *existing = PyObject_GetItem(memo, self);
    if (existing) {
        return existing;
    }

    if (PyErr_ExceptionMatches(PyExc_KeyError)) {
        PyErr_Clear();
    }
    else if (PyErr_Occurred()) {
        return NULL;
    }

    PyObject *copy_obj = py_bitvector_copy(self, NULL);
    if (copy_obj == NULL) {
        return NULL;
    }

    if (PyObject_SetItem(memo, self, copy_obj) < 0) {
        Py_DECREF(copy_obj);
        return NULL;
    }
    return copy_obj;
}
