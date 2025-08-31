import re

from aocpy.aoc_libs.utils.ISolution import ISolution


class BeaconExclusionZone(ISolution):
    def merge_intervals(self, intervals: list[tuple[int, int]]) -> list[tuple[int, int]]:
        if not len(intervals):
            return []
        intervals = sorted(intervals)
        merged: list[tuple[int, int]] = [intervals[0]]
        for itv in intervals[1:]:
            if itv[0] > merged[-1][1]:
                merged.append(itv)
            else:
                merged[-1] = (merged[-1][0], max(merged[-1][1], itv[1]))
        return merged

    def exclusions_at_alignment(self, alignment: int, exludeExsitingBeacon: bool = True):
        intervals: list[tuple[int, int]] = []
        for (sensor, beacon), m_dis in zip(self.data, self.distances):
            remain = m_dis - abs(alignment - sensor[1])
            if remain <= 0:
                continue
            itv = (sensor[0] - remain, sensor[0] + remain)
            if exludeExsitingBeacon:
                itv = itv if beacon[1] != alignment else (
                    itv[0] + 1, itv[1]) if beacon[0] == itv[0] else (itv[0], itv[1] - 1)
            if itv[0] <= itv[1]:
                intervals.append(itv)
        return self.merge_intervals(intervals)

    @staticmethod
    def intersect_itv(intervals: list[tuple[int, int]]) -> tuple[int, int] | None:
        if not len(intervals):
            return None
        res = intervals[0]
        for itv in intervals[1:]:
            res = (max(itv[0], res[0]), min(itv[1], res[1]))
            if res[0] > res[1]:
                return None
        return res

    @staticmethod
    def reverse_itv(intervals: list[tuple[int, int]], xRange: tuple[int, int]):
        reverse_itvs: list[tuple[int, int]] = []
        for itv in intervals:
            if itv[0] > xRange[0]:
                reverse_itvs.append((xRange[0], min(itv[0] - 1, xRange[1])))
            if itv[1] < xRange[1]:
                reverse_itvs.append((max(itv[1] + 1, xRange[0]), xRange[1]))
        return reverse_itvs

    def Part1(self, f):
        reg = re.compile(r"x=(-?\d+), y=(-?\d+)")
        self.data = ([(line[0], line[1]) for line in
                      map(lambda line: [(int(x), int(y)) for x, y in reg.findall(line)], f.read().splitlines())])
        self.distances = [abs(a[0] - b[0]) + abs(a[1] - b[1]) for a, b in self.data]  # Manhattan distance
        print(f"Part 1: {sum([itv[1] - itv[0] + 1 for itv in self.exclusions_at_alignment(2000000)], start=0)}")

    def Part2(self, f):
        xRange = (0, 4000000)
        for alignment in range(0, xRange[1] + 1):
            if (res := self.intersect_itv(self.reverse_itv(self.exclusions_at_alignment(alignment, False), xRange))) is not None:
                print(f"Part 2: {res[0] * xRange[1] + alignment}")
                break


if __name__ == "__main__":
    solution = BeaconExclusionZone()
    with open("Day15/input.txt") as f:
        solution.Part1(f)
    with open("Day15/input.txt") as f:
        solution.Part2(f)
