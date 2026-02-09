/**
 * @file src/python/bitvector_iter.c
 * @brief Implementation of BitVector iterator.
 *
 * Implements the iterator returned by BitVector.__iter__(). Iteration proceeds
 * bit‑by‑bit using a cached 64‑bit word and a shifting mask to minimize
 * indexing overhead and reduce Python/C boundary calls.
 *
 * @see bitvector_object.h
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_iter.h"
#include "cbits_module.h"

/**
 * @struct BitVectorIterObject
 * @brief Iterator structure for PyBitVectorObject
 *
 * Stores a reference to the original PyBitVectorObject and tracks
 * the current bit position and buffer state for iteration.
 */
typedef struct PyBitVectorIterObject {
    PyObject_HEAD PyBitVectorObject
        *bv;       /**< Reference to the PyBitVectorObject being iterated */
    size_t n_bits; /**< Total number of bits in the vector */
    Py_ssize_t position;   /**< Current bit index (0-based) */
    size_t word_index;     /**< Index into the 64-bit word array */
    uint64_t current_word; /**< Local copy of the active 64-bit word */
    uint64_t mask;         /**< Bit mask for next bit */
} PyBitVectorIterObject;

PyObject *
py_bitvector_iter(PyObject *self)
{
    PyBitVectorObject *bv = (PyBitVectorObject *) self;
    cbits_state *state = find_cbits_state_by_type(Py_TYPE(self));

    if (!py_bitvector_check(self, state)) {
        PyErr_BadInternalCall();
        return NULL;
    }

    PyBitVectorIterObject *iter =
        PyObject_GC_New(PyBitVectorIterObject, state->PyBitVectorIterType);
    if (iter == NULL) {
        return NULL;
    }

    iter->bv = (PyBitVectorObject *) Py_NewRef(bv);
    iter->n_bits = bv->bv->n_bits;
    iter->position = 0;
    iter->word_index = 0;
    iter->current_word = 0;
    iter->mask = 0;

    PyObject_GC_Track(iter);
    return (PyObject *) iter;
}
/**
 * @brief Deallocate a BitVector iterator object.
 *
 * Releases the reference to the parent PyBitVectorObject and frees the
 * iterator struct.
 *
 * @param self A PyBitVectorIter instance.
 */
static void
py_bitvectoriter_dealloc(PyObject *self)
{
    PyBitVectorIterObject *iter = (PyBitVectorIterObject *) self;
    PyTypeObject *type = Py_TYPE(iter);
    PyObject_GC_UnTrack(iter);
    Py_XDECREF(iter->bv);
    PyObject_GC_Del(iter);
    Py_DECREF(type);
}

static int
py_bitvectoriter_traverse(PyObject *self, visitproc visit, void *arg)
{
    PyBitVectorIterObject *iter = ((PyBitVectorIterObject *) self);
    Py_VISIT(Py_TYPE(iter));
    Py_VISIT(iter->bv);
    return 0;
}
/**
 * @brief Return the next bit as a Python boolean.
 *
 * Reads one bit from the internal buffer and shifts it out. If all bits have
 * been yielded, raises StopIteration.
 *
 * @param self A PyBitVectorIter instance.
 * @return Py_True or Py_False on success; NULL with StopIteration set at
 * end-of-iteration.
 */
static PyObject *
py_bitvectoriter_next(PyObject *self)
{
    PyBitVectorIterObject *iter = (PyBitVectorIterObject *) self;
    assert(iter != NULL);

    PyBitVectorObject *bv = iter->bv;
    if (bv == NULL) {
        return NULL;
    }

    if (iter->position >= iter->n_bits) {
        iter->bv = NULL;
        Py_DECREF(bv);
        return NULL;
    }

    if (iter->mask == 0) {
        if (iter->word_index >= iter->bv->bv->n_words) {
            PyErr_SetNone(PyExc_StopIteration);
            return NULL;
        }
        iter->current_word = iter->bv->bv->data[iter->word_index++];
        iter->mask = 1ULL;

        cbits_prefetch(&iter->bv->bv->data[iter->word_index]);
    }

    int bit = (iter->current_word & iter->mask) != 0;
    iter->mask <<= 1;
    iter->position++;

    if (bit) {
        Py_RETURN_TRUE;
    }
    else {
        Py_RETURN_FALSE;
    }
}

static PyMethodDef py_bitvectoriter_methods[] = {{NULL, NULL}};

/**
 * @brief Slots for the _BitVectorIter type.
 *
 * Defines deallocator, __iter__ and __next__.
 */
static PyType_Slot PyBitVectorIter_slots[] = {
    {Py_tp_dealloc, py_bitvectoriter_dealloc},
    {Py_tp_getattro, PyObject_GenericGetAttr},
    {Py_tp_traverse, py_bitvectoriter_traverse},
    {Py_tp_iter, PyObject_SelfIter},
    {Py_tp_iternext, py_bitvectoriter_next},
    {Py_tp_methods, py_bitvectoriter_methods},
    {0, NULL},
};

/**
 * @brief Type specification for cbits._BitVectorIter.
 *
 * This is the bit‐wise iterator returned by BitVector.__iter__().
 */
PyType_Spec PyBitVectorIter_spec = {
    .name = "cbits._BitVectorIterator",
    .basicsize = sizeof(PyBitVectorIterObject),
    .flags = (Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC |
              Py_TPFLAGS_DISALLOW_INSTANTIATION | Py_TPFLAGS_IMMUTABLETYPE),
    .slots = PyBitVectorIter_slots,
};
