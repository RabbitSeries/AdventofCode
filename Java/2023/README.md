# 2023 Summary

This year is full of mathematics.

## Day5-6

Notice this api: Stream.takeWhile(Objects::nonNull)[.forEach() | .toList()];

## Day7

Notice this api: Stream.collect(Collectors.groupingBy(l->l, Collectors.counting()))

## Day20

I don't know what circuit [this](./main/year2023/Day20/mermaid.md) is, but it seems that the circuit can be seperated
into four parts then LCM to the final result using sort of system chaos index.

## Day21

Hard to explain the lagrange solution. See [Visualize](./main/year2023/Day21/Visualize.ipynb) for BFS solution and math
calculation.

## Day22

Amazing! I finished both Solution1 and Solution2 in one shot!

## Day23

This problem requires some data pattern examine. Thanks to the well-designed DAG, both parts have a brute force
solution. But for Solution 1, topological sort DP solution will be NP-complete.

For part 2, BFS-based brute force solution will enlarge the Queue size to the same scale of DFS traversal times, which
requires much more time for space reallocation comparing to DFS stack reallocation.

## Day24

Linear algebra

## Day25

Karger's algorithm.

## Regex diff

|  Lang  | Regex URL                                                                                                                                     |
|:------:|-----------------------------------------------------------------------------------------------------------------------------------------------|
|  C++   | [Modified ECMAScript from C++](https://en.cppreference.com/w/cpp/regex/ecmascript)                                                            |
|  Java  | [Java EE Summary of regular-expression constructs](https://docs.oracle.com/en/java/javase/22/docs/api/java.base/java/util/regex/Pattern.html) |
| Python | [Python Regular expression operations](https://docs.python.org/3/library/re.html)                                                             |
