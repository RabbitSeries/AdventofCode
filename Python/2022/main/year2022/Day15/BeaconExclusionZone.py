import re
point2D = tuple[int, int]


def merge_intervals(intervals: list[point2D]) -> list[point2D]:
    if not len(intervals):
        return []
    intervals = sorted(intervals)
    merged: list[point2D] = [intervals[0]]
    for itv in intervals[1:]:
        if itv[0] > merged[-1][1]:
            merged.append(itv)
        else:
            merged[-1] = (merged[-1][0], max(merged[-1][1], itv[1]))
    return merged


def exclusions_at_alignment(data: list[tuple[point2D, point2D]], distances: list[int], alignment: int, exludeExsitingBeacon: bool = True):
    intervals: list[point2D] = []
    for (sensor, beacon), m_dis in zip(data, distances):
        remain = m_dis - abs(alignment - sensor[1])
        if remain <= 0:
            continue
        itv = (sensor[0] - remain, sensor[0] + remain)
        if exludeExsitingBeacon:
            itv = itv if beacon[1] != alignment else (itv[0] + 1, itv[1]) if beacon[0] == itv[0] else (itv[0], itv[1] - 1)
        if itv[0] <= itv[1]:
            intervals.append(itv)
    return merge_intervals(intervals)


def intersect_itv(intervals: list[point2D]) -> point2D | None:
    if not len(intervals):
        return None
    res = intervals[0]
    for itv in intervals[1:]:
        res = (max(itv[0], res[0]), min(itv[1], res[1]))
        if res[0] > res[1]:
            return None
    return res


def reverse_itv(intervals: list[point2D], xRange: point2D):
    reverse_itvs: list[point2D] = []
    for itv in intervals:
        if itv[0] > xRange[0]:
            reverse_itvs.append((xRange[0], min(itv[0] - 1, xRange[1])))
        if itv[1] < xRange[1]:
            reverse_itvs.append((max(itv[1] + 1, xRange[0]), xRange[1]))
    return reverse_itvs


def main():
    reg = re.compile(r"x=(-?\d+), y=(-?\d+)")
    with open("Day15/input.txt") as file:
        data = ([(line[0], line[1]) for line in map(lambda line: [(int(x), int(y))
                for x, y in reg.findall(line)], file.read().splitlines())])
    distances = [abs(a[0] - b[0]) + abs(a[1] - b[1]) for a, b in data]  # Manhattan distance
    print(f"Part 1: {sum([itv[1] - itv[0] + 1 for itv in exclusions_at_alignment(data, distances, 2000000)], start=0)}")

    xRange = (0, 4000000)
    for alignment in range(0, xRange[1] + 1):
        if (res := intersect_itv(reverse_itv(exclusions_at_alignment(data, distances, alignment, False), xRange))) is not None:
            print(f"Part 2: {res[0] * xRange[1] + alignment}")
            break


if __name__ == "__main__":
    main()
