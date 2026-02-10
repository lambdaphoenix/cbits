/**
 * @file src/python/bitvector_methods_compare.c
 * @brief Equality and hashing for PyBitVectorObject.
 *
 * Implements ==, !=, and __hash__ using the C backend’s comparison routines
 * and Python’s byte hashing. Hash values are cached until the BitVector is
 * mutated.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_methods_compare.h"

PyObject *
py_bitvector_richcompare(PyObject *a, PyObject *b, int op)
{
    if (op != Py_EQ && op != Py_NE) {
        Py_RETURN_NOTIMPLEMENTED;
    }

    cbits_state *state = find_cbits_state_by_type(Py_TYPE(a));

    if (!py_bitvector_check(a, state) || !py_bitvector_check(b, state)) {
        Py_RETURN_NOTIMPLEMENTED;
    }

    BitVector *A = ((PyBitVectorObject *) a)->bv;
    BitVector *B = ((PyBitVectorObject *) b)->bv;
    bool eq =
        bv_equal(((PyBitVectorObject *) a)->bv, ((PyBitVectorObject *) b)->bv);
    if ((op == Py_EQ) == eq) {
        return Py_NewRef(Py_True);
    }
    return Py_NewRef(Py_False);
}

Py_hash_t
py_bitvector_hash(PyObject *self)
{
    PyBitVectorObject *pbv = (PyBitVectorObject *) self;
    if (pbv->hash_cache != -1) {
        return pbv->hash_cache;
    }
    BitVector *bv = pbv->bv;
    size_t n_bytes = (bv->n_bits + 7) >> 3;
    if (n_bytes == 0) {
        return 0;
    }
    PyObject *b = PyBytes_FromStringAndSize((const char *) bv->data,
                                            (Py_ssize_t) n_bytes);
    if (b == NULL) {
        return -1;
    }
    Py_hash_t hash = PyObject_Hash(b);
    Py_DECREF(b);

    if (hash == -1) {
        hash = -2;
    }
    pbv->hash_cache = hash;
    return hash;
}
