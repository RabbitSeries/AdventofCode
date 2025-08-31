import importlib.resources as ir
import random

from aocpy.aoc_launcher.launcher import import_solution
from aocpy.aoc_libs.utils.ISolution import ISolution


def test_random():
    day = random.randrange(1, 26)
    clazz = import_solution(2022, day)
    assert issubclass(clazz, ISolution)
    ins = clazz()
    with ir.files("aocpy.aoc_solutions.year2022.resources.Day{:0>2d}".format(day)).joinpath("input.txt").open("r") as f:
        ins.Part1(f)
    with ir.files("aocpy.aoc_solutions.year2022.resources.Day{:0>2d}".format(day)).joinpath("input.txt").open("r") as f:
        ins.Part2(f)


if __name__ == "__main__":
    test_random()
