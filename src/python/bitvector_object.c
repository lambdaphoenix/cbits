/**
 * @file src/python/bitvector_object.c
 * @brief Implementation of the PyBitVector Python type.
 *
 * Defines:
 * - type initialization via PyType_Spec
 * - py_bv_new, py_bv_init, py_bv_free
 * - integration of all method groups and iterator support
 *
 * This file binds the C BitVector backend to the Python's objet module.
 *
 * @see bitvector_object.h
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_object.h"
#include "bitvector_methods.h"
#include "bitvector_methods_misc.h"
#include "bitvector_methods_compare.h"
#include "bitvector_methods_slice.h"
#include "bitvector_methods_ops.h"
#include "bitvector_iter.h"

PyObject *
bv_wrap_new(BitVector *bv_data)
{
    PyBitVector *obj =
        (PyBitVector *) PyBitVectorPtr->tp_alloc(PyBitVectorPtr, 0);
    if (!obj) {
        bv_free(bv_data);
        return NULL;
    }
    obj->bv = bv_data;
    obj->hash_cache = -1;
    return (PyObject *) obj;
}

/**
 * @brief __new__ for BitVector: allocate the Python object.
 * @param type The Python type object.
 * @param args Positional args (unused).
 * @param kwds Keyword args (unused).
 * @return New, uninitialized PyBitVector or NULL on failure.
 */
PyObject *
py_bv_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyBitVector *bvself = (PyBitVector *) type->tp_alloc(type, 0);
    if (!bvself) {
        return NULL;
    }
    bvself->bv = NULL;
    bvself->hash_cache = -1;
    return (PyObject *) bvself;
}

/**
 * @brief __init__ for BitVector(size): allocate the underlying C BitVector.
 * @param self A Python PyBitVector instance.
 * @param args Positional args tuple.
 * @param kwds Keyword args dict.
 * @return 0 on success, -1 on error (with exception set).
 */
static int
py_bv_init(PyObject *self, PyObject *args, PyObject *kwds)
{
    Py_ssize_t n_bits;
    static char *kwlist[] = {"size", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "n", kwlist, &n_bits)) {
        return -1;
    }
    PyBitVector *bvself = (PyBitVector *) self;
    bvself->bv = bv_new((size_t) n_bits);
    if (!bvself->bv) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate BitVector");
        return -1;
    }
    return 0;
}

/**
 * @brief Deallocate a PyBitVector object.
 * @param self A Python PyBitVector instance.
 */
static void
py_bv_free(PyObject *self)
{
    PyBitVector *bvself = (PyBitVector *) self;
    if (bvself->bv) {
        bv_free(bvself->bv);
        bvself->bv = NULL;
    }
    Py_TYPE(self)->tp_free(self);
}

PyDoc_STRVAR(
    BitVector__doc__,
    "BitVector(size: int)\n"
    "\n"
    "A high-performance, fixed-size 1D bit array.\n\n"
    "Supports random access, slicing, bitwise ops, and fast iteration.\n\n"
    "Parameters\n"
    "----------\n"
    "size : int\n"
    "    Number of bits in the vector.\n\n"
    "Attributes\n"
    "----------\n"
    "bits : int\n"
    "    The length of this BitVector.\n");

/**
 * @brief Slot table for the PyBitVector type.
 *
 * Maps Python’s type callbacks (new, init, dealloc, repr, etc.)
 * and protocol slots (sequence, number, richcompare) to our C functions.
 *
 * @see PyType_Slot
 */
static PyType_Slot PyBitVector_slots[] = {
    {Py_tp_doc, (char *) BitVector__doc__},

    {Py_tp_new, py_bv_new},
    {Py_tp_init, py_bv_init},
    {Py_tp_dealloc, py_bv_free},
    {Py_tp_methods, BitVector_methods},
    {Py_tp_repr, py_bv_repr},
    {Py_tp_str, py_bv_str},
    {Py_tp_getset, PyBitVector_getset},
    {Py_tp_richcompare, py_bv_richcompare},
    {Py_tp_hash, py_bv_hash},

    {Py_tp_iter, py_bv_iter},
    {Py_mp_length, py_bv_len},
    {Py_mp_subscript, py_bv_subscript},
    {Py_mp_ass_subscript, py_bv_ass_subscript},
    {Py_sq_contains, py_bv_contains},

    {Py_nb_and, py_bv_and},
    {Py_nb_inplace_and, py_bv_iand},
    {Py_nb_or, py_bv_or},
    {Py_nb_inplace_or, py_bv_ior},
    {Py_nb_xor, py_bv_xor},
    {Py_nb_inplace_xor, py_bv_ixor},
    {Py_nb_invert, py_bv_invert},
    {Py_nb_bool, py_bv_bool},

    {0, 0},
};
/**
 * @brief Type specification for BitVector.
 *
 * This structure describes the Python type name, size,
 * inheritance flags, and slot table used to create the type.
 *
 * @see PyType_Spec
 */
PyType_Spec PyBitVector_spec = {
    .name = "cbits.BitVector",
    .basicsize = sizeof(PyBitVector),
    .itemsize = 0,
    .flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .slots = PyBitVector_slots,
};
