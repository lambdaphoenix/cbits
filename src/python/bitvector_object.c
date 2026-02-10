/**
 * @file src/python/bitvector_object.c
 * @brief Implementation of the PyBitVectorObject Python type.
 *
 * Defines:
 * - type initialization via PyType_Spec
 * - py_bitvector_new, py_bitvector_init, py_bitvector_dealloc
 * - integration of all method groups and iterator support
 *
 * This file binds the C BitVector backend to the Python's objet module.
 *
 * @see bitvector_object.h
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "cbits_state.h"
#include "bitvector_object.h"

#include <stddef.h>
#include <structmember.h>

#include "bitvector_methods.h"
#include "bitvector_methods_misc.h"
#include "bitvector_methods_compare.h"
#include "bitvector_methods_slice.h"
#include "bitvector_methods_ops.h"
#include "bitvector_iter.h"

/**
 * @brief __new__ for BitVector: allocate the Python object.
 * @param type The Python type object.
 * @param args Positional args (unused).
 * @param kwds Keyword args (unused).
 * @return New, uninitialized PyBitVectorObject or NULL on failure.
 */
PyObject *
py_bitvector_new(PyTypeObject *type, PyObject *Py_UNUSED(args),
                 PyObject *Py_UNUSED(kwds))
{
    PyBitVectorObject *bvself = (PyBitVectorObject *) type->tp_alloc(type, 0);
    if (!bvself) {
        return NULL;
    }
    bvself->bv = NULL;
    bvself->hash_cache = -1;
    return (PyObject *) bvself;
}

PyObject *
bitvector_wrap_new(PyTypeObject *type, BitVector *bv_data)
{
    assert(type != NULL);
    assert(bv_data != NULL);

    PyObject *object_new = py_bitvector_new(type, NULL, NULL);
    if (object_new == NULL) {
        bv_free(bv_data);
        return NULL;
    }

    PyBitVectorObject *bv_object = (PyBitVectorObject *) object_new;

    bv_object->bv = bv_data;
    return object_new;
}

/**
 * @brief __init__ for BitVector(size): allocate the underlying C BitVector.
 * @param self A Python PyBitVectorObject instance.
 * @param args Positional args tuple.
 * @param kwds Keyword args dict.
 * @return 0 on success, -1 on error (with exception set).
 */
static int
py_bitvector_init(PyObject *self, PyObject *args, PyObject *kwds)
{
    Py_ssize_t n_bits;
    static char *kwlist[] = {"size", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "n", kwlist, &n_bits)) {
        return -1;
    }
    if (n_bits < 0) {
        PyErr_SetString(PyExc_ValueError, "size must be >= 0");
        return -1;
    }
    PyBitVectorObject *bvself = (PyBitVectorObject *) self;

    if (bvself->bv != NULL) {
        bv_free(bvself->bv);
        bvself->bv = NULL;
    }

    bvself->bv = bv_new((size_t) n_bits);
    if (!bvself->bv) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate BitVector");
        return -1;
    }
    return 0;
}

/**
 * @brief Traverse callback for PyBitVectorObject.
 *
 * Participates in Python's cyclic garbage collector by visiting all Python
 * objects referenced by the BitVector instance. Only the type object is
 * visited here, as the BitVector does not own additional Python-level
 * references.
 *
 * @param bv BitVector instance being traversed.
 * @param visit GC visit function supplied by the interpreter.
 * @param arg Extra argument passed through by the GC.
 * @return Always 0 to indicate success.
 *
 * @since 0.3.0
 */

static int
py_bitvector_traverse(PyBitVectorObject *bv, visitproc visit, void *arg)
{
    Py_VISIT(Py_TYPE(bv));
    return 0;
}

/**
 * @brief Deallocate a PyBitVectorObject object.
 * @param object A Python PyBitVectorObject instance.
 */
static void
py_bitvector_dealloc(PyObject *object)
{
    PyTypeObject *type = Py_TYPE(object);
    PyObject_GC_UnTrack(object);

    PyBitVectorObject *self = (PyBitVectorObject *) object;
    if (self->bv) {
        bv_free(self->bv);
        self->bv = NULL;
    }
    type->tp_free(self);
    Py_DECREF(type);
}
/** @brief Docstring for BitVector. */
PyDoc_STRVAR(
    PyBitVector__doc__,
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
 * @brief Member table for PyBitVectorObject.
 *
 * The BitVector type does not expose any struct‑level members to Python, so
 * the table contains only the NULL terminator.
 *
 * @since 0.3.0
 */
static struct PyMemberDef py_bitvector_members[] = {
#if PY_VERSION_HEX < 0x030C0000
    {"__weaklistoffset__", T_PYSSIZET,
     offsetof(PyBitVectorObject, weakreflist), READONLY},
#endif
    {NULL}};

/**
 * @brief Slot table for the PyBitVectorObject type.
 *
 * Maps Python’s type callbacks (new, init, dealloc, repr, etc.)
 * and protocol slots (sequence, number, richcompare) to our C functions.
 *
 * @see PyType_Slot
 */
static PyType_Slot BitVector_slots[] = {
    {Py_tp_doc, (void *) PyBitVector__doc__},

    {Py_tp_alloc, PyType_GenericAlloc},
    {Py_tp_new, py_bitvector_new},
    {Py_tp_traverse, py_bitvector_traverse},
    {Py_tp_init, py_bitvector_init},
    {Py_tp_dealloc, py_bitvector_dealloc},
    {Py_tp_getattro, PyObject_GenericGetAttr},
    {Py_tp_methods, BitVector_methods},
    {Py_tp_members, py_bitvector_members},
    {Py_tp_repr, py_bitvector_repr},
    {Py_tp_str, py_bitvector_str},
    {Py_tp_getset, PyBitVector_getset},
    {Py_tp_richcompare, py_bitvector_richcompare},
    {Py_tp_hash, py_bitvector_hash},
    {Py_tp_iter, py_bitvector_iter},

    {Py_mp_length, py_bitvector_len},
    {Py_mp_subscript, py_bitvector_subscript},
    {Py_mp_ass_subscript, py_bitvector_ass_subscript},

    {Py_sq_length, py_bitvector_len},
    {Py_sq_item, py_bitvector_item},
    {Py_sq_ass_item, py_bitvector_ass_item},
    {Py_sq_contains, py_bitvector_contains},

    {Py_nb_and, py_bitvector_and},
    {Py_nb_inplace_and, py_bitvector_iand},
    {Py_nb_or, py_bitvector_or},
    {Py_nb_inplace_or, py_bitvector_ior},
    {Py_nb_xor, py_bitvector_xor},
    {Py_nb_inplace_xor, py_bitvector_ixor},
    {Py_nb_invert, py_bitvector_invert},
    {Py_nb_bool, py_bitvector_bool},

    {0, NULL},
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
    .basicsize = sizeof(PyBitVectorObject),
#if PY_VERSION_HEX >= 0x030C0000
    .flags = (Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE |
              Py_TPFLAGS_IMMUTABLETYPE | Py_TPFLAGS_HAVE_GC |
              Py_TPFLAGS_SEQUENCE | Py_TPFLAGS_MANAGED_WEAKREF),
#else
    .flags =
        (Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_IMMUTABLETYPE |
         Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_SEQUENCE),
#endif
    .slots = BitVector_slots,
};
