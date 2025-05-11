with open("input.txt") as f:
    groups = [sum(map(int, linGroup.split())) for linGroup in f.read().split("\n\n")]
print(max(groups))
print(sum(sorted(groups, reverse=True)[:3]))
