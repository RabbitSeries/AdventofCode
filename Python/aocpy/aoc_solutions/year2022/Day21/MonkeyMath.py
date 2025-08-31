from __future__ import annotations
import re
from sympy import symbols, Eq, solve
from aocpy.aoc_libs.utils.ISolution import ISolution


class Node:
    def __init__(self, name: str, children: tuple[Node, Node] | None = None) -> None:
        self.name, self.children = name, children


class MonkeyMath(ISolution):

    def build_tree(self, name: str) -> Node:
        node, identity = Node(name), self.parsed[name]
        if isinstance(identity, int):
            return node
        node.children = (self.build_tree(identity[0]),  self.build_tree(identity[2]))
        return node

    def evaluate(self, root: Node) -> int:
        # actually the input data is a strict binary tree, not a graph (no cache hit), this cache can be omitted
        # if root.name in self.evaluated:
        # return self.evaluated[root.name]
        val = self.parsed[root.name]
        if isinstance(val, int):
            self.evaluated.update({root.name: val})
            return val
        if not root.children:
            raise ValueError("Expecting children")
        lft, rgt = self.evaluate(root.children[0]), self.evaluate(root.children[1])
        val = int(eval(f'{lft} {val[1]} {rgt}'))
        self.evaluated.update({root.name: val})
        return val

    def Part1(self, f):
        data = f.read().splitlines()
        re_lone_number, re_operation = re.compile(R"(\w+): (\d+)"), re.compile(R"(\w+): (\w+) ([+\-*/]) (\w+)")
        self.parsed: dict[str, int | tuple[str, str, str]] = {}
        for line in data:
            if m := re_lone_number.match(line):
                self.parsed.update({m.group(1): int(m.group(2))})
                continue
            if m := re_operation.match(line):
                self.parsed.update({m.group(1): (m.group(2), m.group(3), m.group(4))})
                continue
            raise ValueError('There might be more operations')
        self.evaluated: dict[str, int] = {}
        self.root = self.build_tree('root')
        print(f'Part 1: {self.evaluate(self.root)}')

    def pathToHumn(self, root: Node) -> list[str] | None:
        path = [root.name]
        if root.name == 'humn':
            return path
        if root.children:
            if subPath := self.pathToHumn(root.children[0]):
                path += subPath
                return path
            if subPath := self.pathToHumn(root.children[1]):
                path += subPath
                return path
        return None

    def Part2(self, f):
        path2Humn = self.pathToHumn(self.root)
        if not path2Humn:
            return
        p = self.root
        expectVal = None
        for node in path2Humn[1:]:
            formula = self.parsed[p.name]
            if not p.children or isinstance(formula, int):
                raise ValueError('Expecting children')
            lft, rgt = p.children[0].name, p.children[1].name
            p = p.children[0] if lft == node else p.children[1]
            if expectVal is None:  # root node, child's expecting val is its brother's evaluated value
                expectVal = self.evaluated[rgt] if lft == node else self.evaluated[lft]
                continue
            x = symbols('x')
            if lft == node:
                expectVal = solve(Eq(eval(f'x {formula[1]} {self.evaluated[rgt]}'), expectVal), x, dict=True)[0][x]
            else:
                expectVal = solve(Eq(eval(f'{self.evaluated[lft]} {formula[1]} x'), expectVal), x, dict=True)[0][x]
        print(f'Part 2: {expectVal}')


if __name__ == '__main__':
    day21 = MonkeyMath()
    day21.Part1()
    day21.Part2()
