/// @file python/binding.c
/// @brief Python C-API bindings for cbits.BitVector.
///
/// Defines:
///
/// - PyBitVector type
///
/// - tp_new, tp_init, tp_dealloc, tp_methods
///
/// - sequence, number and richcompare protocols
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "bitvector.h"

#define CHECK_BV_OBJ(o)                                         \
    if (!PyObject_TypeCheck(o, PyBitVectorPtr)) {               \
        PyErr_SetString(PyExc_TypeError, "Expected BitVector"); \
        return NULL;                                            \
    }
#define CHECK_BV_BOTH(a, b)                       \
    if (!PyObject_TypeCheck(a, PyBitVectorPtr) || \
        !PyObject_TypeCheck(b, PyBitVectorPtr)) { \
        Py_RETURN_NOTIMPLEMENTED;                 \
    }

typedef struct {
    PyObject_HEAD BitVector *bv;
} PyBitVector;

PyTypeObject *PyBitVectorPtr = NULL;

static PyObject *
bv_wrap_new(BitVector *bv_data)
{
    PyBitVector *obj =
        (PyBitVector *) PyBitVectorPtr->tp_alloc(PyBitVectorPtr, 0);
    if (!obj) {
        bv_free(bv_data);
        return NULL;
    }
    obj->bv = bv_data;
    return (PyObject *) obj;
}
/* -------------------------------------------------------------------------
 * Deallocation and object lifecycle
 * ------------------------------------------------------------------------- */

static void
py_bv_free(PyObject *self)
{
    PyBitVector *bvself = (PyBitVector *) self;
    if (bvself->bv) {
        bv_free(bvself->bv);
        bvself->bv = NULL;
    }
    Py_TYPE(self)->tp_free(self);
}

static PyObject *
py_bv_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyBitVector *bvself = (PyBitVector *) type->tp_alloc(type, 0);
    if (!bvself) {
        return NULL;
    }
    bvself->bv = NULL;
    return (PyObject *) bvself;
}

static PyObject *
py_bv_copy(PyObject *self, PyObject *UNUSED(ignored))
{
    BitVector *copy = bv_copy(((PyBitVector *) self)->bv);
    if (!copy) {
        PyErr_SetString(PyExc_MemoryError,
                        "Failed to allocate BitVector in copy()");
        return NULL;
    }
    return bv_wrap_new(copy);
}

static PyObject *
py_bv_deepcopy(PyObject *self, PyObject *memo)
{
    PyObject *copy = py_bv_copy(self, NULL);
    if (!copy) {
        return NULL;
    }
    if (memo && PyDict_Check(memo)) {
        if (PyDict_SetItem(memo, self, copy) < 0) {
            Py_DECREF(copy);
            return NULL;
        }
    }
    return copy;
}

static int
py_bv_init(PyObject *self, PyObject *args, PyObject *kwds)
{
    Py_ssize_t n_bits;
    static char *kwlist[] = {"size", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "n", kwlist, &n_bits)) {
        return -1;
    }
    PyBitVector *bvself = (PyBitVector *) self;
    bvself->bv = bv_new((size_t) n_bits);
    if (!bvself->bv) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate BitVector");
        return -1;
    }
    return 0;
}

/* -------------------------------------------------------------------------
 * Core BitVector Methods
 * ------------------------------------------------------------------------- */
static inline int
bv_parse_index(PyObject *self, PyObject *const *args, Py_ssize_t n_args,
               size_t *p_index)
{
    if (n_args != 1) {
        PyErr_SetString(PyExc_TypeError, "Method takes exactly one argument");
        return -1;
    }
    Py_ssize_t index = PyLong_AsSsize_t(args[0]);
    if (index == -1 && PyErr_Occurred()) {
        return -1;
    }
    PyBitVector *bvself = (PyBitVector *) self;
    size_t n_bits = bvself->bv->n_bits;
    if (index < 0) {
        index += (Py_ssize_t) n_bits;
    }
    if (index < 0 || index >= n_bits) {
        PyErr_SetString(PyExc_IndexError, "BitVector index out of range");
        return -1;
    }
    *p_index = (size_t) index;
    return 0;
}

static PyObject *
py_bv_get(PyObject *self, PyObject *const *args, Py_ssize_t nargs)
{
    size_t index;
    if (bv_parse_index(self, args, nargs, &index) < 0) {
        return NULL;
    }

    int bit = bv_get(((PyBitVector *) self)->bv, index);
    return PyBool_FromLong(bit);
}

