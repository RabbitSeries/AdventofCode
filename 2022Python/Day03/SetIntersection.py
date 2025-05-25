def priority(c: str) -> int:
    return ord(c) - ord('A') + 27 if c.isupper() else ord(c) - ord('a') + 1


def intersection(iterable: list[str]) -> set[str]:
    return set(iterable[0]).intersection(*iterable[1:])


with open("input.txt") as f:
    lines = [line.strip() for line in f.read().split()]
part1 = sum(
    sum(map(priority, {c for c in line.strip()[:len(line) // 2]} & {c for c in line.strip()[len(line) // 2:]}))
    for line in lines
)
part2 = sum(
    sum(map(priority, intersection(lines[i:i + 3])))
    for i in range(0, len(lines), 3)
)
print("Part 1:", part1)
print("Part 2:", part2)
