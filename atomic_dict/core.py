"""This module provides a data structure which can be used between multiple processes."""

from mmap import MAP_SHARED, PROT_READ, PROT_WRITE, mmap

from atomic_dict.capi import AtomicArray, AtomicValue32, AtomicValue64, DictIterator

class DictEntryIterator:
    it: DictIterator

    def __init__(self, it: DictIterator):
        self.it = it

    def __next__(self) -> tuple[tuple[int, ...], int]:
        """Return the current element and advance the iterator."""

        key = self.it.key()
        val = self.it.value()

        if not key:
            raise StopIteration()

        assert val is not None

        self.it.next()
        return (key, val)

class AtomicDict:
    mm: mmap
    mv: memoryview
    aa: AtomicArray

    def __init__(self, max_entries: int, k64: int = 1, k32: int = 0, v64: int = 1, v32: int = 0) -> None:
        """Create a multi-process / multi-threaded shared dictionary.

        Once created, fork()'d child processes will share this map with the parent.
        """
        # we want 3/8 <= load factor <= 3/4
        max_entries = (max_entries * 3) // 2

        # calculate how many rows per cache-block
        nbytes = (k64 + v64) * 8 + (k32 + v32) * 4
        rows = 64 // nbytes
        if rows < 1:
            rows = 1

        # round blocks up to a power-of-two
        blocks = (max_entries + rows - 1) // rows
        blocks = 1 << (blocks - 1).bit_length()

        # blocks must be at least 64 (for a 4kiB mmap)
        if blocks < 64:
            blocks = 64

        # We need 64-bytes per block
        self.mm = mmap(-1, 64 * blocks, flags=MAP_SHARED, prot=PROT_WRITE | PROT_READ)

        # Convert to a mutable view
        self.mv = memoryview(self.mm)

        # Pass the shared memory to C
        self.aa = AtomicArray(self.mv, k64, k32, v64, v32)

    def __del__(self) -> None:
        """Close the shared memory map uoon destruction.

        This will only close the copy held by a given process.
        The map will stay alive until all users have called __del__.
        """

        self.mv.release()
        self.mm.close()

    def __getitem__(self, key: int | tuple[int, ...]) -> AtomicValue32 | AtomicValue64:
        """dict[key] will return the AtomicValue associated with key

        key must be a non-zero 64-bit unsigned integer.
        The initial value of the AtomicValue is 0.
        """

        if isinstance(key, int):
            return self.aa.index(key)
        else:
            return self.aa.index(*key)

    def __setitem__(self, key: int | tuple[int, ...], value: int) -> None:
        """`dict[key] = value` will update the AtomicValue associated with key to value.

        key must be a non-zero 64-bit unsigned integer.
        value must be a 64-bit unsigned integer.
        """

        if isinstance(key, int):
            self.aa.index(key).store(value)
        else:
            self.aa.index(*key).store(value)

    def __iter__(self) -> DictEntryIterator:
        """NON-ATOMICALLY iterate through the AtomicDict contents."""

        return DictEntryIterator(self.aa.iterator())
