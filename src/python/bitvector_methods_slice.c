#include "bitvector_methods_slice.h"
#include "bitvector_object.h"

/**
 * @brief Implements BitVector.__getitem__, returns the bit at position i.
 *
 * This function checks bounds and returns the corresponding Python boolean
 * (True/False). On out-of-range access it raises IndexError.
 *
 * @param self A Python PyBitVector instance.
 * @param i Index to access
 * @return New reference to Py_True or Py_False on success; NULL and IndexError
 * on failure.
 */
static PyObject *
py_bv_item(PyObject *self, Py_ssize_t i)
{
    BitVector *bv = ((PyBitVector *) self)->bv;
    if (!bv || bv->n_bits <= (size_t) i) {
        PyErr_SetString(PyExc_IndexError, "BitVector index out of range");
        return NULL;
    }
    return PyBool_FromLong(bv__get_inline(bv, (size_t) i));
}

/**
 * @brief Implements slicing for BitVector.__getitem__ with a slice object.
 *
 * Creates and returns a new BitVector containing elements from
 * [start:stop:step]. Raises IndexError if any index is out of bounds.
 *
 * @param self A Python PyBitVector instance.
 * @param start Start index of the slice.
 * @param stop End index (exclusive) of the slice.
 * @param step Step size for the slice.
 * @param slicelength Number of elements in the resulting slice.
 * @return New PyBitVector wrapping the sliced BitVector; NULL and IndexError
 * on failure.
 */
static PyObject *
py_bv_slice(PyObject *self, size_t start, size_t stop, size_t step,
            size_t slicelength)
{
    PyBitVector *pbv = (PyBitVector *) self;
    BitVector *src = pbv->bv;

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
        return bv_wrap_new(out);
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

    return bv_wrap_new(out);
}

/**
 * @brief Implements BitVector.__setitem__ for a single index.
 *
 * Sets or clears the bit at position i based on the truth value of `value`.
 * Raises IndexError if the index is out of range.
 *
 * @param self A Python PyBitVector instance.
 * @param i Index of the bit to assign.
 * @param value Python object interpreted as boolean.
 * @return 0 on success; -1 on error (with exception set).
 */
static int
py_bv_ass_item(PyObject *self, Py_ssize_t i, PyObject *value)
{
    BitVector *bv = ((PyBitVector *) self)->bv;
    if (!bv || bv->n_bits <= (size_t) i) {
        PyErr_SetString(PyExc_IndexError,
                        "BitVector assignment index out of range");
        return -1;
    }
    int bit = PyObject_IsTrue(value);
    if (bit < 0) {
        return -1;
    }
    if (bit) {
        bv__set_inline(bv, (size_t) i);
    }
    else {
        bv__clear_inline(bv, (size_t) i);
    }
    ((PyBitVector *) self)->hash_cache = -1;
    return 0;
}

/**
 * @brief Implements BitVector.__setitem__ for slice assignment.
 *
 * Assigns bits from an iterable `value` to the slice [start:stop:step]. Raises
 * IndexError or ValueError on length mismatch or out-of-range.
 *
 * @param self A Python PyBitVector instance.
 * @param start Start index of the slice.
 * @param stop End index (exclusive) of the slice.
 * @param step Step size for the slice.
 * @param slicelength Number of elements in the resulting slice.
 * @param value Iterable of boolean-convertible Python objects.
 * @return 0 on success; -1 on error (with exception set).
 */
static int
py_bv_ass_slice(PyObject *self, size_t start, size_t stop, size_t step,
                size_t slicelength, PyObject *value)
{
    BitVector *bv = ((PyBitVector *) self)->bv;

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
        ((PyBitVector *) self)->hash_cache = -1;
    }

    Py_DECREF(seq);
    return 0;
}

PyObject *
py_bv_subscript(PyObject *self, PyObject *arg)
{
    if (PyIndex_Check(arg)) {
        Py_ssize_t idx = PyNumber_AsSsize_t(arg, PyExc_IndexError);
        if (idx == -1 && PyErr_Occurred()) {
            return NULL;
        }
        return py_bv_item(self, idx);
    }

    if (PySlice_Check(arg)) {
        PyObject *slice = (PyObject *) arg;
        PyBitVector *bv = (PyBitVector *) self;
        Py_ssize_t start, stop, step, slicelength;
        if (PySlice_GetIndicesEx(slice, bv->bv->n_bits, &start, &stop, &step,
                                 &slicelength) < 0) {
            return NULL;
        }
        return py_bv_slice(self, (size_t) start, (size_t) stop, (size_t) step,
                           (size_t) slicelength);
    }

    PyErr_Format(PyExc_TypeError,
                 "indices must be integers or slices, not %.200s",
                 Py_TYPE(arg)->tp_name);
    return NULL;
}

int
py_bv_ass_subscript(PyObject *self, PyObject *arg, PyObject *value)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError,
                        "BitVector does not support item deletion");
        return -1;
    }

    if (PyIndex_Check(arg)) {
        Py_ssize_t idx = PyNumber_AsSsize_t(arg, PyExc_IndexError);
        if (idx == -1 && PyErr_Occurred()) {
            return -1;
        }
        return py_bv_ass_item(self, idx, value);
    }

    if (PySlice_Check(arg)) {
        PyObject *slice = (PyObject *) arg;
        PyBitVector *bv = (PyBitVector *) self;
        Py_ssize_t start, stop, step, slicelength;
        if (PySlice_GetIndicesEx(slice, bv->bv->n_bits, &start, &stop, &step,
                                 &slicelength) < 0) {
            return -1;
        }
        return py_bv_ass_slice(self, (size_t) start, (size_t) stop,
                               (size_t) step, (size_t) slicelength, value);
    }

    PyErr_Format(PyExc_TypeError,
                 "indices must be integers or slices, not %.200s",
                 Py_TYPE(arg)->tp_name);
    return -1;
}