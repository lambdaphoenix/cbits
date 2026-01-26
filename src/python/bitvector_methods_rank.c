#include "bitvector_methods_rank.h"
#include "bitvector_parse.h"

PyObject *
py_bv_rank(PyObject *self, PyObject *arg)
{
    size_t index;
    if (bv_parse_index(self, arg, &index) < 0) {
        return NULL;
    }

    size_t rank = bv_rank(((PyBitVector *) self)->bv, index);
    return PyLong_FromSize_t(rank);
}