static PyObject *
py_bv_set(PyObject *self, PyObject *const *args, Py_ssize_t nargs)
{
    size_t index;
    if (bv_parse_index(self, args, nargs, &index) < 0) {
        return NULL;
    }

    bv_set(((PyBitVector *) self)->bv, index);
    Py_RETURN_NONE;
}

static PyObject *
py_bv_clear(PyObject *self, PyObject *const *args, Py_ssize_t nargs)
{
    size_t index;
    if (bv_parse_index(self, args, nargs, &index) < 0) {
        return NULL;
    }

    bv_clear(((PyBitVector *) self)->bv, index);
    Py_RETURN_NONE;
}

static PyObject *
py_bv_flip(PyObject *self, PyObject *const *args, Py_ssize_t nargs)
{
    size_t index;
    if (bv_parse_index(self, args, nargs, &index) < 0) {
        return NULL;
    }

    bv_flip(((PyBitVector *) self)->bv, index);
    Py_RETURN_NONE;
}

static PyObject *
py_bv_rank(PyObject *self, PyObject *const *args, Py_ssize_t nargs)
{
    size_t index;
    if (bv_parse_index(self, args, nargs, &index) < 0) {
        return NULL;
    }

    size_t rank = bv_rank(((PyBitVector *) self)->bv, index);
    return PyLong_FromSize_t(rank);
}

static PyMethodDef BitVector_methods[] = {
    {"get", (PyCFunction) py_bv_get, METH_FASTCALL, PyDoc_STR("Get bit")},
    {"set", (PyCFunction) py_bv_set, METH_FASTCALL, PyDoc_STR("Set bit")},
    {"clear", (PyCFunction) py_bv_clear, METH_FASTCALL,
     PyDoc_STR("Clear bit")},
    {"flip", (PyCFunction) py_bv_flip, METH_FASTCALL, PyDoc_STR("Flip bit")},
    {"rank", (PyCFunction) py_bv_rank, METH_FASTCALL, PyDoc_STR("Rank query")},
    {"copy", (PyCFunction) py_bv_copy, METH_NOARGS,
     PyDoc_STR("Return a copy of that BitVector")},
    {"__copy__", (PyCFunction) py_bv_copy, METH_NOARGS,
     PyDoc_STR("Return a copy of that BitVector")},
    {"__deepcopy__", (PyCFunction) py_bv_deepcopy, METH_O,
     PyDoc_STR("Return a copy of that BitVector")},
    {NULL, NULL, 0, NULL},
};

/* -------------------------------------------------------------------------
 * Magic Methods
 * ------------------------------------------------------------------------- */

static PyObject *
py_bv_repr(PyObject *self)
{
    PyBitVector *bvself = (PyBitVector *) self;
    return PyUnicode_FromFormat("<cbits.BitVector object at %p bits=%zu>",
                                self, bvself->bv->n_bits);
}

static PyObject *
py_bv_str(PyObject *self)
{
    PyBitVector *bvself = (PyBitVector *) self;
    return PyUnicode_FromFormat("BitVector with %zu bits", bvself->bv->n_bits);
}

static PyObject *
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

/* -------------------------------------------------------------------------
 * Sequence Protocol
 * ------------------------------------------------------------------------- */

static Py_ssize_t
py_bv_len(PyObject *self)
{
    BitVector *bv = ((PyBitVector *) self)->bv;
    return (Py_ssize_t) (bv ? bv->n_bits : 0);
}

static PyObject *
py_bv_item(PyObject *self, Py_ssize_t i)
{
    BitVector *bv = ((PyBitVector *) self)->bv;
    if (!bv || bv->n_bits <= (size_t) i) {
        PyErr_SetString(PyExc_IndexError, "BitVector index out of range");
        return NULL;
    }
    return PyBool_FromLong(bv_get(bv, (size_t) i));
}

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
        bv_set(bv, (size_t) i);
    }
    else {
        bv_clear(bv, (size_t) i);
    }
    return 0;
}

static int
py_bv_contains(PyObject *self, PyObject *value)
{
    if (!PyObject_TypeCheck((PyObject *) value, PyBitVectorPtr)) {
        return false;
    }

    PyBitVector *A = (PyBitVector *) self;
    PyBitVector *B = (PyBitVector *) value;
    return bv_contains_subvector(A->bv, B->bv);
}

/* -------------------------------------------------------------------------
 * Number Protocol
 * ------------------------------------------------------------------------- */

