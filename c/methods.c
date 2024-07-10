#include "methods.h"

extern PyTypeObject AtomicValueType;

int atomic_array_init(AtomicInt *self, PyObject *args, PyObject *kwds) {
    PyObject *memory_view;
    Py_buffer buffer;

    if (!PyArg_ParseTuple(args, "O|", &memory_view, &blocks)) {
        return -1;
    }

    buffer = PyMemoryView_GET_BUFFER(memory_view);
    self->blocks = (AtomicCacheBlock *)buffer.buf;
    self->num_blocks = buffer.len / 64;

    return 0;
}

static uint64_t key_hash(uint64_t key) {
    // Lifted from MumurHash3 fmix64
    key ^= key >> 33;
    key *= UINT64_C(0xFF51AFD7ED558CCD);
    key ^= key >> 33;
    key *= UINT64_C(0xC4CEB9FE1A85EC53);
    key ^= key >> 33

    return key;
}

PyObject *atomic_array_index(AtomicArray *self, PyObject * const *args, Py_ssize_t nargs) {
    atomic_uint_fast64_t key;
    atomic_uint_fast64_t expected;
    uint64_t             hash;
    Py_ssize_t           index;
    Py_ssize_t           stride;
    AtomicValue         *value;

    key = PyLong_AsUnsignedLongLong(args[0]);
    
    value = PyObject_New(AtomicValue, &AtomicValueType);
    if (!value) return value;

    hash = key_hash(key);
    index = hash & (self->num_blocks - 1);
    stride = ((hash >> 32) & (self->num_blocks - 1)) | 1;

    while (true) {
        AtomicBlock *block = self->blocks + index;

        expected = 0;
        atomic_compare_exchange_strong(&block->keys[0], &expected, key);
        if (expected == 0 || expected == key) { value->val = &block->vals[0]; break; }

        expected = 0;
        atomic_compare_exchange_strong(&block->keys[1], &expected, key);
        if (expected == 0 || expected == key) { value->val = &block->vals[1]; break; }

        expected = 0;
        atomic_compare_exchange_strong(&block->keys[2], &expected, key);
        if (expected == 0 || expected == key) { value->val = &block->vals[2]; break; }

        expected = 0;
        atomic_compare_exchange_strong(&block->keys[3], &expected, key);
        if (expected == 0 || expected == key) { value->val = &block->vals[3]; break; }

        index = (index + stride) & (self->num_blocks - 1);
    }

    return (PyObject *)value;
}

PyObject *atomic_value_swap(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    if (nargs > 0) {
        return PyLong_FromUnsignedLongLong(atomic_exchange(self->val, PyLong_AsUnsignedLongLong(args[0])));
    }
    Py_RETURN_NONE;
}

PyObject *atomic_value_add(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    if (nargs > 0) {
        return PyLong_FromUnsignedLongLong(atomic_add(self->val, PyLong_AsUnsignedLongLong(args[0])));
    }
    Py_RETURN_NONE;
}

PyObject *atomic_value_and(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    if (nargs > 0) {
        return PyLong_FromUnsignedLongLong(atomic_and(self->val, PyLong_AsUnsignedLongLong(args[0])));
    }
    Py_RETURN_NONE;
}

PyObject *atomic_value_or(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    if (nargs > 0) {
        return PyLong_FromUnsignedLongLong(atomic_or(self->val, PyLong_AsUnsignedLongLong(args[0])));
    }
    Py_RETURN_NONE;
}

PyObject *atomic_value_xor(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    if (nargs > 0) {
        return PyLong_FromUnsignedLongLong(atomic_xor(self->val, PyLong_AsUnsignedLongLong(args[0])));
    }
    Py_RETURN_NONE;
}