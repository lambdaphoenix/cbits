/**
 * @file src/python/bitvector_methods_ops.c
 * @brief Bitwise operation methods for BitVector.
 *
 * Implements Python bindings for all bitwise operators: AND, OR, XOR, their
 * in‑place variants, bitwise NOT, and truth‑value testing. Operations are
 * performed word‑wise using the native C backend for high throughput on large
 * vectors.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_methods_ops.h"
#include "bitvector_object.h"

PyObject *
py_bitvector_and(PyObject *oA, PyObject *oB)
{
    PyBitVectorObject *A = (PyBitVectorObject *) oA;
    cbits_state *state = find_cbits_state_by_type(Py_TYPE(A));

    if (!py_bitvector_check(oA, state) || !py_bitvector_check(oB, state)) {
        Py_RETURN_NOTIMPLEMENTED;
    }
    PyBitVectorObject *B = (PyBitVectorObject *) oB;

    size_t size = A->bv->n_bits;
    if (size != B->bv->n_bits) {
        PyErr_Format(PyExc_ValueError, "length mismatch: A=%zu, B=%zu", size,
                     B->bv->n_bits);
        return NULL;
    }
    BitVector *C = bv_new(size);
    if (!C) {
        PyErr_SetString(PyExc_MemoryError,
                        "BitVector allocation failed in __and__");
        return NULL;
    }

    uint64_t *restrict a = A->bv->data;
    uint64_t *restrict b = B->bv->data;
    uint64_t *restrict c = C->data;

    size_t i = 0;
    for (; i + 3 < A->bv->n_words; i += 4) {
        cbits_prefetch(&a[i + 16]);
        cbits_prefetch(&b[i + 16]);

        c[i] = a[i] & b[i];
        c[i + 1] = a[i + 1] & b[i + 1];
        c[i + 2] = a[i + 2] & b[i + 2];
        c[i + 3] = a[i + 3] & b[i + 3];
    }
    for (; i < A->bv->n_words; ++i) {
        c[i] = a[i] & b[i];
    }
    bv_apply_tail_mask(C);
    return bitvector_wrap_new(state->PyBitVectorType, C);
}

PyObject *
py_bitvector_iand(PyObject *self, PyObject *arg)
{
    PyBitVectorObject *A = (PyBitVectorObject *) self;
    cbits_state *state = find_cbits_state_by_type(Py_TYPE(A));

    if (!py_bitvector_check(self, state)) {
        PyErr_SetString(PyExc_TypeError, "Expected BitVector");
        return NULL;
    }
    PyBitVectorObject *B = (PyBitVectorObject *) arg;

    size_t size = A->bv->n_bits;
    if (size != B->bv->n_bits) {
        PyErr_Format(PyExc_ValueError, "length mismatch: A=%zu, B=%zu", size,
                     B->bv->n_bits);
        return NULL;
    }

    uint64_t *restrict a = A->bv->data;
    uint64_t *restrict b = B->bv->data;

    size_t i = 0;
    for (; i + 3 < A->bv->n_words; i += 4) {
        cbits_prefetch(&a[i + 16]);
        cbits_prefetch(&b[i + 16]);

        a[i] &= b[i];
        a[i + 1] &= b[i + 1];
        a[i + 2] &= b[i + 2];
        a[i + 3] &= b[i + 3];
    }
    for (; i < A->bv->n_words; ++i) {
        a[i] &= b[i];
    }
    bv_apply_tail_mask(A->bv);
    A->bv->rank_dirty = true;
    A->hash_cache = -1;
    Py_INCREF(self);
    return self;
}

PyObject *
py_bitvector_or(PyObject *oA, PyObject *oB)
{
    PyBitVectorObject *A = (PyBitVectorObject *) oA;
    cbits_state *state = find_cbits_state_by_type(Py_TYPE(A));

    if (!py_bitvector_check(oA, state) || !py_bitvector_check(oB, state)) {
        Py_RETURN_NOTIMPLEMENTED;
    }
    PyBitVectorObject *B = (PyBitVectorObject *) oB;

    size_t size = A->bv->n_bits;
    if (size != B->bv->n_bits) {
        PyErr_Format(PyExc_ValueError, "length mismatch: A=%zu, B=%zu", size,
                     B->bv->n_bits);
        return NULL;
    }
    BitVector *C = bv_new(size);
    if (!C) {
        PyErr_SetString(PyExc_MemoryError,
                        "BitVector allocation failed in __or__");
        return NULL;
    }

    uint64_t *restrict a = A->bv->data;
    uint64_t *restrict b = B->bv->data;
    uint64_t *restrict c = C->data;

    size_t i = 0;
    for (; i + 3 < A->bv->n_words; i += 4) {
        cbits_prefetch(&a[i + 16]);
        cbits_prefetch(&b[i + 16]);

        c[i] = a[i] | b[i];
        c[i + 1] = a[i + 1] | b[i + 1];
        c[i + 2] = a[i + 2] | b[i + 2];
        c[i + 3] = a[i + 3] | b[i + 3];
    }
    for (; i < A->bv->n_words; ++i) {
        c[i] = a[i] | b[i];
    }
    bv_apply_tail_mask(C);
    return bitvector_wrap_new(state->PyBitVectorType, C);
}

PyObject *
py_bitvector_ior(PyObject *self, PyObject *arg)
{
    PyBitVectorObject *A = (PyBitVectorObject *) self;
    cbits_state *state = find_cbits_state_by_type(Py_TYPE(A));

    if (!py_bitvector_check(self, state)) {
        PyErr_SetString(PyExc_TypeError, "Expected BitVector");
        return NULL;
    }
    PyBitVectorObject *B = (PyBitVectorObject *) arg;

    size_t size = A->bv->n_bits;
    if (size != B->bv->n_bits) {
        PyErr_Format(PyExc_ValueError, "length mismatch: A=%zu, B=%zu", size,
                     B->bv->n_bits);
        return NULL;
    }

    uint64_t *restrict a = A->bv->data;
    uint64_t *restrict b = B->bv->data;
    size_t i = 0;
    for (; i + 3 < A->bv->n_words; i += 4) {
        cbits_prefetch(&a[i + 16]);
        cbits_prefetch(&b[i + 16]);

        a[i] |= b[i];
        a[i + 1] |= b[i + 1];
        a[i + 2] |= b[i + 2];
        a[i + 3] |= b[i + 3];
    }
    for (; i < A->bv->n_words; ++i) {
        a[i] |= b[i];
    }
    bv_apply_tail_mask(A->bv);
    A->bv->rank_dirty = true;
    A->hash_cache = -1;
    Py_INCREF(self);
    return self;
}

PyObject *
py_bitvector_xor(PyObject *oA, PyObject *oB)
{
    PyBitVectorObject *A = (PyBitVectorObject *) oA;
    cbits_state *state = find_cbits_state_by_type(Py_TYPE(A));

    if (!py_bitvector_check(oA, state) || !py_bitvector_check(oB, state)) {
        Py_RETURN_NOTIMPLEMENTED;
    }
    PyBitVectorObject *B = (PyBitVectorObject *) oB;

    size_t size = A->bv->n_bits;
    if (size != B->bv->n_bits) {
        PyErr_Format(PyExc_ValueError, "length mismatch: A=%zu, B=%zu", size,
                     B->bv->n_bits);
        return NULL;
    }
    BitVector *C = bv_new(size);
    if (!C) {
        PyErr_SetString(PyExc_MemoryError,
                        "BitVector allocation failed in __xor__");
        return NULL;
    }

    uint64_t *restrict a = A->bv->data;
    uint64_t *restrict b = B->bv->data;
    uint64_t *restrict c = C->data;

    size_t i = 0;
    for (; i + 3 < A->bv->n_words; i += 4) {
        cbits_prefetch(&a[i + 16]);
        cbits_prefetch(&b[i + 16]);

        c[i] = a[i] ^ b[i];
        c[i + 1] = a[i + 1] ^ b[i + 1];
        c[i + 2] = a[i + 2] ^ b[i + 2];
        c[i + 3] = a[i + 3] ^ b[i + 3];
    }
    for (; i < A->bv->n_words; ++i) {
        c[i] = a[i] ^ b[i];
    }
    bv_apply_tail_mask(C);
    return bitvector_wrap_new(state->PyBitVectorType, C);
}

PyObject *
py_bitvector_ixor(PyObject *self, PyObject *arg)
{
    PyBitVectorObject *A = (PyBitVectorObject *) self;
    cbits_state *state = find_cbits_state_by_type(Py_TYPE(A));

    if (!py_bitvector_check(self, state)) {
        PyErr_SetString(PyExc_TypeError, "Expected BitVector");
        return NULL;
    }
    PyBitVectorObject *B = (PyBitVectorObject *) arg;

    size_t size = A->bv->n_bits;
    if (size != B->bv->n_bits) {
        PyErr_Format(PyExc_ValueError, "length mismatch: A=%zu, B=%zu", size,
                     B->bv->n_bits);
        return NULL;
    }

    uint64_t *restrict a = A->bv->data;
    uint64_t *restrict b = B->bv->data;
    size_t i = 0;
    for (; i + 3 < A->bv->n_words; i += 4) {
        cbits_prefetch(&a[i + 16]);
        cbits_prefetch(&b[i + 16]);

        a[i] ^= b[i];
        a[i + 1] ^= b[i + 1];
        a[i + 2] ^= b[i + 2];
        a[i + 3] ^= b[i + 3];
    }
    for (; i < A->bv->n_words; ++i) {
        a[i] ^= b[i];
    }
    bv_apply_tail_mask(A->bv);
    A->bv->rank_dirty = true;
    A->hash_cache = -1;
    Py_INCREF(self);
    return self;
}

PyObject *
py_bitvector_invert(PyObject *self)
{
    PyBitVectorObject *A = (PyBitVectorObject *) self;
    cbits_state *state = find_cbits_state_by_type(Py_TYPE(A));

    BitVector *C = bv_new(A->bv->n_bits);
    if (!C) {
        PyErr_SetString(PyExc_MemoryError,
                        "BitVector allocation failed in __invert__");
        return NULL;
    }
    uint64_t *restrict a = A->bv->data;
    uint64_t *restrict c = C->data;

    size_t i = 0;
    for (; i + 3 < A->bv->n_words; i += 4) {
        cbits_prefetch(&a[i + 16]);

        c[i] = ~a[i];
        c[i + 1] = ~a[i + 1];
        c[i + 2] = ~a[i + 2];
        c[i + 3] = ~a[i + 3];
    }
    for (; i < A->bv->n_words; ++i) {
        c[i] = ~a[i];
    }
    bv_apply_tail_mask(C);
    return bitvector_wrap_new(state->PyBitVectorType, C);
}

int
py_bitvector_bool(PyObject *self)
{
    PyBitVectorObject *bvself = (PyBitVectorObject *) self;
    return bv_rank(bvself->bv, bvself->bv->n_bits - 1) > 0;
}
