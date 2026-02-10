/**
 * @file bitvector_methods_misc.c
 * @brief Implementation of miscellaneous ``BitVector`` Python methods.
 *
 * Provides protocol support and convenience features for the BitVector type:
 * - ``__repr__`` and ``__str__`` for string representations
 * - ``__len__`` for container length
 * - ``__contains__`` for membership tests
 * - the read‑only ``bits`` property
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_methods_misc.h"

PyObject *
py_bitvector_repr(PyObject *object)
{
    PyBitVectorObject *self = (PyBitVectorObject *) object;
    return PyUnicode_FromFormat("<cbits.BitVector object at %p bits=%zu>",
                                object, self->bv->n_bits);
}

PyObject *
py_bitvector_str(PyObject *self)
{
    PyBitVectorObject *bvself = (PyBitVectorObject *) self;
    return PyUnicode_FromFormat("BitVector with %zu bits", bvself->bv->n_bits);
}

Py_ssize_t
py_bitvector_len(PyObject *self)
{
    BitVector *bv = ((PyBitVectorObject *) self)->bv;
    return (Py_ssize_t) (bv ? bv->n_bits : 0);
}

int
py_bitvector_contains(PyObject *object, PyObject *value)
{
    PyBitVectorObject *self = (PyBitVectorObject *) object;
    cbits_state *state = find_cbits_state_by_type(Py_TYPE(object));

    if (!py_bitvector_check(value, state)) {
        return 0;
    }
    PyBitVectorObject *sub = (PyBitVectorObject *) value;
    return bv_contains_subvector(self->bv, sub->bv);
}

/**
 * @brief Getter for the read-only ``bits`` property.
 *
 * Returns the number of bits in the BitVector.
 *
 * @param object A ``PyBitVectorObject`` instance.
 * @param closure Unused.
 * @return Python integer of the bit-length
 */
static PyObject *
py_bitvector_get_size(PyObject *object, void *Py_UNUSED(closure))
{
    PyBitVectorObject *self = (PyBitVectorObject *) object;
    return PyLong_FromSize_t(self->bv->n_bits);
}

/**
 * @brief Property table for the BitVector type.
 *
 * Lists all Python‑visible properties. Currently only ``bits`` is exposed and
 * is read‑only.
 *
 * @see PyGetSetDef
 */
PyGetSetDef PyBitVector_getset[] = {
    {"bits", py_bitvector_get_size, NULL, PyDoc_STR("The number of bits.")},
    {NULL},
};