static PyObject *
py_bv_and(PyObject *a, PyObject *b)
{
    CHECK_BV_BOTH(a, b)

    PyBitVector *A = (PyBitVector *) a;
    PyBitVector *B = (PyBitVector *) b;

    size_t size = A->bv->n_bits;
    if (size != B->bv->n_bits) {
        PyErr_SetString(PyExc_ValueError, "length mismatch");
        return NULL;
    }
    BitVector *C = bv_new(size);
    if (!C) {
        PyErr_SetString(PyExc_MemoryError,
                        "BitVector allocation failed in __and__");
        return NULL;
    }

    for (size_t i = 0; i < A->bv->n_words; ++i) {
        C->data[i] = A->bv->data[i] & B->bv->data[i];
    }
    return bv_wrap_new(C);
}

static PyObject *
py_bv_iand(PyObject *self, PyObject *arg)
{
    CHECK_BV_OBJ(arg)

    PyBitVector *A = (PyBitVector *) self;
    PyBitVector *B = (PyBitVector *) arg;

    size_t size = A->bv->n_bits;
    if (size != B->bv->n_bits) {
        PyErr_SetString(PyExc_ValueError, "length mismatch");
        return NULL;
    }

    for (size_t i = 0; i < A->bv->n_words; ++i) {
        cbits_atomic_fetch_and(&A->bv->data[i], B->bv->data[i]);
    }
    A->bv->rank_dirty = true;
    Py_INCREF(self);
    return self;
}

static PyObject *
py_bv_or(PyObject *a, PyObject *b)
{
    CHECK_BV_BOTH(a, b)

    PyBitVector *A = (PyBitVector *) a;
    PyBitVector *B = (PyBitVector *) b;

    size_t size = A->bv->n_bits;
    if (size != B->bv->n_bits) {
        PyErr_SetString(PyExc_ValueError, "length mismatch");
        return NULL;
    }
    BitVector *C = bv_new(size);
    if (!C) {
        PyErr_SetString(PyExc_MemoryError,
                        "BitVector allocation failed in __or__");
        return NULL;
    }

    for (size_t i = 0; i < A->bv->n_words; ++i) {
        C->data[i] = A->bv->data[i] | B->bv->data[i];
    }
    return bv_wrap_new(C);
}

static PyObject *
py_bv_ior(PyObject *self, PyObject *arg)
{
    CHECK_BV_OBJ(arg)

    PyBitVector *A = (PyBitVector *) self;
    PyBitVector *B = (PyBitVector *) arg;

    size_t size = A->bv->n_bits;
    if (size != B->bv->n_bits) {
        PyErr_SetString(PyExc_ValueError, "length mismatch");
        return NULL;
    }

    for (size_t i = 0; i < A->bv->n_words; ++i) {
        cbits_atomic_fetch_or(&A->bv->data[i], B->bv->data[i]);
    }
    A->bv->rank_dirty = true;
    Py_INCREF(self);
    return self;
}

static PyObject *
py_bv_xor(PyObject *a, PyObject *b)
{
    CHECK_BV_BOTH(a, b)

    PyBitVector *A = (PyBitVector *) a;
    PyBitVector *B = (PyBitVector *) b;

    size_t size = A->bv->n_bits;
    if (size != B->bv->n_bits) {
        PyErr_SetString(PyExc_ValueError, "length mismatch");
        return NULL;
    }
    BitVector *C = bv_new(size);
    if (!C) {
        PyErr_SetString(PyExc_MemoryError,
                        "BitVector allocation failed in __xor__");
        return NULL;
    }

    for (size_t i = 0; i < A->bv->n_words; ++i) {
        C->data[i] = A->bv->data[i] ^ B->bv->data[i];
    }
    return bv_wrap_new(C);
}

static PyObject *
py_bv_ixor(PyObject *self, PyObject *arg)
{
    CHECK_BV_OBJ(arg)

    PyBitVector *A = (PyBitVector *) self;
    PyBitVector *B = (PyBitVector *) arg;

    size_t size = A->bv->n_bits;
    if (size != B->bv->n_bits) {
        PyErr_SetString(PyExc_ValueError, "length mismatch");
        return NULL;
    }

    for (size_t i = 0; i < A->bv->n_words; ++i) {
        cbits_atomic_fetch_xor(&A->bv->data[i], B->bv->data[i]);
    }
    A->bv->rank_dirty = true;
    Py_INCREF(self);
    return self;
}

