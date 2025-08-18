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
public class LongestHike implements SolutionBase {
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

    final Map<Integer, Character> DIRECTION = new HashMap<>(Map.of(0, '^', 1, '>', 2, 'v', 3, '<'));

    boolean isValid(Point2D pos) {
        return pos.first >= 0 && pos.second >= 0 && pos.first < rows && pos.second < cols;
    }

    Point2D getNext(Point2D pos, int direction) {
        return new Point2D(pos.first + dx[direction], pos.second + dy[direction]);
    }

    class Path {
        Path(Point2D s, Point2D e, int l) {
            StartPos = s;
            EndPos = e;
            PathLen = l;
            visited.add(e);
        }

        @Override
        public int hashCode() {
            return Objects.hash(StartPos, EndPos, PathLen);
        }

        public Point2D StartPos, EndPos;

        public int GetPathLen() {
            return PathLen;
        };

        Set<Point2D> visited = new HashSet<>();

        int PathLen;
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
            Path curPath = q.poll();
            Point2D curEndPos = curPath.EndPos;
            int curBlock = IceIsland.get(curEndPos.first).get(curEndPos.second);
            if ((curBlock != PATH && curPath.PathLen > 1) || curEndPos.equals(Dest)) {
                if (!curEndPos.equals(Dest)) {
                    curEndPos = curPath.EndPos = getNext(curEndPos, curBlock);
                    curPath.PathLen++;
                    visited.add(curEndPos);// Mark the generated position as visited
                }
                OutPath.computeIfAbsent(curPath.StartPos, pos -> new HashSet<>()).add(curPath);// Add curPath ->StartPos - HashSet();
                InDgree.put(curEndPos, InDgree.getOrDefault(curEndPos, 0) + 1);// Increase EndPos's InDgree;
                OutPath.putIfAbsent(curEndPos, new HashSet<>());
                if (!curEndPos.equals(Dest)) {// Not readching destination yet, continue to search more Path.
                    q.add(new Path(curPath.EndPos, curPath.EndPos, 0));
                }
                continue;
            }
            for (int i = 0; i < 4; i++) {
                Point2D nextPos = getNext(curEndPos, i);
                if (isValid(nextPos) && (GetBlock(nextPos) == PATH || GetBlock(nextPos) == i) && !visited.contains(nextPos)) {
                    visited.add(nextPos);// This can also be processed before for-loop search, as there is no two path with intersection before the joint.
                    q.add(new Path(curPath.StartPos, nextPos, curPath.PathLen + 1));
                }
            }
        }
    }

    public void Solution1(BufferedReader input) throws Exception {
        readFile(input);
        BuildDirectedNetwork();
        HashMap<Point2D, Integer> Dist = new HashMap<>();
        Queue<Point2D> topoQ = new LinkedList<>(List.of(Start));
        while (!topoQ.isEmpty()) {
            Point2D curPos = topoQ.poll();
            for (Path p : OutPath.get(curPos)) {
                UndidrectedNetwork.computeIfAbsent(curPos, pos -> new HashSet<>()).add(p);
                UndidrectedNetwork.computeIfAbsent(p.EndPos, pos -> new HashSet<>()).add(new Path(p.EndPos, curPos, p.PathLen));
                // backPath.visited.addAll(p.visited);
                InDgree.put(p.EndPos, InDgree.get(p.EndPos) - 1);
                Dist.put(p.EndPos, Math.max(Dist.getOrDefault(p.EndPos, 0), Dist.getOrDefault(p.StartPos, 0) + p.PathLen));
                if (InDgree.get(p.EndPos).equals(0)) {
                    topoQ.add(p.EndPos);
                }
            }
        }
        System.out.println("Solution 1: " + Dist.get(Dest));
    }

    // int DFSCnt = 0;
    int DFS(Point2D curPos, Set<Point2D> visited, int curPathLen) {
        // DFSCnt++;
        int res = curPathLen;
        for (Path p : UndidrectedNetwork.get(curPos)) {
            Point2D nextEnd = p.EndPos;
            if (!visited.contains(nextEnd)) {
                visited.add(nextEnd);
                res = Math.max(res, DFS(nextEnd, visited, curPathLen + p.PathLen));
                visited.remove(nextEnd);
            }
        }
        return res;
    }

    public void Solution2(BufferedReader input) throws Exception {
        System.out.println("Solution 2: " + DFS(Start, new HashSet<>(Set.of(Start)), 0));
        // System.out.println("DFS CNT: " + DFSCnt); // 30580294
        // // Equivalent BFS, but the Queue space will be DFS CNT size, which causes numerous rellocation cost.
        // Queue<Path> q = new LinkedList<>(List.of(new Path(Start, Start, 0)));
        // // HashMap<Point2D, Integer> Dist = new HashMap<>(Map.of(Start, 0));
        // int res = 0;
        // while (!q.isEmpty()) {
        //     Path curPath = q.poll();
        //     int curPathLen = curPath.GetPathLen();
        //     if (curPath.EndPos.equals(Dest)) {
        //         res = Math.max(curPathLen, res);
        //         // PrintPath(curPath.visited);
        //         continue;
        //     }
        //     for (Path p : UndidrectedNetwork.get(curPath.EndPos)) {
        //         Point2D nextEnd = p.EndPos;
        //         if (!curPath.visited.contains(nextEnd)) {
        //             int nextPathLen = curPathLen + p.PathLen;
        //             Path nextPath = new Path(curPath.StartPos, p.EndPos, nextPathLen);
        //             nextPath.visited.addAll(curPath.visited);
        //             q.add(nextPath);
        //         }
        //     }
        // }
        // System.out.println("BFS res: " + res);
    }

    public static void main(String[] args) throws Exception {
        LongestHike Day23 = new LongestHike();
        Day23.Solution1(new BufferedReader(new FileReader("Day23/input.txt")));
        Day23.Solution2(new BufferedReader(new FileReader("Day23/input.txt")));
    }
}
