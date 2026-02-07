/**
 * @file src/python/bitvector_module.c
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
#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "bitvector_methods.h"

/** Global pointer to the PyBitVector type object. */
PyTypeObject *PyBitVectorPtr = NULL;
/** Global pointer for the iterator type object */
PyTypeObject *PyBitVectorIterType = NULL;

#if PY_VERSION_HEX >= 0x030C0000
    /**
     * @def ADD_OBJECT(module, name, object)
     * @brief Add a PyObject to a module, handling reference counts portably.
     *
     * On Python ≥ 3.12, PyModule_AddObjectRef() is available and automatically
     * steals a reference. On older versions, we fall back to
     * PyModule_AddObject() and manually increment the reference on success.
     *
     * @param module The Python module to which the object is added.
     * @param name The attribute name under which the object is registered.
     * @param object The PyObject pointer to add.
     * @return 0 on success, -1 on failure (exception set by
     * PyModule_AddObject*).
     */
    #define ADD_OBJECT(module, name, object) \
        (PyModule_AddObjectRef(module, name, object))
#else

    /**
     * @def ADD_OBJECT(module, name, object)
     * @brief Add a PyObject to a module, handling reference counts portably.
     *
     * On Python ≥ 3.12, PyModule_AddObjectRef() is available and automatically
     * steals a reference. On older versions, we fall back to
     * PyModule_AddObject() and manually increment the reference on success.
     *
     * @param module The Python module to which the object is added.
     * @param name The attribute name under which the object is registered.
     * @param object The PyObject pointer to add.
     * @return 0 on success, -1 on failure (exception set by
     * PyModule_AddObject*).
     */
    #define ADD_OBJECT(module, name, object)           \
        (PyModule_AddObject(module, name, object) == 0 \
             ? (Py_XINCREF(object), 0)                 \
             : -1)
#endif
#ifdef PYPY_VERSION
    #undef ADD_OBJECT
static inline int
cbits_add_object(PyObject *module, const char *name, PyObject *obj)
{
    int err = PyModule_AddObject(module, name, obj);
    if (err < 0) {
        return err;
    }
    Py_XINCREF(obj);
    return 0;
}

    /**
     * @def ADD_OBJECT(module, name, object)
     * @brief Add a PyObject to a module, handling reference counts portably.
     *
     * On Python ≥ 3.12, PyModule_AddObjectRef() is available and automatically
     * steals a reference. On older versions, we fall back to
     * PyModule_AddObject() and manually increment the reference on success.
     *
     * @param module The Python module to which the object is added.
     * @param name The attribute name under which the object is registered.
     * @param object The PyObject pointer to add.
     * @return 0 on success, -1 on failure (exception set by
     * PyModule_AddObject*).
     */
    #define ADD_OBJECT(module, name, object) \
        cbits_add_object(module, name, object)
#endif

/**
 * @brief Module exec callback: register BitVector type and metadata.
 * @param module New module instance.
 * @return 0 on success; -1 on failure (exception set).
 */
static int
cbits_module_exec(PyObject *module)
{
/* Register BitVector */
#if defined(_MSC_VER)
    init_cpu_dispatch();
#endif
#if PY_VERSION_HEX >= 0x030B0000
    PyBitVectorPtr = (PyTypeObject *) PyType_FromModuleAndSpec(
        module, &PyBitVector_spec, NULL);
    if (!PyBitVectorPtr) {
        PyErr_SetString(PyExc_RuntimeError,
                        "Failed to initialize BitVector type");
        return -1;
    }
    PyBitVectorIterType = (PyTypeObject *) PyType_FromModuleAndSpec(
        module, &PyBitVectorIter_spec, NULL);
    if (!PyBitVectorIterType) {
        PyErr_SetString(PyExc_RuntimeError,
                        "Failed to initialize _BitVectorIter type");
        return -1;
    }
#else
    PyBitVectorPtr = (PyTypeObject *) PyType_FromSpec(&PyBitVector_spec);
    if (!PyBitVectorPtr || PyType_Ready(PyBitVectorPtr) < 0) {
        PyErr_SetString(PyExc_RuntimeError,
                        "Failed to initialize BitVector type");
        return -1;
    }
    PyBitVectorIterType =
        (PyTypeObject *) PyType_FromSpec(&PyBitVectorIter_spec);
    if (!PyBitVectorIterType || PyType_Ready(PyBitVectorIterType) < 0) {
        PyErr_SetString(PyExc_RuntimeError,
                        "Failed to initialize _BitVectorIter type");
        return -1;
    }
#endif
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

/**
 * @brief Module initialization slots.
 *
 * Lists callbacks invoked when the module is loaded; here,
 * we use Py_mod_exec to register types and module constants.
 *
 * @see PyModuleDef_Slot
 */
static PyModuleDef_Slot cbits_module_slots[] = {
    {Py_mod_exec, cbits_module_exec},
    {0, NULL},
};

/**
 * @brief Definition of the _cbits extension module.
 *
 * Describes the module’s name, docstring, memory footprint,
 * and its initialization slot table.
 *
 * @see PyModuleDef
 */
static PyModuleDef cbits_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "_cbits",
    .m_doc = PyDoc_STR("cbits"),
    .m_size = 0,
    .m_slots = cbits_module_slots,
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
