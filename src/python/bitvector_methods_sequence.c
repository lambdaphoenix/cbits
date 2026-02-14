/**
 * @file bitvector_methods_sequence.c
 * @brief Sequence protocol operations for BitVector (concat and repeat).
 *
 * Provides implementations for the sq_concat and sq_repeat slots of the
 * BitVector type. These operations construct new BitVector instances by
 * concatenating or repeating the bit pattern of existing vectors.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_object.h"

PyObject *
py_bitvector_concat(PyObject *oA, PyObject *oB)
{
    PyBitVectorObject *A = (PyBitVectorObject *) oA;
    cbits_state *state = find_cbits_state_by_type(Py_TYPE(A));

    if (!py_bitvector_check(oA, state) || !py_bitvector_check(oB, state)) {
        PyErr_SetString(PyExc_TypeError,
                        "can only concatenate BitVector to BitVector");
        return NULL;
    }

    PyBitVectorObject *B = (PyBitVectorObject *) oB;
    if (A->bv == NULL || B->bv == NULL) {
        PyErr_BadInternalCall();
        return NULL;
    }

    BitVector *res = bv_concat(A->bv, B->bv);
    if (res == NULL) {
        PyErr_SetString(PyExc_MemoryError,
                        "Failed to allocate BitVector for repeat");
        return NULL;
    }

    PyObject *result = bitvector_wrap_new(Py_TYPE(oA), res);
    if (result == NULL) {
        bv_free(res);
        return NULL;
    }
    return result;
}
PyObject *
py_bitvector_repeat(PyObject *self, Py_ssize_t count)
{
    PyBitVectorObject *bv_obj = (PyBitVectorObject *) self;
    if (bv_obj->bv == NULL) {
        PyErr_BadInternalCall();
        return NULL;
    }

    if (count < 0) {
        PyErr_SetString(PyExc_ValueError, "negative repeat count");
        return NULL;
    }

    BitVector *res = bv_repeat(bv_obj->bv, (size_t) count);

    if (res == NULL) {
        PyErr_SetString(PyExc_MemoryError,
                        "Failed to allocate BitVector for repeat");
        return NULL;
    }

    PyObject *result = bitvector_wrap_new(Py_TYPE(self), res);
    if (result == NULL) {
        bv_free(res);
        return NULL;
    }
    return result;
}
