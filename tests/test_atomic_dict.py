from atomic_dict.core import AtomicDict
import multiprocessing

if __name__ == "__main__":
    dict = AtomicDict(1024*1024)
    context = multiprocessing.get_context("fork")
    def worker(id: int) -> None:
        for _ in range(32*1024):
            pass
    with context.Pool(8) as p:
        p.map(worker, range(16))

    print("OK!")
