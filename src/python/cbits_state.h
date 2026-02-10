/**
 * @file cbits_state.h
 * @brief Module-local state definition for the cbits extension.
 *
 * This header defines the per‑module state structure used by the BitVector
 * CPython extension. The state stores references to the Python types exposed
 * by the module and provides helpers for retrieving the state and validating
 * objects at runtime.
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_STATE_H
#define CBITS_STATE_H

#include "cbits_module.h"

/**
 * @defgroup cbits_state_module Module State
 * @brief Internal state and helpers for the cbits extension.
 * @{
 */

/**
 * @brief Per-module state for the cbits extension.
 *
 * Each module instance maintains its own state, allocated through CPython’s
 * module state mechanism. The structure stores the Python type objects
 * required by the BitVector implementation.
 * @since 0.3.0
 */
typedef struct {
    PyTypeObject *PyBitVectorType;     /**< BitVector type object */
    PyTypeObject *PyBitVectorIterType; /**< BitVector iterator type object */
} cbits_state;

/**
 * @brief Retrieve the cbits module state from a module object.
 *
 * This function returns the state associated with a module instance created
 * from ::cbits_module.
 *
 * @param module Python module object.
 * @return Pointer to the module's ::cbits_state.
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
     * Convenience macro for accessing module state from type methods or slots.
     *
     * @param type Type object whose defining module should be queried.
     * @return Pointer to the module's ::cbits_state.
     *
     * @since 0.3.0
     */
    #define find_cbits_state_by_type(type) \
        (get_cbits_state(PyType_GetModuleByDef((type), &cbits_module)))
#else
    /**
     * @brief Retrieve the cbits module state using a type object.
     *
     * Compatibility macro for Python versions prior to 3.11.
     *
     * @param type Type object whose defining module should be queried.
     * @return Pointer to the module's ::cbits_state.
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
 * @param state Module state containing the BitVector type reference.
 * @return Non-zero if @p object is a BitVector instance, zero otherwise.
 * @since 0.3.0
 */
#define py_bitvector_check(object, state) \
    PyObject_TypeCheck(object, state->PyBitVectorType)

/** @} */ /* end of cbits_state_module */

#endif /* CBITS_STATE_H */
