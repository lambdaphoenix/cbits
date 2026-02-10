/**
 * @file src/python/bitvector_methods_slice.c
 * @brief Slicing support for the BitVector Python type.
 *
 * Implements __getitem__ and __setitem__ for slice objects as well as integer
 * indexing. Slice extraction uses fast bit‑shifting paths for contiguous
 * ranges and falls back to per‑bit copying for stepped slices. Slice
 * assignment accepts any iterable of truthy values and writes them into the
 * target range.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_methods_slice.h"
#include "bitvector_object.h"

/**
 * @brief Returns the boolean value of a single bit.
 *
 * @param object A Python PyBitVectorObject instance.
 * @param i Index to access
 * @return New reference to Py_True or Py_False
 *
 * @since 0.3.0
 */
static PyObject *
py_bitvector_get_item(PyObject *object, Py_ssize_t i)
{
    PyBitVectorObject *self = (PyBitVectorObject *) object;
    assert(i >= 0 && i < self->bv->n_bits);
    return PyBool_FromLong(bv__get_inline(self->bv, (size_t) i));
}
PyObject *
py_bitvector_item(PyObject *object, Py_ssize_t i)
{
    PyBitVectorObject *self = (PyBitVectorObject *) object;
    if (i < 0 || i >= self->bv->n_bits) {
        PyErr_SetString(PyExc_IndexError, "BitVector index out of range");
        return NULL;
    }
    return py_bitvector_get_item(object, i);
}
int
py_bitvector_ass_item(PyObject *object, Py_ssize_t i, PyObject *value)
{
    PyBitVectorObject *self = (PyBitVectorObject *) object;
    if (i < 0 || i >= self->bv->n_bits) {
        PyErr_SetString(PyExc_IndexError, "BitVector assignment out of range");
        return -1;
    }
    if (value == NULL) {
        bv__clear_inline(self->bv, (size_t) i);
    }
    else {
        int bit = PyObject_IsTrue(value);
        if (bit < 0) {
            return -1;
        }
        if (bit) {
            bv__set_inline(self->bv, (size_t) i);
        }
        else {
            bv__clear_inline(self->bv, (size_t) i);
        }
    }
    ((PyBitVectorObject *) self)->hash_cache = -1;
    return 0;
}

/**
 * @brief Implements slicing for BitVector.__getitem__ with a slice object.
 *
 * Creates and returns a new BitVector containing eleme
 * nts from
 * [start:stop:step]. Raises IndexError if any index is out of bounds.
 *
 * @param object A Python PyBitVectorObject instance.
 * @param start Start index of the slice.
 * @param stop End index (exclusive) of the slice.
 * @param step Step size for the slice.
 * @param slicelength Number of elements in the resulting slice.
 * @return New PyBitVectorObject wrapping the sliced BitVector; NULL and
 * IndexError on failure.
 */
static PyObject *
py_bitvector_slice(PyObject *object, size_t start, size_t stop, size_t step,
                   size_t slicelength)
{
    cbits_state *state = find_cbits_state_by_type(Py_TYPE(object));
    PyBitVectorObject *self = (PyBitVectorObject *) object;
    BitVector *src = self->bv;

    BitVector *out = bv_new(slicelength);
    if (!out) {
        return NULL;
    }
    if (step == 1 && slicelength > 0) {
        size_t s_word = start >> 6;
        unsigned s_off = (unsigned) (start & 63);
        size_t words_needed = out->n_words;

        uint64_t carry = 0;
        for (size_t j = 0; j < words_needed; ++j) {
            size_t aw = s_word + j;
            uint64_t lo = (aw < src->n_words) ? src->data[aw] : 0ULL;
            uint64_t hi = (aw + 1 < src->n_words) ? src->data[aw + 1] : 0ULL;

            uint64_t word = (lo >> s_off) | (hi << (64 - s_off));
            out->data[j] = word;
        }
        bv_apply_tail_mask(out);
        return bitvector_wrap_new(state->PyBitVectorType, out);
    }

    size_t n_bits = src->n_bits;
    for (size_t i = 0, idx = start; i < slicelength; ++i, idx += step) {
        if (n_bits <= idx) {
            bv_free(out);
            PyErr_SetString(PyExc_IndexError, "BitVector slice out of range");
            return NULL;
        }
        if (bv__get_inline(src, idx)) {
            bv__set_inline(out, i);
        }
    }

    return bitvector_wrap_new(state->PyBitVectorType, out);
}
/**
 * @brief Implements BitVector.__setitem__ for slice assignment.
 *
 * Assigns bits from an iterable `value` to the slice [start:stop:step]. Raises
 * IndexError or ValueError on length mismatch or out-of-range.
 *
 * @param self A Python PyBitVectorObject instance.
 * @param start Start index of the slice.
 * @param stop End index (exclusive) of the slice.
 * @param step Step size for the slice.
 * @param slicelength Number of elements in the resulting slice.
 * @param value Iterable of boolean-convertible Python objects.
 * @return 0 on success; -1 on error (with exception set).
 */
