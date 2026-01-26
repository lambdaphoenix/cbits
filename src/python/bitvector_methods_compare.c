#include "bitvector_methods_compare.h"
#include "bitvector_checks.h"

PyObject *
py_bv_richcompare(PyObject *a, PyObject *b, int op)
{
    if (op != Py_EQ && op != Py_NE) {
        Py_RETURN_NOTIMPLEMENTED;
    }
    CHECK_BV_BOTH(a, b)
    BitVector *A = ((PyBitVector *) a)->bv;
    BitVector *B = ((PyBitVector *) b)->bv;

    bool eq = bv_equal(((PyBitVector *) a)->bv, ((PyBitVector *) b)->bv);
    if ((op == Py_EQ) == eq) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

Py_hash_t
py_bv_hash(PyObject *self)
{
    PyBitVector *pbv = (PyBitVector *) self;
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