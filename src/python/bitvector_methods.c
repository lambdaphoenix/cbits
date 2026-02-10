/**
 * @file bitvector_methods.c
 * @brief Aggregates all Python-visible ``BitVector`` methods into one table.
 *
 * Collects all method groups (basic operations, copying, rank queries, etc.)
 * and exposes them as a unified ``PyMethodDef`` array used by the BitVector
 * type specification.
 *
 * This file defines:
 * - the full method table \ref BitVector_methods
 * - all associated docstrings for Python‑level methods
 *
 * @author lambdaphoenix
 * @version 0.3.0
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_methods.h"
#include "bitvector_methods_basic.h"
#include "bitvector_methods_copy.h"
#include "bitvector_methods_rank.h"

/* Docstrings */

/** @brief Docstring for ``BitVector.get``. */
PyDoc_STRVAR(
    py_bv_get__doc__,
    "get(index: int) -> bool\n"
    "\n"
    "Return the boolean value of the bit at position *index*.\n"
    "Negative indices are supported. Raises IndexError if out of range.");
/** @brief Docstring for ``BitVector.set``. */
PyDoc_STRVAR(
    py_bv_set__doc__,
    "set(index: int) -> None\n"
    "\n"
    "Set the bit at position *index* to True. Supports negative indexing.\n"
    "Raises IndexError if out of range.");
/** @brief Docstring for ``BitVector.clear``. */
PyDoc_STRVAR(py_bv_clear__doc__,
             "clear(index: int) -> None\n"
             "\n"
             "Clear the bit (set to False) at position *index*. Supports "
             "negative indexing.\n"
             "Raises IndexError if out of range.");
/** @brief Docstring for ``BitVector.flip``. */
PyDoc_STRVAR(
    py_bv_flip__doc__,
    "flip(index: int) -> None\n"
    "\n"
    "Toggle the bit at position *index*. Supports negative indexing.\n"
    "Raises IndexError if out of range.");
/** @brief Docstring for ``BitVector.set_range``. */
PyDoc_STRVAR(py_bv_set_range__doc__,
             "set_range(start: int, length: int) -> None\n"
             "\n"
             "Set all bits in the half-open range [start, start+length).\n"
             "Raises IndexError if the range is out of bounds.");
/** @brief Docstring for ``BitVector.clear_range``. */
PyDoc_STRVAR(py_bv_clear_range__doc__,
             "clear_range(start: int, length: int) -> None\n"
             "\n"
             "Clear all bits in the half-open range [start, start+length).\n"
             "Raises IndexError if the range is out of bounds.");
/** @brief Docstring for ``BitVector.flip_range``. */
PyDoc_STRVAR(py_bv_flip_range__doc__,
             "flip_range(start: int, length: int) -> None\n"
             "\n"
             "Toggle all bits in the half-open range [start, start+length).\n"
             "Raises IndexError if the range is out of bounds.");
/** @brief Docstring for ``BitVector.copy``. */
PyDoc_STRVAR(py_bv_copy__doc__,
             "copy() -> BitVector\n"
             "\n"
             "Return a copy of this BitVector.");
/** @brief Docstring for ``BitVector.__copy__``. */
PyDoc_STRVAR(py_bv_copy_inline__doc__,
             "__copy__() -> BitVector\n"
             "\n"
             "Return a copy of this BitVector.");
/** @brief Docstring for ``BitVector.__deepcopy__``.*/
PyDoc_STRVAR(py_bv_deepcopy__doc__,
             "__deepcopy__(memo: dict) -> BitVector\n"
             "\n"
             "Return a copy of this BitVector, registering it in *memo*.");
/** @brief Docstring for ``BitVector.rank``. */
PyDoc_STRVAR(
    py_bv_rank__doc__,
    "rank(index: int) -> int\n"
    "\n"
    "Count the number of bits set to True in the half-open range [0..index].\n"
    "Supports negative indexing. Raises IndexError if out of range.");
/**
 * @brief Unified method table for the BitVector type.
 *
 * Contains all Python‑callable methods exposed by ``BitVector``. This table is
 * referenced by the type specification in ``bitvector_object.c``.
 */
PyMethodDef BitVector_methods[] = {
    {"get", (PyCFunction) py_bitvector_get, METH_O, py_bv_get__doc__},
    {"set", (PyCFunction) py_bitvector_set, METH_O, py_bv_set__doc__},
    {"clear", (PyCFunction) py_bitvector_clear, METH_O, py_bv_clear__doc__},
    {"flip", (PyCFunction) py_bitvector_flip, METH_O, py_bv_flip__doc__},

    {"set_range", (PyCFunction) py_bitvector_set_range, METH_VARARGS,
     py_bv_set_range__doc__},
    {"clear_range", (PyCFunction) py_bitvector_clear_range, METH_VARARGS,
     py_bv_clear_range__doc__},
    {"flip_range", (PyCFunction) py_bitvector_flip_range, METH_VARARGS,
     py_bv_flip_range__doc__},

    {"rank", (PyCFunction) py_bitvector_rank, METH_O, py_bv_rank__doc__},

    {"copy", (PyCFunction) py_bitvector_copy, METH_NOARGS, py_bv_copy__doc__},
    {"__copy__", (PyCFunction) py_bitvector_copy, METH_NOARGS,
     py_bv_copy_inline__doc__},
    {"__deepcopy__", (PyCFunction) py_bitvector_deepcopy, METH_O,
     py_bv_deepcopy__doc__},
    {NULL, NULL, 0, NULL},
};
