/**
 * @file src/python/bitvector_methods_misc.c
 * @brief Implementation of miscellaneous BitVector methods.
 *
 * Implements:
 * - __repr__ and __str__
 * - __len__
 * - __contains__
 * - the "bits" read-only property
 *
 * These methods provide standard Python protocol support and convenience
 * features for the BitVector type.
 *
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_methods_misc.h"

PyObject *
py_bv_repr(PyObject *self)
{
    PyBitVector *bvself = (PyBitVector *) self;
    return PyUnicode_FromFormat("<cbits.BitVector object at %p bits=%zu>",
                                self, bvself->bv->n_bits);
}

PyObject *
py_bv_str(PyObject *self)
{
    PyBitVector *bvself = (PyBitVector *) self;
    return PyUnicode_FromFormat("BitVector with %zu bits", bvself->bv->n_bits);
}

Py_ssize_t
py_bv_len(PyObject *self)
{
    BitVector *bv = ((PyBitVector *) self)->bv;
    return (Py_ssize_t) (bv ? bv->n_bits : 0);
}

int
py_bv_contains(PyObject *self, PyObject *value)
{
    if (!PyObject_TypeCheck((PyObject *) value, PyBitVectorPtr)) {
        return false;
    }

    PyBitVector *A = (PyBitVector *) self;
    PyBitVector *B = (PyBitVector *) value;
    return bv_contains_subvector(A->bv, B->bv);
}

/**
 * @brief Getter for the read-only "bits" property.
 * @param self A Python PyBitVector instance.
 * @param closure Unused.
 * @return Python integer of the bit-length
 */
static PyObject *
py_bv_get_size(PyObject *self, void *closure)
{
    PyBitVector *bvself = (PyBitVector *) self;
    return PyLong_FromSize_t(bvself->bv->n_bits);
}

/**
 * @brief Setter for the read-only "bits" property, always raises.
 * @param self A Python PyBitVector instance.
 * @param closure Unused.
 * @return -1 and sets AttributeError
 */
static int
py_bv_set_size(PyObject *self, void *closure)
{
    PyErr_SetString(PyExc_AttributeError, "size is read-only");
    return -1;
}

/**
 * @brief Property definitions for the BitVector type.
 *
 * This table lists all read-only and writable properties exposed
 * on the Python BitVector object.
 *
 * @see PyGetSetDef
 */
PyGetSetDef PyBitVector_getset[] = {
    {"bits", (getter) py_bv_get_size, (setter) py_bv_set_size,
     PyDoc_STR("The number of bits"), NULL},
    {NULL},
};
