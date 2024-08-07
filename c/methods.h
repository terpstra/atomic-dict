#ifndef ATOMIC_DICT_METHODS_H
#define ATOMIC_DICT_METHODS_H

#include "types.h"

int atomic_array_init(AtomicArray *self, PyObject *args, PyObject *kwds);

AtomicValue *atomic_array_index(AtomicArray *self, PyObject * const *args, Py_ssize_t nargs);

DictIterator *atomic_array_iterator(AtomicArray *self, PyObject * const *Args, Py_ssize_t nargs);


PyObject *atomic_value_load(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);

PyObject *atomic_value_store(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);

PyObject *atomic_value_swap(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);

PyObject *atomic_value_add(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);

PyObject *atomic_value_sub(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);

PyObject *atomic_value_and(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);

PyObject *atomic_value_or(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);

PyObject *atomic_value_xor(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);

PyObject *atomic_value_cas(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);


PyObject *dict_iterator_key(DictIterator *self, PyObject * const *args, Py_ssize_t nargs);

PyObject *dict_iterator_value(DictIterator *self, PyObject * const *args, Py_ssize_t nargs);

PyObject *dict_iterator_next(DictIterator *self, PyObject * const *args, Py_ssize_t nargs);


PyObject *get_pointer(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs);

#endif
