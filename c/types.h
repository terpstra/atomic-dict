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
} AtomicValue64;

typedef struct {
    PyObject_HEAD
    atomic_uint_fast32_t *val;
} AtomicValue32;

typedef struct {
    PyObject_HEAD
    AtomicArray *array;
    Py_ssize_t offset;
} DictIterator;

#endif
