/**
 * @file cbits_module.h
 * @brief Forward declaration of the cbits module definition.
 *
 * Provides external visibility of the module's ``PyModuleDef`` so that other
 * compilation units-such as state helpers, type initializers, or method
 * implementations-can reference the module definition without introducing
 * circular dependencies.
 *
 * This header does not define initialization logic; it only exposes the
 * declaration of the module object implemented in ``cbits_module.c``.
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
