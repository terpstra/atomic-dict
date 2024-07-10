#ifndef ATOMIC_DICT_METHODS_H
#define ATOMIC_DICT_METHODS_H

#include "types.h"

int atomic_array_init(AtomicArray *self, PyObject *args, PyObject *kwds);

PyObject *atomic_array_index(AtomicArray *self, PyObject * const *args, Py_ssize_t nargs);


PyObject *atomic_value_swap(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);

PyObject *atomic_value_add(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);

PyObject *atomic_value_and(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);

PyObject *atomic_value_or(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);

PyObject *atomic_value_xor(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);

#endif