/**
 * @file src/python/bitvector_check.h  //TODO rename to _check
 * @brief Validation helpers for Python BitVector operations.
 *
 * Contain macros for:
 * - type checking
 *
 * Used across the method modules to ensure consistent error behavior.
 *
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_CHECK_H
#define CBITS_PY_BITVECTOR_CHECK_H

#include <Python.h>

/**
 * @def CHECK_BV_OBJ(o)
 * @brief Verify that @a o is a PyBitVector instance or raise @c TypeError.
 */
#define CHECK_BV_OBJ(o)                                         \
    if (!PyObject_TypeCheck(o, PyBitVectorPtr)) {               \
        PyErr_SetString(PyExc_TypeError, "Expected BitVector"); \
        return NULL;                                            \
    }

/**
 * @def CHECK_BV_BOTH(a, b)
 * @brief Verify both @a a and @a b are PyBitVector, else return @c
 * NotImplemented.
 */
#define CHECK_BV_BOTH(a, b)                       \
    if (!PyObject_TypeCheck(a, PyBitVectorPtr) || \
        !PyObject_TypeCheck(b, PyBitVectorPtr)) { \
        Py_RETURN_NOTIMPLEMENTED;                 \
    }

#endif /* CBITS_PY_BITVECTOR_CHECK_H */
