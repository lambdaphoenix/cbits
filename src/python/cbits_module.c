/**
 * @file src/python/cbits_module.c
 * @brief Module initialization for the cbits._cbits extension.
 *
 * Registers:
 * - BitVector type
 * - BitVector iterator type
 * - module-level constants
 *
 * This is the entry point for the Python extension module.
 *
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "cbits_state.h"
#include "cbits_module.h"

#include "bitvector_iter.h"

/**
 * @brief Module exec callback: register BitVector type and metadata.
 * @param module New module instance.
 * @return 0 on success; -1 on failure (exception set).
 */
static int
cbits_module_exec(PyObject *module)
{
    cbits_state *state = get_cbits_state(module);

    state->PyBitVectorType = (PyTypeObject *) PyType_FromModuleAndSpec(
        module, &PyBitVector_spec, NULL);
    if (state->PyBitVectorType == NULL) {
        return -1;
    }
    state->PyBitVectorIterType = (PyTypeObject *) PyType_FromModuleAndSpec(
        module, &PyBitVectorIter_spec, NULL);
    if (state->PyBitVectorIterType == NULL) {
        return -1;
    }
    Py_SET_TYPE(state->PyBitVectorIterType, &PyType_Type);

    if (PyModule_AddObjectRef(module, "BitVector",
                              (PyObject *) state->PyBitVectorType) < 0) {
        return -1;
    }
    PyObject *mod = PyImport_ImportModule("collections.abc");
    if (mod == NULL) {
        Py_DECREF((PyObject *) state->PyBitVectorType);
        return -1;
    }
    PyObject *sequence = PyObject_GetAttrString(mod, "Sequence");
    Py_DECREF(mod);
    if (sequence == NULL) {
        Py_DECREF((PyObject *) state->PyBitVectorType);
        return -1;
    }
    PyObject *res = PyObject_CallMethod(sequence, "register", "O",
                                        (PyObject *) state->PyBitVectorType);
    Py_DECREF(sequence);
    if (!res) {
        Py_DECREF((PyObject *) state->PyBitVectorType);
        return -1;
    }
    Py_DECREF(res);

    if (PyModule_AddType(module, state->PyBitVectorType) < 0) {
        return -1;
    }

    /* Metadata */
    if (PyModule_AddStringConstant(module, "__author__", "lambdaphoenix") <
        0) {
        return -1;
    }
    if (PyModule_AddStringConstant(module, "__version__", "0.2.1") < 0) {
        return -1;
    }
    if (PyModule_AddStringConstant(module, "__license__", "Apache-2.0") < 0) {
        return -1;
    }
    if (PyModule_AddStringConstant(
            module, "__license_url__",
            "https://github.com/lambdaphoenix/cbits/blob/main/LICENSE") < 0) {
        return -1;
    }
    return 0;
}

PyDoc_STRVAR(module_doc, "cbits");

static PyMethodDef cbits_methods[] = {{NULL, NULL, 0, NULL}};

/**
 * @brief Module initialization slots.
 *
 * Lists callbacks invoked when the module is loaded; here,
 * we use Py_mod_exec to register types and module constants.
 *
 * @see PyModuleDef_Slot
 */
static PyModuleDef_Slot cbits_slots[] = {
    {Py_mod_exec, cbits_module_exec},
    {Py_mod_multiple_interpreters, Py_MOD_PER_INTERPRETER_GIL_SUPPORTED},
    {Py_mod_gil, Py_MOD_GIL_NOT_USED},
    {0, NULL},
};

static int
cbits_traverse(PyObject *module, visitproc visit, void *arg)
{
    cbits_state *state = get_cbits_state(module);
    Py_VISIT(state->PyBitVectorType);
    Py_VISIT(state->PyBitVectorIterType);
    return 0;
}

static int
cbits_clear(PyObject *module)
{
    cbits_state *state = get_cbits_state(module);
    Py_CLEAR(state->PyBitVectorType);
    Py_CLEAR(state->PyBitVectorIterType);
    return 0;
}

static void
cbits_free(void *module)
{
    (void) cbits_clear((PyObject *) module);
}

/**
 * @brief Definition of the _cbits extension module.
 *
 * Describes the module’s name, docstring, memory footprint,
 * and its initialization slot table.
 *
 * @see PyModuleDef
 */
struct PyModuleDef cbits_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "_cbits",
    .m_size = sizeof(cbits_state),
    .m_doc = module_doc,
    .m_methods = cbits_methods,
    .m_slots = cbits_slots,
    .m_traverse = cbits_traverse,
    .m_clear = cbits_clear,
    .m_free = cbits_free,
};

/**
 * @brief Python entrypoint for _cbits extension module.
 * @return New module object (borrowed reference).
 */
PyMODINIT_FUNC
PyInit__cbits(void)
{
    return PyModuleDef_Init(&cbits_module);
}