static PyObject *
py_bv_invert(PyObject *self)
{
    PyBitVector *A = (PyBitVector *) self;
    BitVector *C = bv_new(A->bv->n_bits);
    if (!C) {
        PyErr_SetString(PyExc_MemoryError,
                        "BitVector allocation failed in __not__");
        return NULL;
    }
    for (size_t i = 0; i < A->bv->n_words; ++i) {
        C->data[i] = ~A->bv->data[i];
    }
    return bv_wrap_new(C);
}

static int
py_bv_bool(PyObject *self)
{
    PyBitVector *bvself = (PyBitVector *) self;
    return bv_rank(bvself->bv, bvself->bv->n_bits - 1) > 0;
}

/* -------------------------------------------------------------------------
 * Properties
 * ------------------------------------------------------------------------- */

static PyObject *
py_bv_get_size(PyObject *self, void *closure)
{
    PyBitVector *bvself = (PyBitVector *) self;
    return PyLong_FromSize_t(bvself->bv->n_bits);
}

static int
py_bv_set_size(PyObject *self, void *closure)
{
    PyErr_SetString(PyExc_AttributeError, "size is read-only");
    return -1;
}

static PyGetSetDef PyBitVector_getset[] = {
    {"bits", (getter) py_bv_get_size, (setter) py_bv_set_size,
     PyDoc_STR("The number of bits"), NULL},
    {NULL},
};

/* -------------------------------------------------------------------------
 * Type Object Definition
 * ------------------------------------------------------------------------- */

static PyType_Slot PyBitVector_slots[] = {
    {Py_tp_new, py_bv_new},
    {Py_tp_init, py_bv_init},
    {Py_tp_dealloc, py_bv_free},
    {Py_tp_methods, BitVector_methods},
    {Py_tp_repr, py_bv_repr},
    {Py_tp_str, py_bv_str},
    {Py_tp_doc, PyDoc_STR("BitVector")},
    {Py_tp_getset, PyBitVector_getset},
    {Py_tp_richcompare, py_bv_richcompare},

    {Py_sq_length, py_bv_len},
    {Py_sq_item, py_bv_item},
    {Py_sq_ass_item, py_bv_ass_item},
    {Py_sq_contains, py_bv_contains},

    {Py_nb_and, py_bv_and},
    {Py_nb_inplace_and, py_bv_iand},
    {Py_nb_or, py_bv_or},
    {Py_nb_inplace_or, py_bv_ior},
    {Py_nb_xor, py_bv_xor},
    {Py_nb_inplace_xor, py_bv_ixor},
    {Py_nb_invert, py_bv_invert},
    {Py_nb_bool, py_bv_bool},

    {0, 0},
};

PyType_Spec PyBitVector_spec = {
    .name = "cbits.BitVector",
    .basicsize = sizeof(PyBitVector),
    .itemsize = 0,
    .flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .slots = PyBitVector_slots,
};

/* -------------------------------------------------------------------------
 * Module Init
 * ------------------------------------------------------------------------- */

#if PY_VERSION_HEX >= 0x030C0000
    #define ADD_OBJECT(module, name, object) \
        (PyModule_AddObjectRef(module, name, object))
#else
    #define ADD_OBJECT(module, name, object)           \
        (PyModule_AddObject(module, name, object) == 0 \
             ? (Py_XINCREF(object), 0)                 \
             : -1)
#endif

static int
cbits_module_exec(PyObject *module)
{
    /* Register BitVector */
    PyBitVectorPtr = (PyTypeObject *) PyType_FromModuleAndSpec(
        module, &PyBitVector_spec, NULL);
    if (!PyBitVectorPtr) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to create BitVector type");
        return -1;
    }

    if (ADD_OBJECT(module, "BitVector", (PyObject *) PyBitVectorPtr) < 0) {
        return -1;
    }

    /* Metadata */
    if (PyModule_AddStringConstant(module, "__author__", "lambdaphoenix") <
        0) {
        return -1;
    }
    if (PyModule_AddStringConstant(module, "__version__", "0.1.0") < 0) {
        return -1;
    }
    return 0;
}

static PyModuleDef_Slot cbits_module_slots[] = {
    {Py_mod_exec, cbits_module_exec},
    {0, NULL},
};

static PyModuleDef cbits_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "_cbits",
    .m_doc = PyDoc_STR("cbits"),
    .m_size = 0,
    .m_slots = cbits_module_slots,
};

PyMODINIT_FUNC
PyInit__cbits(void)
{
    return PyModuleDef_Init(&cbits_module);
}
