package year2023.Day23;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Queue;
import java.util.Set;

import JavaDataModel.*;

@AoCSolution()
public class LongestHike implements ISolution {
    final int PATH = -2;

    final int FOREST = -1;

    Point2D Start, Dest;

    final int dx[] = new int[] {
            -1, 0, 1, 0
    };

    final int dy[] = new int[] {
            0, 1, 0, -1
    };

    int rows, cols;

    List<List<Integer>> IceIsland = new ArrayList<>();

    void readFile(BufferedReader input) throws Exception {
        IceIsland = input.lines().map(line -> line.trim().chars().mapToObj(c -> {
            switch (c) {
                case (int) '.':
                    return PATH;
                case (int) '#':
                    return FOREST;
                case '^':
                    return 0;
                case '>':
                    return 1;
                case 'v':
                    return 2;
                case '<':
                    return 3;
                default:
                    return PATH;
            }
        }).toList()).toList();
        Start = new Point2D(0, 1);
        Dest = new Point2D(IceIsland.size() - 1, IceIsland.get(0).size() - 2);
        rows = IceIsland.size();
        cols = IceIsland.get(0).size();
    }

    boolean isValid(Point2D pos) {
        return pos.first >= 0 && pos.second >= 0 && pos.first < rows && pos.second < cols;
    }

    Point2D getNext(Point2D pos, int direction) {
        return new Point2D(pos.first + dx[direction], pos.second + dy[direction]);
    }

    class Path {
        Path(Point2D s, Point2D e, int l) {
            start = s;
            end = e;
            len = l;
            visited.add(e);
        }

        @Override
        public int hashCode() {
            return Objects.hash(start, end, len);
        }

        public Point2D start, end;

        public int GetPathLen() {
            return len;
        };

        Set<Point2D> visited = new HashSet<>();

        int len;
    }

    int GetBlock(Point2D pos) {
        return IceIsland.get(pos.first).get(pos.second);
    }

    Map<Point2D, Set<Path>> OutPath = new HashMap<>(), UndidrectedNetwork = new HashMap<>();

    Map<Point2D, Integer> InDgree = new HashMap<>();

    void BuildDirectedNetwork() throws Exception {
        Queue<Path> q = new LinkedList<>(List.of(new Path(Start, Start, 0)));
        Set<Point2D> visited = new HashSet<>(Set.of(Start));
        while (!q.isEmpty()) {
            Path path = q.poll();
            int curLen = path.len;
            Point2D curStart = path.start, curEnd = path.end;
            for (int i = 0; i < 4; i++) {
                Point2D nextPos = getNext(curEnd, i);
                // Leave GetBlock(nextPos) == FOREST || != i. Leave != i because it is anthoer path's endian, don't mark them as visited
                if (isValid(nextPos) && (GetBlock(nextPos) == PATH || GetBlock(nextPos) == i) && !visited.contains(nextPos)) {
                    visited.add(nextPos);// This can also be processed before for-loop search, as there is no two path with intersection before the joint.
                    if (GetBlock(nextPos) == PATH || curLen == 0) {
                        if (nextPos.equals(Dest)) {
                            OutPath.computeIfAbsent(curStart, pos -> new HashSet<>()).add(new Path(curStart, nextPos, curLen + 1));// Add curPath ->StartPos - HashSet();
                            InDgree.compute(nextPos, (k, v) -> v == null ? 1 : v + 1);
                        } else {
                            q.add(new Path(curStart, nextPos, curLen + 1));
                        }
                    } else if (GetBlock(nextPos) == i && curLen > 0) {
                        Point2D joint = getNext(nextPos, i);
                        visited.add(joint);// Mark the generated position as visited
                        OutPath.computeIfAbsent(curStart, pos -> new HashSet<>()).add(new Path(curStart, joint, curLen + 2));// Add curPath ->StartPos - HashSet();
                        InDgree.compute(joint, (k, v) -> v == null ? 1 : v + 1);
                        q.add(new Path(joint, joint, 0));
                    }
                }
            }
        }
    }

    public void Solution1(BufferedReader input) throws Exception {
        readFile(input);
        BuildDirectedNetwork();
        Map<Point2D, Integer> Dist = new HashMap<>(Map.of(Start, 0));
        Queue<Point2D> topoQ = new LinkedList<>(List.of(Start));
        while (!topoQ.isEmpty()) {
            Point2D curPos = topoQ.poll();
            for (Path p : OutPath.getOrDefault(curPos, Set.of())) {
                UndidrectedNetwork.computeIfAbsent(curPos, pos -> new HashSet<>()).add(p);
                UndidrectedNetwork.computeIfAbsent(p.end, pos -> new HashSet<>()).add(new Path(p.end, curPos, p.len));
                // backPath.visited.addAll(p.visited);
                Dist.compute(p.end, (k, v) -> v == null ? Dist.get(p.start) + p.len : Math.max(Dist.get(p.start) + p.len, v));
                InDgree.compute(p.end, (k, v) -> {
                    if (v - 1 == 0) {
                        topoQ.add(p.end);
                    }
                    return v - 1;
                });
            }
        }
        System.out.println("Solution 1: " + Dist.get(Dest));
    }

    // int DFSCnt = 0;
    int DFS(Point2D curPos, Set<Point2D> visited, int curPathLen) {
        // DFSCnt++;
        int res = curPathLen;
        for (Path p : UndidrectedNetwork.get(curPos)) {
            Point2D nextEnd = p.end;
            if (!visited.contains(nextEnd)) {
                visited.add(nextEnd);
                res = Math.max(res, DFS(nextEnd, visited, curPathLen + p.len));
                visited.remove(nextEnd);
            }
        }
        return res;
    }

    int BFS() {
        // Equivalent BFS, but the Queue space will be DFS CNT size, which causes numerous space (not Garbage Colleted) at the same time.
        Queue<Path> q = new LinkedList<>(List.of(new Path(Start, Start, 0)));
        int res = 0;
        while (!q.isEmpty()) {
            Path curPath = q.poll();
            int curPathLen = curPath.GetPathLen();
            if (curPath.end.equals(Dest)) {
                res = Math.max(curPathLen, res);
                continue;
            }
            for (Path p : UndidrectedNetwork.get(curPath.end)) {
                Point2D nextEnd = p.end;
                if (!curPath.visited.contains(nextEnd)) {
                    int nextPathLen = curPathLen + p.len;
                    Path nextPath = new Path(curPath.start, p.end, nextPathLen);
                    nextPath.visited.addAll(curPath.visited);
                    q.add(nextPath);
                }
            }
        }
        System.out.println("BFS res: " + res);
        return res;
    }

    public void Solution2(BufferedReader input) throws Exception {
        System.out.println("Solution 2: " + DFS(Start, new HashSet<>(Set.of(Start)), 0));
        // System.out.println("DFS CNT: " + DFSCnt); // 30580294
    }

    public static void main(String[] args) throws Exception {
        LongestHike Day23 = new LongestHike();
        Day23.Solution1(new BufferedReader(new FileReader("Day23/input.txt")));
        Day23.Solution2(new BufferedReader(new FileReader("Day23/input.txt")));
    }
}
