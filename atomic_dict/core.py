"""This module provides a data structure which can be used between multiple processes."""

from mmap import MAP_SHARED, PROT_READ, PROT_WRITE, mmap

from atomic_dict.atomic_dict_c import AtomicArray, AtomicValue, DictIterator

class DictEntryIterator:
    it: DictIterator

    def __init__(self, it: DictIterator):
        self.it = it

    def __next__(self) -> (int, int):
        """Return the current element and advance the iterator."""

        key = self.it.key()
        val = self.it.value()

        if key == 0:
            raise StopIteration()

        self.it.next()
        return (key, val)

class AtomicDict:
    mm: mmap
    mv: memoryview
    aa: AtomicArray

    def __init__(self, max_entries: int) -> None:
        """Create a multi-process / multi-threaded shared dictionary.

        Once created, fork()'d child processes will share this map with the parent.
        """
        # we want a load factor <= 0.5
        max_entries *= 2

        # round size up to a power-of-two
        size = 1 << (max_entries - 1).bit_length()

        # size must be at least 256 (for a 4kiB mmap)
        if size < 256:
            size = 256

        # We need 16-bytes per entry
        self.mm = mmap(-1, 16 * size, flags=MAP_SHARED, prot=PROT_WRITE | PROT_READ)

        # Convert to a mutable view
        self.mv = memoryview(self.mm)

        # Pass the shared memory to C
        self.aa = AtomicArray(self.mv)

    def __del__(self) -> None:
        """Close the shared memory map uoon destruction.

        This will only close the copy held by a given process.
        The map will stay alive until all users have called __del__.
        """

        self.mv.release()
        self.mm.close()

    def __getitem__(self, key: int) -> AtomicValue:
        """dict[key] will return the AtomicValue associated with key

        key must be a non-zero 64-bit unsigned integer.
        The initial value of the AtomicValue is 0.
        """

        return self.aa.index(key)

    def __setitem__(self, key: int, value: int) -> None:
        """`dict[key] = value` will update the AtomicValue associated with key to value.

        key must be a non-zero 64-bit unsigned integer.
        value must be a 64-bit unsigned integer.
        """
        self.aa.index(key).store(value)

    def __iter__(self) -> DictEntryIterator:
        """NON-ATOMICALLY iterate through the AtomicDict contents."""

        return DictEntryIterator(self.aa.iterator())
