/**
 * @file src/python/bitvector_methods.h
 * @brief Aggregated method table for the BitVector Python type.
 *
 * Declares:
 * - \ref BitVector_methods[]: the unified PyMethodDef table
 *
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_H
#define CBITS_PY_BITVECTOR_METHODS_H

#include <Python.h>
#include "bitvector_object.h"

extern PyMethodDef BitVector_methods[];

#endif /* CBITS_PY_BITVECTOR_METHODS_H */
