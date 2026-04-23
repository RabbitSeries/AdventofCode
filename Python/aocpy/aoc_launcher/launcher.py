import pkgutil
import importlib
import importlib.resources as ir
from sys import stderr
from aocpy.aoc_libs.utils.ISolution import ISolution

import time

from queue import PriorityQueue


def main():
    pq: PriorityQueue[tuple[float, str]] = PriorityQueue()
    total = 0
    for year in [2022]:
        for day in range(1, 26):
            try:
                now = time.time()
                clazz = import_solution(year, day)
                running = "year{}.Day{:0>2d}.{}:".format(year, day, clazz.__name__)
                print(running)
                if clazz is None:
                    raise ModuleNotFoundError(f"year{year}.Day{day} is not found")
                ins = clazz()
                for method in ["Part1", "Part2"]:
                    with ir.files("aocpy.aoc_solutions.year{0}.resources.Day{1:0>2d}".format(year, day)).joinpath("input.txt").open("r") as f:
                        print("\t", end="")
                        getattr(ins, method)(f)
                delta = time.time() - now
                total += delta
                pq.put((delta, running))
            except Exception as exp:
                print(exp, file=stderr)
                exit(-1)
    print(f"Total time: {total}", file=stderr)
    i = 1
    while not pq.empty():
        cost, day = pq.get()
        print(f"Time cost ranking {i} {day}:", file=stderr)
        print(f"\t{cost}s", file=stderr)
        i += 1


def import_solution(year: int, day: int):
    package = importlib.import_module("aocpy.aoc_solutions.year{0}.Day{1:0>2d}".format(year, day))
    for _, pkname, ispkg in pkgutil.walk_packages(package.__path__):
        if not ispkg:
            module = importlib.import_module(package.__name__+"."+pkname)
            clazz = getattr(module, pkname)
            if issubclass(clazz, ISolution):
                return clazz
    return None


if __name__ == "__main__":
    main()
