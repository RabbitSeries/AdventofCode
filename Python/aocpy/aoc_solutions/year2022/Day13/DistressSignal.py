from __future__ import annotations
import re

from aocpy.aoc_libs.utils.ISolution import ISolution


class Packet:
    def __init__(self, packets: list[Packet] | int) -> None:
        self.packets = packets

    def __repr__(self) -> str:
        # return f"Packet({self.packets})"
        return f"{self.packets}".replace(" ", "")

    def __lt__(self, rhs) -> bool:
        return self.compareTo(rhs) < 0

    def compareTo(self, rhs: Packet) -> int:
        if type(self.packets) is int and type(rhs.packets) is int:
            return self.packets - rhs.packets
        if type(self.packets) is list and type(rhs.packets) is list:
            for p, q in zip(self.packets, rhs.packets):
                cmp = p.compareTo(q)
                if cmp != 0:
                    return cmp
            return len(self.packets) - len(rhs.packets)
        return Packet([self]).compareTo(rhs) if type(self.packets) is int else self.compareTo(Packet([rhs]))


class Injection(Packet):
    def __init__(self, packets: list[Packet] | int, id: int) -> None:
        super().__init__(packets)
        self.id = id


class DistressSignal(ISolution):
    @staticmethod
    def ConsumeNumber(s: str):
        res: int | None = None
        if match := re.compile(r"^\d+").match(s):
            resStr = match.group()
            res = int(resStr)
            s = s[len(resStr):]
            if s and s[0] == ",":  # Commit char
                s = s[1:]
        return (res, s)

    @staticmethod
    def ConsumeList(s: str):
        stack = []
        res: str | None = None
        for i, c in enumerate(s, 0):
            if c == "[":
                stack.append(i)
            elif c == "]":
                if len(stack) > 1:
                    stack.pop()
                elif len(stack) == 1:
                    res = s[stack[0]:i + 1]  # "[*]"
                    s = s[i + 1:]
                    if s and s[0] == ",":  # Commit char
                        s = s[1:]
                    break
        return (res, s)

    def Parse(self, s: str) -> Packet | None:
        resList: list[Packet] = []
        if not s[0] == "[" and not s[-1] == "]":
            return None
        s = s[1:-1]  # [*] => *
        while len(s) > 0:
            if s[0].isdigit():
                num, s = self.ConsumeNumber(s)
                if num is not None:
                    resList.append(Packet(num))
                else:
                    raise RuntimeError("Error consuming")
            else:
                packetStr, s = self.ConsumeList(s)
                if packetStr is not None and (packet := self.Parse(packetStr)):
                    resList.append(packet)
                else:
                    raise RuntimeError("Error consuming")
        return Packet(resList)

    def Part1(self, f):
        self.PacketPairs = [tuple([self.Parse(packet) for packet in Pair.splitlines()]) for Pair in f.read().split("\n\n")]
        print(f"Part 1: {sum([i for i, (a, b) in enumerate(self.PacketPairs, 1) if a and b and a.compareTo(b) <= 0])}")

    def Part2(self, f):
        if (a := self.Parse("[[2]]")) and (b := self.Parse("[[6]]")):
            a, b = Injection(a.packets, 0), Injection(b.packets, 1)
            FlatMap = [a, b] + [packet for pair in self.PacketPairs for packet in pair if packet]
            FlatMap = sorted(FlatMap)
            decoder = [0, 0]
            for i, e in enumerate(FlatMap, 1):
                if type(e) is Injection:
                    decoder[e.id] = i
            print(f"Part 2: {decoder[0] * decoder[1]}")


if __name__ == "__main__":
    solution = DistressSignal()
    with open("Day13/input.txt") as f:
        solution.Part1(f)
    with open("Day13/input.txt") as f:
        solution.Part2(f)