static int
py_bitvector_ass_slice(PyObject *self, size_t start, size_t stop, size_t step,
                       size_t slicelength, PyObject *value)
{
    BitVector *bv = ((PyBitVectorObject *) self)->bv;

    PyObject *seq =
        PySequence_Fast(value, "can only assign iterable to BitVector slice");
    if (!seq) {
        return -1;
    }

    Py_ssize_t vlen = PySequence_Fast_GET_SIZE(seq);
    if ((size_t) vlen != slicelength) {
        Py_DECREF(seq);
        PyErr_Format(PyExc_ValueError,
                     "attempt to assign sequence of length %zd "
                     "to slice of length %zu",
                     vlen, slicelength);
        return -1;
    }

    PyObject **items = PySequence_Fast_ITEMS(seq);

    size_t n_bits = bv->n_bits;
    for (size_t i = 0, idx = start; i < slicelength; ++i, idx += step) {
        if (n_bits <= idx) {
            Py_DECREF(seq);
            PyErr_SetString(PyExc_IndexError, "BitVector slice out of range");
            return -1;
        }
        int bit = PyObject_IsTrue(items[i]);
        if (bit < 0) {
            Py_DECREF(seq);
            return -1;
        }
        if (bit) {
            bv__set_inline(bv, idx);
        }
        else {
            bv__clear_inline(bv, idx);
        }
        ((PyBitVectorObject *) self)->hash_cache = -1;
    }

    Py_DECREF(seq);
    return 0;
}

PyObject *
py_bitvector_subscript(PyObject *object, PyObject *arg)
{
    PyBitVectorObject *self = (PyBitVectorObject *) object;
    cbits_state *state = find_cbits_state_by_type(Py_TYPE(self));

    if (PyIndex_Check(arg)) {
        Py_ssize_t idx = PyNumber_AsSsize_t(arg, PyExc_IndexError);
        if (idx == -1 && PyErr_Occurred()) {
            return NULL;
        }
        if (idx < 0) {
            idx += self->bv->n_bits;
        }
        return py_bitvector_item(object, idx);
    }

    if (PySlice_Check(arg)) {
        Py_ssize_t start, stop, step, slicelength, needed;
        if (PySlice_Unpack(arg, &start, &stop, &step) < 0) {
            return NULL;
        }
        slicelength =
            PySlice_AdjustIndices(self->bv->n_bits, &start, &stop, step);
        return py_bitvector_slice(object, start, stop, step, slicelength);
    }

    PyErr_Format(PyExc_TypeError,
                 "indices must be integers or slices, not %.200s",
                 Py_TYPE(arg)->tp_name);
    return NULL;
}

int
py_bitvector_ass_subscript(PyObject *object, PyObject *arg, PyObject *value)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError,
                        "BitVector does not support item deletion");
        return -1;
    }
    PyBitVectorObject *self = (PyBitVectorObject *) object;

    if (PyIndex_Check(arg)) {
        Py_ssize_t idx = PyNumber_AsSsize_t(arg, PyExc_IndexError);

        if (idx == -1 && PyErr_Occurred()) {
            return -1;
        }
        if (idx < 0) {
            idx += self->bv->n_bits;
        }
        return py_bitvector_ass_item(object, idx, value);
    }
    else if (PySlice_Check(arg)) {
        Py_ssize_t start, stop, step, slicelength, needed;
        if (PySlice_Unpack(arg, &start, &stop, &step) < 0) {
            return -1;
        }
        slicelength =
            PySlice_AdjustIndices(self->bv->n_bits, &start, &stop, step);
        return py_bitvector_ass_slice(object, start, stop, step, slicelength,
                                      value);
    }
    else {
        PyErr_SetString(PyExc_TypeError, "BitVector indices must be integers");
        return -1;
    }
}
