# Atomic-Dict: A library for lock-free shared 64-bit dictionary

Atomic-Dict provides a shared Map[Int, Int] for 64-bit integer values.
Create an AtomicDict(), fork, then update keyed values using atomic operations.
Supported operations include compare-and-swap, exchange, increment, etc.

## Limitations

This is not a general purpose dictionary implementation.
It is designed to be used as a synchronization primitive.
Key limitations include:

* The key must be a non-zero 64-bit value.
* The value for freshly allocated keys are always initialized as 0.
* Keys allocated into the dictionary can never be removed.
* The maximum size of the dictionary must be specified upfront.

To support more complex types, build shared a list[XYZ] before fork(),
then use indexes into those lists as the keys and values of the AtomicDict.

## Performance

While bare bones, AtomicDict is fast.
Operations never lock, never wait, and leverage cache locality.
It is hard to imagine a faster shared dictionary implementation.

## Example use

```python
from atomic_dict import AtomicDict
import multiprocessing

if __name__ == "__main__":
  dict = AtomicDict(1024*1024)
  context = multiprocessing.get_context("fork")
  def worker(id: int) -> None:
      for _ in range(32*1024):
          idx = dict[0].add(1) # also: or, xor, and, swap, cas(expected, replacement)
          dict[100 + idx] = id
  with context.Pool(8) as p:
      p.map(worker, range(16))
```

