/**
 * @file bitvector_methods.h
 * @brief Aggregated method table for the ``BitVector`` Python type.
 *
 * Declares the unified ``PyMethodDef`` table used by the BitVector type. All
 * method groups (core operations, slicing, comparison, misc utilities)
 * contribute entries to this table, which is then referenced by the
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#ifndef CBITS_PY_BITVECTOR_METHODS_H
#define CBITS_PY_BITVECTOR_METHODS_H

#include <Python.h>
#include "bitvector_object.h"

extern PyMethodDef BitVector_methods[];

#endif /* CBITS_PY_BITVECTOR_METHODS_H */
