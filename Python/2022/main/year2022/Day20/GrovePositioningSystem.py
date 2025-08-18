from __future__ import annotations

from aoc_libs.utils.ISolution import ISolution

# WTF? A simple linked list cost me so much time?


class Node:
    def __init__(self, id: int, nums: list[int], next: Node | None, pre: Node | None):
        self.id, self.nums, self.next, self.pre = id, nums, next, pre

    def __repr__(self) -> str:
        return f'{self.nums[self.id]}'


class LinkedList:
    def __init__(self, nums: list[int]):
        self.head: Node = Node(-1, nums, None, None)
        self.nums = nums
        self.len = 0

    def __repr__(self) -> str:
        l = []
        p = self.head.next
        for _ in range(self.len):
            if p is not None:
                l.append({self.nums[p.id]: p.id})
                p = p.next
        return f"{l}"

    def push_back(self, node: Node):
        if self.head.next is None:
            self.head.next = node
            node.pre = node
            node.next = node
        else:
            tail = self.head.next.pre
            if tail is not None:
                tail.next = node
                node.pre = tail
                node.next = self.head.next
                self.head.next.pre = node

        self.len = self.len + 1

    def pivot(self, id: int):
        value = self.nums[id]
        if (value % (self.len - 1)) == 0:
            return

        p = self.find(id)

        if p and self.head.next and p.id == self.head.next.id:
            self.head.next = p.next

        if not (p and p.pre and p.next):
            raise ValueError()
        p.pre.next = p.next
        p.next.pre = p.pre
        self.len = self.len - 1

        q = self.seek(p, value)
        if not (q and q.next and q.pre):
            raise ValueError()
        if value > 0:
            #! Don't change the order
            p.next = q.next
            p.pre = q
            q.next.pre = p
            q.next = p
        else:
            p.next = q
            p.pre = q.pre
            q.pre.next = p
            q.pre = p
        self.len = self.len + 1

    def find(self, id: int):
        p = self.head.next
        i = 0
        while p is not None and p.id != id and i < self.len:
            p = p.next
            i = i + 1
        return p

    def seek(self, begin: Node | None, offset: int):
        pos = abs(offset) % self.len
        i = 0
        while begin is not None and i < pos:
            if offset > 0:
                begin = begin.next
            else:
                begin = begin.pre
            i = i + 1
        return begin


class GrovePositioningSystem(ISolution):
    def __init__(self):
        with open("Day20/input.txt") as f:
            self.nums = [num for num in map(int, f.read().splitlines())]
        self.cycle = len(self.nums)
        self.zeroid = self.nums.index(0)

    def buildLinkedList(self, nums: list[int]):
        linkedList = LinkedList(nums)
        for i in range(self.cycle):
            linkedList.push_back(Node(i, nums, None, None))
        return linkedList

    def mix(self, linkedList: LinkedList):
        for id in range(self.cycle):
            linkedList.pivot(id)

    def decrypt(self, linkedList: LinkedList):
        zero = linkedList.find(self.zeroid)
        result = [linkedList.seek(zero, pos) for pos in [1000, 2000, 3000]]
        return sum(linkedList.nums[node.id] for node in result if node is not None)

    def Part1(self):
        linkedList = self.buildLinkedList(self.nums)
        self.mix(linkedList)
        print(f'Part 1: {self.decrypt(linkedList)}')

    def Part2(self):
        key = 811589153
        linkedList = self.buildLinkedList([num*key for num in self.nums])
        for i in range(10):
            self.mix(linkedList)
            print(f'Part 2: process {i+1}/10')
        print(f'Part 2: {self.decrypt(linkedList)}')


if __name__ == '__main__':
    day20 = GrovePositioningSystem()
    day20.Part1()
    day20.Part2()

# Python time complexity 5k * 5k ~ 1s
