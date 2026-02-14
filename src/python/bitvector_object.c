/**
 * @file bitvector_object.c
 * @brief Implementation of the ``PyBitVectorObject`` Python type.
 *
 * Implements the Python wrapper type for the native ``BitVector`` structure.
 * This includes:
 * - type construction via ``PyType_Spec`` and slot tables
 * - ``__new__``, ``__init__``, and deallocation logic
 * - integration of all BitVector method groups and iterator support
 *
 * This file connects the C BitVector backend with Python’s object model.
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
#include "bitvector_methods_sequence.h"

/**
 * @brief ``__new__`` for ``BitVector``: allocate the Python object.
 *
 * Allocates a new ``PyBitVectorObject`` without initializing the underlying
 * native BitVector. Initialization is performed in ``py_bitvector_init``.
 *
 * @param type The Python type object.
 * @param args Unused positional arguments.
 * @param kwds Unused keyword arguments.
 * @retval new_object New ``PyBitVectorObject`` on success.
 * @retval NULL on allocation failure (exception set).
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
 * @brief ``__init__`` for ``BitVector(size)``: allocate the native BitVector.
 *
 * Parses the ``size`` argument, frees any existing BitVector, and allocates a
 * new one of the requested length.
 *
 * @param self A ``PyBitVectorObject`` instance.
 * @param args Positional arguments.
 * @param kwds Keyword arguments.
 * @retval 0 Success.
 * @retval -1 Failure (exception set).
 *
 * @warning Passing a negative size raises ``ValueError``.
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
 * @brief GC traverse callback for ``PyBitVectorObject``.
 *
 * Reports Python‑level references held by the object to the cyclic garbage
 * collector. Only the type object is visited, as the native BitVector does not
 * reference Python objects.
 *
 * @param bv The BitVector instance being traversed.
 * @param visit GC visit function.
 * @param arg Extra argument passed through by the GC.
 * @retval 0 Always returns ``0``.
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
 * @brief Deallocate a ``PyBitVectorObject``.
 *
 * Frees the underlying native BitVector (if present), clears internal state,
 * and releases the Python object memory.
 *
 * @param object A ``PyBitVectorObject`` instance.
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
/** @brief Docstring for the ``BitVector`` type. */
PyDoc_STRVAR(
    PyBitVector__doc__,
    "BitVector(size: int)\n"
    "\n"
    "A high-performance, fixed-size 1D bit array.\n\n"
    "Supports random access, slicing, bitwise ops, and fast iteration.\n\n"
    "Parameters\n"
    "----------\n"
    "size : int\n"
    "   Number of bits in the vector.\n\n"
    "Methods\n"
    "----------\n"
    "set(pos: int) -> None\n"
    "   Set the bit at the given position to 1.\n"
    "\n"
    "clear(pos: int) -> None\n"
    "   Clear the bit at the given position (set to 0).\n"
    "\n"
    "flip(pos: int) -> None\n"
    "   Toggle the bit at the given position.\n"
    "\n"
    "set_range(start: int, length: int) -> None\n"
    "   Set all bits in the half-open interval [start, start+length).\n"
    "\n"
    "clear_range(start: int, length: int) -> None\n"
    "   Clear all bits in the half-open interval [start, start+length).\n"
    "\n"
    "flip_range(start: int, length: int) -> None\n"
    "   Toggle all bits in the half-open interval [start, start+length).\n"
    "\n"
    "rank(pos: int) -> int\n"
    "   Return the number of set bits in the range [0, pos].\n"
    "\n"
    "copy() -> BitVector\n"
    "   Return a deep copy of the BitVector.\n"
    "\n"
    "contains(sub: BitVector) -> bool\n"
    "   Return True if 'sub' appears as a contiguous subvector.\n"
    "\n"
    "Attributes\n"
    "----------\n"
    "bits : int\n"
    "   The length of this BitVector.\n");

/**
 * @brief Member table for ``PyBitVectorObject``.
 *
 * Only weakref support is exposed (on Python < 3.12). Otherwise empty.
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
 * @brief Slot table for the ``PyBitVector`` type.
 *
 * Maps Python type slots (constructor, GC hooks, sequence protocol, number
 * protocol, etc.) to the corresponding C implementations.
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
    {Py_sq_concat, py_bitvector_concat},
    {Py_sq_repeat, py_bitvector_repeat},

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
 * @brief Type specification for ``BitVector``.
 *
 * Defines the type name, size, flags, and slot table used to construct the
 * Python type via ``PyType_FromModuleAndSpec``.
 *
 * @see PyType_Spec
 */
PyType_Spec PyBitVector_spec = {
    .name = "cbits.BitVector",
    .basicsize = sizeof(PyBitVectorObject),
    .flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE |
             Py_TPFLAGS_IMMUTABLETYPE | Py_TPFLAGS_HAVE_GC |
             Py_TPFLAGS_SEQUENCE
#if PY_VERSION_HEX >= 0x030C0000
             | Py_TPFLAGS_MANAGED_WEAKREF,
#endif
    .slots = BitVector_slots,
};
