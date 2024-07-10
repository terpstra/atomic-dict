#ifndef ATOMIC_DICT_DOC_H
#define ATOMIC_DICT_DOC_H

#include "types.h"

PyDoc_STRVAR(
        atomic_dict_doc,
        "Internal C bindings for AtomicArray and AtomicValue");

PyDoc_STRVAR(
        atomic_array_doc,
        "An AtomicArray type\n"
        "\n"
        "AtomicArray(memory_view)\n"
        "\n"
        "Parameters\n"
        "----------\n"
        "memory_view : The MemoryView to use as shared storage");

PyDoc_STRVAR(
        atomic_array_index_doc,
        "index(self, key)\n"
        "--\n"
        "\n"
        "Retrieves or installs key into the AtomicArray\n"
        "\n"
        "Parameters\n"
        "----------\n"
        "key : A non-zero 64-bit unsigned integer to use as the key in the AtomicArray\n"
        "\n"
        "Return value\n"
        "----------\n"
        "The AtomicValue assigned to this key in the AtomicArray");

PyDoc_STRVAR(
        atomic_value_load_doc,
        "load(self)\n"
        "--\n"
        "\n"
        "Return the value of the AtomicValue\n"
        "\n"
        "Return value\n"
        "----------\n"
        "The value of in the AtomicValue");

PyDoc_STRVAR(
        atomic_value_store_doc,
        "storep(self, arg)\n"
        "--\n"
        "\n"
        "Store arg to the AtomicValue\n"
        "\n"
        "Parameters\n"
        "----------\n"
        "arg : Value to be stored in the AtomicValue\n"
        "\n"
        "Return value\n"
        "----------\n"
        "None");

PyDoc_STRVAR(
        atomic_value_swap_doc,
        "swap(self, arg)\n"
        "--\n"
        "\n"
        "Swaps arg with the AtomicValue\n"
        "\n"
        "Parameters\n"
        "----------\n"
        "arg : Value to be stored in the AtomicValue\n"
        "\n"
        "Return value\n"
        "----------\n"
        "The value immediately preceding the effects of this function");

PyDoc_STRVAR(
        atomic_value_add_doc,
        "add(self, arg)\n"
        "--\n"
        "\n"
        "Adds arg to the AtomicValue\n"
        "\n"
        "Parameters\n"
        "----------\n"
        "arg : Value to be added to the AtomicValue\n"
        "\n"
        "Return value\n"
        "----------\n"
        "The value immediately preceding the effects of this function");

PyDoc_STRVAR(
        atomic_value_sub_doc,
        "sub(self, arg)\n"
        "--\n"
        "\n"
        "Subtracts arg from the AtomicValue\n"
        "\n"
        "Parameters\n"
        "----------\n"
        "arg : Value to be subtracted from the AtomicValue\n"
        "\n"
        "Return value\n"
        "----------\n"
        "The value immediately preceding the effects of this function");

PyDoc_STRVAR(
        atomic_value_and_doc,
        "and(self, arg)\n"
        "--\n"
        "\n"
        "Bitwise AND arg with the AtomicValue\n"
        "\n"
        "Parameters\n"
        "----------\n"
        "arg : Value to be AND'd with the AtomicValue\n"
        "\n"
        "Return value\n"
        "----------\n"
        "The value immediately preceding the effects of this function");


PyDoc_STRVAR(
        atomic_value_or_doc,
        "or(self, arg)\n"
        "--\n"
        "\n"
        "Bitwise OR arg with the AtomicValue\n"
        "\n"
        "Parameters\n"
        "----------\n"
        "arg : Value to be OR'd with the AtomicValue\n"
        "\n"
        "Return value\n"
        "----------\n"
        "The value immediately preceding the effects of this function");

PyDoc_STRVAR(
        atomic_value_xor_doc,
        "xor(self, arg)\n"
        "--\n"
        "\n"
        "Bitwise XOR arg with the AtomicValue\n"
        "\n"
        "Parameters\n"
        "----------\n"
        "arg : Value to be XOR'd with the AtomicValue\n"
        "\n"
        "Return value\n"
        "----------\n"
        "The value immediately preceding the effects of this function");

PyDoc_STRVAR(
        atomic_value_cas_doc,
        "cas(self, expected, desired)\n"
        "--\n"
        "\n"
        "Compare the AtomicValue to expected."
        "If they are equal, store desired in the AtomicValue\n"
        "\n"
        "Parameters\n"
        "----------\n"
        "expected : Value to be compared with the AtomicValue\n"
        "desired : Value to be store to the AtomicValue\n"
        "\n"
        "Return value\n"
        "----------\n"
        "The value immediately preceding the effects of this function");

PyDoc_STRVAR(
        get_pointer_doc,
        "get_pointer(capsule)\n"
        "--\n"
        "\n"
        "Retrieve the underlying pointer of a capsule."
        "This might be useful as a key for AtomicDict\n"
        "\n"
        "Parameters\n"
        "----------\n"
        "capsule : PyCapsule for the C object to identify\n"
        "\n"
        "Return value\n"
        "----------\n"
        "An unsigned long corresponding to the capsule's pointer");

#endif
