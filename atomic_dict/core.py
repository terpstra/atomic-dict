"""This module provides a data structure which can be used between multiple processes."""

from mmap import MAP_SHARED, PROT_READ, PROT_WRITE, mmap


class AtomicDict:
    mm: mmap
    mv: memoryview

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

    def __del__(self) -> None:
        """Close the shared memory map uoon destruction.

        This will only close the copy held by a given process.
        The map will stay alive until all users have called __del__.
        """

        self.mv.release()
        self.mm.close()

