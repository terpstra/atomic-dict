#ifndef ATOMIC_DICT_TYPES_H
#define ATOMIC_DICT_TYPES_H

#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <stdatomic.h>

typedef struct {
  atomic_uint_fast64_t keys[4];
  atomic_uint_fast64_t vals[4];
} AtomicCacheBlock;

typedef struct {
    PyObject_HEAD
    AtomicCacheBlock *blocks;
    Py_ssize_t num_blocks;
} AtomicArray;

typedef struct {
    PyObject_HEAD
    atomic_uint_fast64_t *val;
} AtomicValue;

#endif
