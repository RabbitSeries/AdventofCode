from __future__ import annotations

from aocpy.aoc_libs.utils.ISolution import ISolution

# WTF? A simple linked list cost me so much time?


class Node:
    def __init__(self, id: int, nums: list[int], next: Node | None, pre: Node | None):
        self.id, self.nums, self.next, self.pre = id, nums, next if next else self, pre if pre else self

    def __repr__(self) -> str:
        return f'{self.nums[self.id]}'


class LinkedList:
    def __init__(self, nums: list[int]):
        self.head: Node | None = None
        self.nums = nums
        self.len = 0

    def __repr__(self) -> str:
        l = []
        p = self.head
        for _ in range(self.len):
            if p is not None:
                l.append({self.nums[p.id]: p.id})
                p = p.next
            else:
                break
        return f"{l}"

    def push_back(self, node: Node):
        if self.head is None:
            self.head = node
        else:
            tail = self.head.pre
            tail.next = node
            node.pre = tail
            node.next = self.head
            self.head.pre = node
        self.len = self.len + 1

    def pivot(self, id: int):
        if not self.head:
            raise ValueError('Empty link')

        offset = self.nums[id]
        if self.len == 1 or offset % (self.len - 1) == 0:  # Delete the node first
            return

        p = self.find(id)
        if p.id == self.head.id:
            self.head = p.next
        p.pre.next = p.next
        p.next.pre = p.pre
        self.len = self.len - 1

        q = self.seek(p, offset)
        self.insert(p, q, offset < 0)

    def insert(self, p: Node, q: Node, before: bool = True):
        p.pre = q.pre if before else q
        p.next = q if before else q.next
        p.pre.next = p
        p.next.pre = p
        self.len = self.len + 1

    def find(self, id: int):
        if not self.head:
            raise ValueError('Empty link')
        p = self.head
        for _ in range(self.len):
            if p.id == id:
                return p
            p = p.next
        raise ValueError(f'Id {id} not found')

    def seek(self, p: Node, offset: int):
        pos = abs(offset) % self.len
        for _ in range(pos):
            if offset > 0:
                p = p.next
            else:
                p = p.pre
        return p


class GrovePositioningSystem(ISolution):

    def buildLinkedList(self, nums: list[int]):
        link = LinkedList(nums)
        for i in range(self.cycle):
            link.push_back(Node(i, nums, None, None))
        return link

    def mix(self, linkedList: LinkedList):
        for id in range(self.cycle):
            linkedList.pivot(id)

    def decrypt(self, link: LinkedList):
        zero = link.find(self.zeroid)
        result = [link.seek(zero, pos) for pos in [1000, 2000, 3000]]
        return sum(link.nums[node.id] for node in result)

    def Part1(self, f):
        self.nums = [num for num in map(int, f.read().splitlines())]
        self.cycle = len(self.nums)
        self.zeroid = self.nums.index(0)
        linkedList = self.buildLinkedList(self.nums)
        self.mix(linkedList)
        print(f'Part 1: {self.decrypt(linkedList)}')

    def Part2(self, f):
        key = 811589153
        link = self.buildLinkedList([num*key for num in self.nums])
        for _ in range(10):
            self.mix(link)
        print(f'Part 2: {self.decrypt(link)}')


if __name__ == "__main__":
    solution = GrovePositioningSystem()
    with open("Day20/input.txt") as f:
        solution.Part1(f)
    with open("Day20/input.txt") as f:
        solution.Part2(f)


# Python time complexity 5k * 5k ~ 1s
