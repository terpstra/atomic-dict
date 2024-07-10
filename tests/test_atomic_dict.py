from atomic_dict import AtomicDict
import multiprocessing

if __name__ == "__main__":
    dict = AtomicDict(1024*1024)
    dict[20] = 52
    dict[3] = 99
    for (key, val) in dict:
        print(f"({key}, {val})")

    context = multiprocessing.get_context("fork")

    def worker(id: int) -> None:
        for _ in range(32*1024):
            idx = dict[1].add(1)
            dict[100 + idx] = id

    with context.Pool(8) as p:
        p.map(worker, range(16))

    assert dict[1].load() == 32*1024*16
    print("OK!")
