/**
 * @file src/python/cbits_state.h
 * @brief Module-local state for the cbits extension.
 *
 * Defines the per-module state structure used by the BitVector C extension.
 * The state stores references to the Python types exposed by the module and
 * provides helpers to retrieve the state and validate objects.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_STATE_H
#define CBITS_STATE_H

#include "cbits_module.h"

/**
 * @brief Per-module state for the cbits extension.
 *
 * Holds the Python type objects needed by the module. Each module instance
 * maintains its own state, allocated by CPython's module state mechanism.
 *
 * @since 0.3.0
 */
typedef struct {
    PyTypeObject *PyBitVectorType;     /**< BitVector type object */
    PyTypeObject *PyBitVectorIterType; /**< BitVector iterator type object */
} cbits_state;

/**
 * @brief Retrieve the cbits module state from a module object.
 *
 * @param module Python module object created from cbits_module.
 * @return Pointer to the module's cbits_state.
 * @since 0.3.0
 */
static inline cbits_state *
get_cbits_state(PyObject *module)
{
    return (cbits_state *) PyModule_GetState(module);
}

#if PY_VERSION_HEX >= 0x030B0000
    /**
     * @brief Retrieve the cbits module state using a type object.
     *
     * Convenience wrapper for modules that need to access their state from a
     * type method or slot.
     *
     * @param type Type object to retrieve module from
     * @return Pointer to the module's cbits_state.
     *
     * @since 0.3.0
     */
    #define find_cbits_state_by_type(type) \
        (get_cbits_state(PyType_GetModuleByDef((type), &cbits_module)))
#else
    /**
     * @brief Retrieve the cbits module state using a type object.
     *
     * Convenience wrapper for modules that need to access their state from a
     * type method or slot.
     *
     * @param type Type object to retrieve module from
     * @return Pointer to the module's cbits_state.
     *
     * @since 0.3.0
     */
    #define find_cbits_state_by_type(type) \
        (get_cbits_state(PyType_GetModule((type))))
#endif

/**
 * @brief Check whether an object is an instance of the BitVector type.
 *
 * @param object Python object to test.
 * @param state Module state containing the BitVector type.
 * @return Non-zero if the object is a BitVector instance, zero otherwise.
 *
 * @since 0.3.0
 */
#define py_bitvector_check(object, state) \
    PyObject_TypeCheck(object, state->PyBitVectorType)

#endif /* CBITS_STATE_H */
