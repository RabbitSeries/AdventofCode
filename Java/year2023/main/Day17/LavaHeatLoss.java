package Day17;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

import JavaDataModel.*;

public class LavaHeatLoss implements SolutionBase2023 {
    void readFile() throws IOException {
        LavaMap = new ArrayList<>();
        String buf;
        BufferedReader input = new BufferedReader(new FileReader("Day17/input.txt"));
        while ((buf = input.readLine()) != null) {
            LavaMap.add(buf.chars().mapToObj(c -> c - '0').toList());
        }
        input.close();
    }

    final int[] dx = new int[] {
            0, 1, 0, -1
    };

    final int[] dy = new int[] {
            1, 0, -1, 0
    };

    void printLossMap(List<List<List<Integer>>> LossMap) {
        List<List<Character>> Canvas = LavaMap.stream().map(l -> (List<Character>) new ArrayList<Character>(l.stream().map(a -> (char) (a + '0')).toList())).toList();
        for (int row = 0; row < LavaMap.size(); row++) {
            for (int col = 0; col < LavaMap.get(0).size(); col++) {
                for (int i = 0; i < 4; i++) {
                    if (LossMap.get(row).get(col).get(i) != Integer.MAX_VALUE) {
                        Canvas.get(row).set(col, '*');
                    }
                }
            }
        }
        Canvas.forEach(line -> {
            line.forEach(c -> System.out.print(c));
            System.out.println();
        });
    }

    void printPathList(HashSet<Pair<Integer, Integer>> PathList) {
        List<List<Character>> Canvas = LavaMap.stream().map(l -> (List<Character>) new ArrayList<Character>(l.stream().map(a -> (char) (a + '0')).toList())).toList();
        for (var pos : PathList) {
            Canvas.get(pos.first).set(pos.second, '*');
        }
        Canvas.forEach(line -> {
            line.forEach(c -> System.out.print(c));
            System.out.println();
        });
    }

    int Dijkstra(Point2D startPos, Point2D endPos, int lowerBound, int upperBound) {
        int rows = LavaMap.size(), cols = LavaMap.get(0).size();
        PriorityQueue<PosInfo> pq = new PriorityQueue<>((p1, p2) -> p1.first - p2.first);
        List<List<HashMap<String, Integer>>> LossMap = Collections.nCopies(rows, Collections.nCopies(cols, new HashMap<String, Integer>()))
                .stream().map(row -> row
                        .stream().map(loss -> new HashMap<>(loss))
                        .toList())
                .toList();
        for (int i = 0; i < 4; i++) {
            // LossMap.get(startPos.first).get(startPos.second).set(i, 0);
            Point2D nextPos = new Point2D(startPos.first + dx[i], startPos.second + dy[i]);
            if (Point2D.isValid(rows, cols, nextPos)) {
                LossMap.get(nextPos.first).get(nextPos.second).put(1 + "," + i, LavaMap.get(nextPos.first).get(nextPos.second));
                pq.add(new PosInfo(LavaMap.get(nextPos.first).get(nextPos.second), 1, i, nextPos));
            }
        }
        int minLoss = Integer.MAX_VALUE;
        while (!pq.isEmpty()) {
            // System.out.println(pq.size());
            var curPosInfo = pq.poll();
            int curLoss = curPosInfo.first, curStraightCnt = curPosInfo.second.first, curDirection = curPosInfo.second.second.first;
            Point2D curPos = curPosInfo.second.second.second;
            if (curLoss > LossMap.get(curPos.first).get(curPos.second).get(curStraightCnt + "," + curDirection)) {
                continue;
            }
            // curPos.pathList.add(new Point2D(curPos.first, curPos.second));
            // printLossMap(LavaMap, LossMap);
            if (curPos.equals(endPos) && curLoss <= minLoss && curStraightCnt >= lowerBound) {
                minLoss = curLoss;
                // printPathList(LavaMap, curPos.pathList);
                // if(!isFindAllPath)
                break;
            } else if (minLoss != Integer.MAX_VALUE && curLoss != minLoss) {
                break;
            }
            for (int i = 0; i < 4; i++) {
                if (i == 2)
                    continue;
                int nextDirection = (curDirection + i) % 4, nextStraightCnt = (i == 0 ? curStraightCnt + 1 : 1);
                if (nextStraightCnt <= upperBound) {
                    Point2D nextPos = new Point2D(curPos.first + dx[nextDirection], curPos.second + dy[nextDirection]);
                    if (Point2D.isValid(rows, cols, nextPos)) {
                        int nextLoss = curLoss + LavaMap.get(nextPos.first).get(nextPos.second);
                        var nextPosHashMap = LossMap.get(nextPos.first).get(nextPos.second);
                        String queryStr = nextStraightCnt + "," + nextDirection;
                        if (!nextPosHashMap.containsKey(queryStr) || nextLoss < nextPosHashMap.get(queryStr)) {
                            // nextPos.pathList = new HashSet<>(curPos.pathList);
                            LossMap.get(nextPos.first).get(nextPos.second).put(queryStr, nextLoss);
                            pq.add(new PosInfo(nextLoss, nextStraightCnt, nextDirection, nextPos));
                        }
                    }
                }
                if (curStraightCnt < lowerBound)
                    break;
            }
        }
        return minLoss;
    }

    List<List<Integer>> LavaMap;

    public void Solution1() throws IOException {
        readFile();
        int rows = LavaMap.size(), cols = LavaMap.get(0).size();
        Point2D startPos = new Point2D(0, 0), endPos = new Point2D(rows - 1, cols - 1);
        System.out.println("Solution 1: " + Dijkstra(startPos, endPos, 1, 3));
        return;
    }

    public void Solution2() throws IOException {
        int rows = LavaMap.size(), cols = LavaMap.get(0).size();
        Point2D startPos = new Point2D(0, 0), endPos = new Point2D(rows - 1, cols - 1);
        System.out.println("Solution 2: " + Dijkstra(startPos, endPos, 4, 10));
        return;
    }

    class PosInfo extends Pair<Integer, Pair<Integer, Pair<Integer, Point2D>>> {
        PosInfo(Integer curLoss, Integer curStraightCnt, Integer curDirection, Point2D curPos) {
            super(curLoss, new Pair<>(curStraightCnt, new Pair<>(curDirection, curPos)));
        }
    }

    public static void main(String[] args) throws IOException {
        var now = System.currentTimeMillis();
        LavaHeatLoss Day17 = new LavaHeatLoss();
        Day17.Solution1();
        Day17.Solution2();
        System.out.println("Time usage: " + (System.currentTimeMillis() - now) + " ms");
    }
}
