/**
 * @file src/python/cbits_module.h
 * @brief Forward declaration of the cbits module definition.
 *
 * Exposes the module's PyModuleDef so that other compilation units (such as
 * state helpers or type initializers) can reference the module definition
 * without creating circular dependencies.
 *
 * @see cbits_module.c
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_MODULE_H
#define CBITS_MODULE_H

#define PY_SSIZE_T_CLEAN
#include "Python.h"

extern struct PyModuleDef cbits_module;

#endif /* CBITS_MODULE_H */
