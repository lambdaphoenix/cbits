/**
 * @file src/python/bitvector_methods.c
 * @brief Aggregates all BitVector Python methods into a single table.
 *
 * Includes all method group headers and constructs:
 * - \ref BitVector_methods[]
 *
 * This file centralizes the Python-visible API of the BitVector type.
 *
 * @author lambdaphoenix
 * @version 0.2.1
 * @copyright Copyright (c) 2026 lambdaphoenix
 */
#include "bitvector_methods.h"
#include "bitvector_methods_basic.h"
#include "bitvector_methods_copy.h"
#include "bitvector_methods_rank.h"

/* Docstrings */
PyDoc_STRVAR(
    py_bv_get__doc__,
    "get(index: int) -> bool\n"
    "\n"
    "Return the boolean value of the bit at position *index*.\n"
    "Negative indices are supported. Raises IndexError if out of range.");
PyDoc_STRVAR(
    py_bv_set__doc__,
    "set(index: int) -> None\n"
    "\n"
    "Set the bit at position *index* to True. Supports negative indexing.\n"
    "Raises IndexError if out of range.");
PyDoc_STRVAR(py_bv_clear__doc__,
             "clear(index: int) -> None\n"
             "\n"
             "Clear the bit (set to False) at position *index*. Supports "
             "negative indexing.\n"
             "Raises IndexError if out of range.");
PyDoc_STRVAR(
    py_bv_flip__doc__,
    "flip(index: int) -> None\n"
    "\n"
    "Toggle the bit at position *index*. Supports negative indexing.\n"
    "Raises IndexError if out of range.");
PyDoc_STRVAR(py_bv_set_range__doc__,
             "set_range(start: int, length: int) -> None\n"
             "\n"
             "Set all bits in the half-open range [start, start+length).\n"
             "Raises IndexError if the range is out of bounds.");
PyDoc_STRVAR(py_bv_clear_range__doc__,
             "clear_range(start: int, length: int) -> None\n"
             "\n"
             "Clear all bits in the half-open range [start, start+length).\n"
             "Raises IndexError if the range is out of bounds.");
PyDoc_STRVAR(py_bv_flip_range__doc__,
             "flip_range(start: int, length: int) -> None\n"
             "\n"
             "Toggle all bits in the half-open range [start, start+length).\n"
             "Raises IndexError if the range is out of bounds.");
PyDoc_STRVAR(py_bv_copy__doc__,
             "copy() -> BitVector\n"
             "\n"
             "Return a copy of this BitVector.");
PyDoc_STRVAR(py_bv_copy_inline__doc__,
             "__copy__() -> BitVector\n"
             "\n"
             "Return a copy of this BitVector.");
PyDoc_STRVAR(py_bv_deepcopy__doc__,
             "__deepcopy__(memo: dict) -> BitVector\n"
             "\n"
             "Return a copy of this BitVector, registering it in *memo*.");
PyDoc_STRVAR(
    py_bv_rank__doc__,
    "rank(index: int) -> int\n"
    "\n"
    "Count the number of bits set to True in the half-open range [0..index].\n"
    "Supports negative indexing. Raises IndexError if out of range.");
/**
 * @brief Method table for BitVector core methods.
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
