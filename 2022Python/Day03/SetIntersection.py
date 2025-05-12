from typing import List, Set
with open("input.txt") as f:
    def priority(c: str) -> int:
        return ord(c) - ord('A') + 27 if c.isupper() else ord(c) - ord('a') + 1
    lines = [line.strip() for line in f.read().split()]
    part1 = sum(
        sum(map(priority, {c for c in line.strip()[:len(line) // 2]}
            & {c for c in line.strip()[len(line) // 2:]}))
        for line in lines
    )

    def intersection(iterable: List[List[str]]) -> Set[str]:
        return set(iterable[0]).intersection(*iterable[1:])

    part2 = sum(
        sum(map(priority, intersection(lines[i:i + 3])))
        for i in range(0, len(lines), 3)
    )
    print(part1)
    print(part2)
