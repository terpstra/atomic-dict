#include "methods.h"

extern PyTypeObject AtomicValueType;
extern PyTypeObject DictIteratorType;

int atomic_array_init(AtomicArray *self, PyObject *args, PyObject *kwds) {
    PyObject *memory_view;
    Py_buffer *buffer;

    if (!PyArg_ParseTuple(args, "O|", &memory_view)) {
        return -1;
    }

    buffer = PyMemoryView_GET_BUFFER(memory_view);
    if (!buffer)
        return -1;

    self->blocks = (AtomicCacheBlock *)buffer->buf;
    self->num_blocks = buffer->len / 64;

    return 0;
}

static uint64_t key_hash(uint64_t key) {
    // Lifted from MumurHash3 fmix64
    key ^= key >> 33;
    key *= UINT64_C(0xFF51AFD7ED558CCD);
    key ^= key >> 33;
    key *= UINT64_C(0xC4CEB9FE1A85EC53);
    key ^= key >> 33;

    return key;
}

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define CHECK_ARGN(fn, n) if (nargs != n) {                                               \
    PyErr_SetString(PyExc_TypeError, fn " expected " STR(n) " arguments"); \
    return 0;                                                                             \
}

AtomicValue *atomic_array_index(AtomicArray *self, PyObject * const *args, Py_ssize_t nargs) {
    atomic_uint_fast64_t key;
    atomic_uint_fast64_t expected;
    uint64_t             hash;
    Py_ssize_t           index;
    Py_ssize_t           stride;
    Py_ssize_t           attempts;
    AtomicValue         *value;

    CHECK_ARGN("AtomicArray.index", 1);

    key = PyLong_AsUnsignedLongLong(args[0]);
    
    value = PyObject_New(AtomicValue, &AtomicValueType);
    if (!value) return value;

    hash = key_hash(key);
    index = hash & (self->num_blocks - 1);
    stride = ((hash >> 32) & (self->num_blocks - 1)) | 1;

    for (attempts = 0; attempts < self->num_blocks; ++attempts) {
        AtomicCacheBlock *block = self->blocks + index;

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

    if (attempts == self->num_blocks) {
        Py_DECREF(value);
        PyErr_SetString(PyExc_ValueError, "AtomicArray capacity exceeded");
        return 0;
    }

    return value;
}

DictIterator *atomic_array_iterator(AtomicArray *self, PyObject * const *args, Py_ssize_t nargs) {
    DictIterator *out;

    CHECK_ARGN("AtomicArray.iterator", 0);

    out = PyObject_New(DictIterator, &DictIteratorType);
    if (out) {
        out->array = self;
        out->offset = 0;
    }

    return out;
}

PyObject *atomic_value_load(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    CHECK_ARGN("AtomicValue.load", 0);
    return PyLong_FromUnsignedLongLong(atomic_load(self->val));
}

PyObject *atomic_value_store(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    CHECK_ARGN("AtomicValue.store", 1);
    atomic_store(self->val, PyLong_AsUnsignedLongLong(args[0]));
    Py_RETURN_NONE;
}

PyObject *atomic_value_swap(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    CHECK_ARGN("AtomicValue.swap", 1);
    return PyLong_FromUnsignedLongLong(atomic_exchange(self->val, PyLong_AsUnsignedLongLong(args[0])));
}

PyObject *atomic_value_add(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    CHECK_ARGN("AtomicValue.add", 1);
    return PyLong_FromUnsignedLongLong(atomic_fetch_add(self->val, PyLong_AsUnsignedLongLong(args[0])));
}

PyObject *atomic_value_sub(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    CHECK_ARGN("AtomicValue.sub", 1);
    return PyLong_FromUnsignedLongLong(atomic_fetch_sub(self->val, PyLong_AsUnsignedLongLong(args[0])));
}

PyObject *atomic_value_and(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    CHECK_ARGN("AtomicValue.and", 1);
    return PyLong_FromUnsignedLongLong(atomic_fetch_and(self->val, PyLong_AsUnsignedLongLong(args[0])));
}

PyObject *atomic_value_or(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    CHECK_ARGN("AtomicValue.or", 1);
    return PyLong_FromUnsignedLongLong(atomic_fetch_or(self->val, PyLong_AsUnsignedLongLong(args[0])));
}

PyObject *atomic_value_xor(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    CHECK_ARGN("AtomicValue.xor", 1);
    return PyLong_FromUnsignedLongLong(atomic_fetch_xor(self->val, PyLong_AsUnsignedLongLong(args[0])));
}

PyObject *atomic_value_cas(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    CHECK_ARGN("AtomicValue.cas", 2);
    atomic_uint_fast64_t expected = PyLong_AsUnsignedLongLong(args[0]);
    atomic_uint_fast64_t desired  = PyLong_AsUnsignedLongLong(args[1]);
    atomic_compare_exchange_strong(self->val, &expected, desired);
    return PyLong_FromUnsignedLongLong(expected);
}

PyObject *dict_iterator_key(DictIterator *self, PyObject * const *args, Py_ssize_t nargs) {
    AtomicCacheBlock *block = self->array->blocks + (self->offset >> 2);
    AtomicCacheBlock *end = self->array->blocks + self->array->num_blocks;

    CHECK_ARGN("DictIterator.key", 0);

    while (block != end) {
        atomic_uint_fast64_t key = atomic_load(&block->keys[self->offset & 3]);
        if (key) return PyLong_FromUnsignedLongLong(key);
        ++self->offset;
        if (0 == (self->offset & 3)) ++block;
    }

    return PyLong_FromUnsignedLongLong(0);
}

PyObject *dict_iterator_value(DictIterator *self, PyObject * const *args, Py_ssize_t nargs) {
    AtomicCacheBlock *block = self->array->blocks + (self->offset >> 2);
    AtomicCacheBlock *end = self->array->blocks + self->array->num_blocks;

    CHECK_ARGN("DictIterator.value", 0);

    while (block != end) {
        atomic_uint_fast64_t key = atomic_load(&block->keys[self->offset & 3]);
        if (key) {
            atomic_uint_fast64_t val = atomic_load(&block->vals[self->offset & 3]);
            return PyLong_FromUnsignedLongLong(val);
        }
        ++self->offset;
        if (0 == (self->offset & 3)) ++block;
    }

    return PyLong_FromUnsignedLongLong(0);
}

PyObject *dict_iterator_next(DictIterator *self, PyObject * const *args, Py_ssize_t nargs) {
    CHECK_ARGN("DictIterator.next", 0);

    if (self->offset >> 2 < self->array->num_blocks) {
        ++self->offset;
    }

    Py_RETURN_NONE;
}

PyObject *get_pointer(AtomicValue *self, PyObject * const *args, Py_ssize_t nargs) {
    CHECK_ARGN("get_pointer", 1);

    const char *name = PyCapsule_GetName(args[0]);
    if (!name) return 0;

    void *pointer = PyCapsule_GetPointer(args[0], name);
    if (!pointer) return 0;

    return PyLong_FromUnsignedLongLong((uint64_t)pointer);
}
