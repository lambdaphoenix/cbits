/**
 * @file cbits_module.c
 * @brief Module initialization for the ``cbits._cbits`` extension.
 *
 * Implements the initialization logic for the BitVector C extension. This
 * includes creating and registering the module’s type objects, setting up the
 * per‑module state, installing metadata, and defining the garbage‑collection
 * callbacks used by CPython to manage module lifetime.
 *
 * @see cbits_module.h
 * @see cbits_state.h
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "cbits_state.h"
#include "cbits_module.h"

#include "bitvector_iter.h"

/**
 * @brief Module exec callback: create and register types and metadata.
 *
 * Executed during module initialization. Allocates the BitVector and iterator
 * types, registers them with the module, integrates BitVector with
 * ``collections.abc.Sequence``, and sets module‑level metadata such as author,
 * version, and license.
 *
 * @param module Newly created module instance.
 * @retval 0 Initialization succeeded
 * @retval -1 Initialization failed and a Python exception is set
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
    if (PyModule_AddStringConstant(module, "__version__", "0.3.0") < 0) {
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
/**
 * @brief Module‑level docstring for ``_cbits``.
 *
 * Installed as the module’s ``__doc__`` attribute.
 * @since 0.3.0
 */
PyDoc_STRVAR(module_doc, "cbits");
/**
 * @brief Method table for the module.
 *
 * The module does not expose any top-level Python functions, so the table
 * contains only the NULL terminator.
 * @since 0.3.0
 */
static PyMethodDef cbits_methods[] = {{NULL, NULL, 0, NULL}};

/**
 * @brief Initialization slot table for the module.
 *
 * Specifies callbacks executed during module creation, including the exec
 * function and optional interpreter/GIL configuration flags.
 *
 * @see PyModuleDef_Slot
 */
static PyModuleDef_Slot cbits_slots[] = {
    {Py_mod_exec, cbits_module_exec},
#if Py_VERSION_HEX >= 0x030C0000
    {Py_mod_multiple_interpreters, Py_MOD_PER_INTERPRETER_GIL_SUPPORTED},
#endif
#if Py_VERSION_HEX >= 0x030D0000
    {Py_mod_gil, Py_MOD_GIL_NOT_USED},
#endif
    {0, NULL},
};

/**
 * @brief GC traverse callback for the module.
 *
 * Reports all Python objects stored in the module state to the garbage
 * collector. This ensures proper participation in cyclic GC.
 *
 * @param module Module instance.
 * @param visit GC visitor function.
 * @param arg Extra argument passed through by the GC.
 * @return Always ``0``.
 * @since 0.3.0
 */
static int
cbits_traverse(PyObject *module, visitproc visit, void *arg)
{
    cbits_state *state = get_cbits_state(module);
    Py_VISIT(state->PyBitVectorType);
    Py_VISIT(state->PyBitVectorIterType);
    return 0;
}
/**
 * @brief GC clear callback for the module.
 *
 * Releases all Python references stored in the module state and resets them to
 * ``NULL``. Called during interpreter shutdown or module teardown.
 *
 * @param module Module instance.
 * @return Always ``0``.
 * @since 0.3.0
 */
static int
cbits_clear(PyObject *module)
{
    cbits_state *state = get_cbits_state(module);
    Py_CLEAR(state->PyBitVectorType);
    Py_CLEAR(state->PyBitVectorIterType);
    return 0;
}
/**
 * @brief Free callback for the module.
 *
 * Invoked when the module is being deallocated. Delegates to ``cbits_clear()``
 * to release all Python references held in the module state.
 *
 * @param module The module object being freed.
 * @since 0.3.0
 */
static void
cbits_free(void *module)
{
    (void) cbits_clear((PyObject *) module);
}

/**
 * @brief Definition of the ``_cbits`` extension.
 *
 * Specifies the module name, documentation string, state size, method table,
 * initialization slots, and GC callbacks.
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
 * @brief Python entry point for ``_cbits`` extension.
 *
 * Creates and returns a new module instance using the definition in
 * ::cbits_module.
 *
 * @return Newly created module object.
 */
PyMODINIT_FUNC
PyInit__cbits(void)
{
    return PyModuleDef_Init(&cbits_module);
}
