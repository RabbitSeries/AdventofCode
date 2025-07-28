import importlib
from aoc_libs.utils.ISolution import ISolution
from inspect import getmembers, isclass
module = importlib.import_module("Day17.PyroclasticFlow")
for name, meta in getmembers(module):
    if isclass(meta) and issubclass(meta, ISolution) and meta != ISolution:
        print(name)
