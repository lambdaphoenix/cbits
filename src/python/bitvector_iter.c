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

/**
 * @struct PyBitVectorIter
 * @brief Iterator structure for PyBitVector
 *
 * Stores a reference to the original PyBitVector and tracks
 * the current bit position and buffer state for iteration.
 */
typedef struct {
    PyObject_HEAD PyBitVector
        *bv;               /**< Reference to the PyBitVector being iterated */
    size_t n_bits;         /**< Total number of bits in the vector */
    size_t position;       /**< Current bit index (0-based) */
    size_t word_index;     /**< Index into the 64-bit word array */
    uint64_t current_word; /**< Local copy of the active 64-bit word */
    uint64_t mask;         /**< Bit mask for next bit */
} PyBitVectorIter;

void
py_bviter_dealloc(PyObject *self)
{
    PyBitVectorIter *iter = (PyBitVectorIter *) self;
    Py_XDECREF(iter->bv);
    Py_TYPE(iter)->tp_free(self);
}

PyObject *
py_bviter_iternext(PyObject *self)
{
    PyBitVectorIter *iter = (PyBitVectorIter *) self;

    if (iter->position >= iter->n_bits) {
        PyErr_SetNone(PyExc_StopIteration);
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

PyObject *
py_bv_iter(PyObject *self)
{
    PyBitVector *bv = (PyBitVector *) self;
    PyBitVectorIter *iter = PyObject_New(PyBitVectorIter, PyBitVectorIterType);
    if (!iter) {
        return NULL;
    }
    Py_INCREF(bv);
    iter->bv = bv;
    iter->n_bits = bv->bv->n_bits;
    iter->position = 0;
    iter->word_index = 0;
    iter->current_word = 0;
    iter->mask = 0;

    return (PyObject *) iter;
}

/**
 * @brief Slots for the _BitVectorIter type.
 *
 * Defines deallocator, __iter__ and __next__.
 */
static PyType_Slot PyBitVectorIter_slots[] = {
    {Py_tp_dealloc, py_bviter_dealloc},
    {Py_tp_iter, PyObject_SelfIter},
    {Py_tp_iternext, py_bviter_iternext},
    {0, 0},
};

/**
 * @brief Type specification for cbits._BitVectorIter.
 *
 * This is the bit‐wise iterator returned by BitVector.__iter__().
 */
PyType_Spec PyBitVectorIter_spec = {
    .name = "cbits._BitVectorIter",
    .basicsize = sizeof(PyBitVectorIter),
    .flags = Py_TPFLAGS_DEFAULT,
    .slots = PyBitVectorIter_slots,
};
