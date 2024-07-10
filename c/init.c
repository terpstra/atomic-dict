#include "types.h"
#include "methods.h"
#include "doc.h"


struct PyModuleDef atomic_dict_c_module = {
        PyModuleDef_HEAD_INIT,
        .m_name = "atomic_dict_c",
        .m_doc = atomic_dict_doc,
        .m_size = -1
};

PyMethodDef atomic_array_methods[] = {
        {"index", (PyCFunction) atomic_array_index, METH_FASTCALL, atomic_array_index_doc},
        {NULL}  /* Sentinel */
};

PyTypeObject AtomicArrayType = {
        .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "atomic_dict_c.AtomicArray",
        .tp_doc = atomic_array_doc,
        .tp_basicsize = sizeof(AtomicArray),
        .tp_itemsize = 0,
        .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
        .tp_new = PyType_GenericNew,
        .tp_init = (initproc) atomic_array_init,
        .tp_methods = atomic_array_methods,
};

PyMethodDef atomic_value_methods[] = {
        {"load",  (PyCFunction) atomic_value_load,  METH_FASTCALL, atomic_value_load_doc},
        {"store", (PyCFunction) atomic_value_store, METH_FASTCALL, atomic_value_store_doc},
        {"swap",  (PyCFunction) atomic_value_swap,  METH_FASTCALL, atomic_value_swap_doc},
        {"add",   (PyCFunction) atomic_value_add,   METH_FASTCALL, atomic_value_add_doc},
        {"sub",   (PyCFunction) atomic_value_sub,   METH_FASTCALL, atomic_value_sub_doc},
        {"and",   (PyCFunction) atomic_value_and,   METH_FASTCALL, atomic_value_and_doc},
        {"or",    (PyCFunction) atomic_value_or,    METH_FASTCALL, atomic_value_or_doc},
        {"xor",   (PyCFunction) atomic_value_xor,   METH_FASTCALL, atomic_value_xor_doc},
        {"cas",   (PyCFunction) atomic_value_cas,   METH_FASTCALL, atomic_value_cas_doc},
        {NULL}  /* Sentinel */
};

PyTypeObject AtomicValueType = {
        .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "atomic_dict_c.AtomicValue",
        .tp_basicsize = sizeof(AtomicValue),
        .tp_itemsize = 0,
        .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
        .tp_new = PyType_GenericNew,
        .tp_methods = atomic_value_methods,
};

PyMODINIT_FUNC PyInit_atomic_dict_c(void) {
    PyObject *m;
    if (PyType_Ready(&AtomicArrayType) < 0 || PyType_Ready(&AtomicValueType) < 0) {
        return NULL;
    }

    m = PyModule_Create(&atomic_dict_c_module);
    if (m == NULL) {
        return NULL;
    }

    if (PyModule_AddObjectRef(m, "AtomicArray", (PyObject *) &AtomicArrayType) < 0) {
        Py_DECREF(m);
        return NULL;
    }

    if (PyModule_AddObjectRef(m, "AtomicValue", (PyObject *) &AtomicValueType) < 0) {
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
