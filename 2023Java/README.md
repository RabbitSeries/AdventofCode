# 2023 AoC Solution Repo

## Day19

~~Suprisingly, Java solution is 1/4 faster than cpp solution.~~(on windows)

## Day20

I don't know what circuit [this](./Day20/mermaid.md) is, but it seems that the circuit can be seperated into four parts then LCM to the final result using sort of system chaos index.

## Day21

Hard to exlain the langrange solution. See [Visualiza](./Day21/Visualize.ipynb) for BFS solution and math calculation.

## Day22

Amazing! I finished both Solution1 and Solution2 in one shot!

## Day23

This problem requires some data pattern examine. Thanks to the well designed DAG, both parts have a brute force solution. But for Solution 1, topological sort DP solution will be NP-complete.

For part 2, BFS-based brute force solution will enlarge the Queue size to the same scale of DFS traversal times, which requires much time for space reallocation comparing to DFS stack reallocation.

This difference also remind the my [AVL tree implementation](https://github.com/RabbitSeries/Algorithms/blob/main/Datastruct/Tree/AVL_Iterative.hpp), the manually maintained non-DFS stack is slower than simple DFS. This is because the height of a AVL is only $log_{2}n$, so DFS won't cause stack overflow problem. Under this case, stack maintainance can be handled by the OS process controll block for context reservation and restore of which the time cost is rather low comparing to the dynamically rellocated stack.

## Regex diff

Lang    | Regex URL
:---:   | --------
C++     | [Modified ECMAScript from C++](https://en.cppreference.com/w/cpp/regex/ecmascript)
Java    | [Java EE Summary of regular-expression constructs](https://docs.oracle.com/en/java/javase/22/docs/api/java.base/java/util/regex/Pattern.html)
Python  | [Python Regular expression operations](https://docs.python.org/3/library/re.html)
