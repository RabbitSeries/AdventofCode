package Day10;

import java.io.*;
import java.time.Clock;
import java.util.*;
import java.util.AbstractMap.SimpleEntry;
import java.util.Map.Entry;
import java.util.stream.*;
import Day10.PipeModel.*;
import DataModel.Point2D;

public class CharacterLoop {
    List<List<Character>> PipeMap;
    int rows, cols;
    Point2D StartPos;

    void readFile() throws IOException {
        PipeMap = new ArrayList<>();
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        String buf;
        while ((buf = input.readLine()) != null) {
            if (buf.indexOf("S") != -1) {
                StartPos = new Point2D(PipeMap.size(), buf.indexOf("S"));
            }
            PipeMap.add(buf.chars().mapToObj(l -> (char) l).collect(Collectors.toList()));
        }
        rows = PipeMap.size();
        cols = PipeMap.get(0).size();
        input.close();
    }

    void PrintMap() {
        PipeMap.forEach(line -> {
            line.forEach(c -> {
                System.out.printf("%c", !c.equals('*') ? ' ' : c);
            });
            System.out.println();
        });
    }

    void PrintPath(List<Point2D> pathList) {
        List<List<Character>> printMap = new ArrayList<>();
        for (List<Character> row : PipeMap) {
            // Deep copy instead of shallow copy.
            printMap.add(new ArrayList<>(row));
        }
        for (Point2D p : pathList) {
            printMap.get(p.getKey()).set(p.getValue(), '*');
        }
        printMap.get(StartPos.getKey()).set(StartPos.getValue(), 'S');
        printMap.forEach(line -> {
            line.forEach(c -> {
                System.out.printf("%c", c.equals('S') ? 'S' : c.equals('*') ? '*' : ' ');
            });
            System.out.println();
        });
    }

    Character getPipe(Point2D curPos) {
        return PipeMap.get(curPos.getKey()).get(curPos.getValue());
    }

    boolean isValid(Point2D curPos) {
        return Point2D.isValid(rows, cols, curPos) && PipeMap.get(curPos.getKey()).get(curPos.getValue()) != '.';
    }

    static ClockOrder PathClockOrder = null;

    List<Point2D> Solution1() throws IOException, InterruptedException {
        readFile();
        Queue<Entry<Entry<Point2D, Integer>, ClockOrder>> q = new LinkedList<>();
        HashMap<Entry<Point2D, Integer>, Boolean> visited = new HashMap<>();

        for (int i = 0; i < 4; i++) {
            Point2D nextPos = Point2D.getNextPosition(StartPos, i);

            if (isValid(nextPos) && PipeModel.NextPipeDirection.get(getPipe(nextPos)).getOrDefault(i, -1) != -1) {
                // Start of path, ClockOrder is null;
                q.add(new SimpleEntry<>(new SimpleEntry<>(nextPos, i), null));
                visited.put(new AbstractMap.SimpleEntry<>(nextPos, i), true);
            }
        }

        int loopLen = 1;
        // BFS
        while (!q.isEmpty()) {
            var curLevelList = q.stream().collect(Collectors.toList());
            q.clear();
            for (var front : curLevelList) {
                // Level info
                Point2D curPos = front.getKey().getKey();
                int curFace = front.getKey().getValue();
                ClockOrder curOrder = front.getValue();

                // Path process
                if (curPos.pathList == null) {
                    curPos.pathList = new ArrayList<>();
                }
                curPos.pathList.add(curPos.extractPosPair());

                // Destination process
                if (curPos.equals(StartPos)) {
                    System.out.println("Solution 1: " + loopLen / 2);
                    PathClockOrder = curOrder;
                    return new ArrayList<>(
                            curPos.pathList.stream().map(entry -> new Point2D(entry.getKey(), entry.getValue()))
                                    .collect(Collectors.toList()));
                }

                // Construct nextPos info
                int nextFace = PipeModel.NextPipeDirection.get(getPipe(curPos)).getOrDefault(curFace, -1);
                if (nextFace != -1) {
                    Point2D nextPos = Point2D.getNextPosition(curPos, nextFace);

                    // Enqueue
                    if (isValid(nextPos) && !visited.getOrDefault(new SimpleEntry<>(nextPos, nextFace), false)) {
                        ClockOrder nextOrder = curOrder;

                        // Once confirmed path clock order, keep it thereafter
                        if (nextOrder == null) {
                            Map<Integer, ClockOrder> PreFaceQuery = PipeModel.ClockOrderQuery.getOrDefault(
                                    getPipe(nextPos),
                                    null);
                            if (PreFaceQuery != null) {
                                nextOrder = PreFaceQuery.getOrDefault(curFace, null);
                            }
                        }

                        nextPos.pathList = new ArrayList<>(curPos.pathList);
                        q.add(new SimpleEntry<>(new SimpleEntry<>(nextPos, nextFace), nextOrder));
                        visited.put(new SimpleEntry<>(nextPos, nextFace), true);
                    }
                }
            }
            loopLen++;
        }
        return null;
    }

    int innerFlood(Point2D s, HashMap<Point2D, Boolean> visited) {
        Queue<Point2D> q = new LinkedList<>();
        q.add(s);
        boolean innerBlock = true;
        int cellCnt = 0;
        while (!q.isEmpty()) {
            Point2D curPos = q.poll();
            cellCnt++;
            for (int i = 0; i < 4; i++) {
                Point2D nextPos = Point2D.getNextPosition(curPos, i);
                if (Point2D.isValid(rows, cols, nextPos)) {
                    if (!visited.containsKey(nextPos)) {
                        visited.put(nextPos, true);
                        q.add(nextPos);
                    }
                } else {
                    innerBlock = false;
                }
            }
        }
        // if (innerBlock) {
        //     return cellCnt;
        // }
        return cellCnt;
        // throw new ExceptionInInitializerError(
        // "Unexpeted, the algo should seek path's adjacent point on path's inner
        // direction");
    }

    void Solution2(List<Point2D> pathList) {
        PrintPath(pathList);
        // HashMap<Character, Map<Integer, List<Integer>>> QueryModel = PathClockOrder
        // .equals(PipeModel.ClockOrder.ClockWise) ? PipeModel.ClockwiseQuery :
        // PipeModel.CounterclockwiseQuery;
        // Get the smaller one
        HashMap<Character, Map<Integer, List<Integer>>> QueryModel = PipeModel.ClockwiseQuery;
        QueryModel = PipeModel.CounterclockwiseQuery;
        HashMap<Point2D, Boolean> visited = new HashMap<>();
        for (Point2D curPos : pathList) {
            visited.putIfAbsent(curPos, true);
        }
        int res = 0;
        Point2D prePos = null;
        for (Point2D curPos : pathList) {
            if (curPos.equals(StartPos)) {
                System.out.println("Solution 2: " + res);
                return;
            }
            if (prePos != null) {
                for (var adjPos : getInnerAdjacents(curPos, prePos, QueryModel, visited)) {

                    // int t = innerFlood(adjPos, visited);
                    // System.out.println("Flood this valid point, got " + t);
                    res += innerFlood(adjPos, visited);
                }
            }
            prePos = curPos;
        }
    }

    List<Point2D> getInnerAdjacents(Point2D curPos, Point2D prePos,
            HashMap<Character, Map<Integer, List<Integer>>> QueryModel, HashMap<Point2D, Boolean> visited) {
        int inDirection = -1;
        int dx = curPos.getKey() - prePos.getKey(), dy = curPos.getValue() - prePos.getValue();
        if (dx == 0) {
            inDirection = (dy == 1 ? 2 : 3);
        } else {
            inDirection = (dx == 1 ? 0 : 1);
        }
        // System.out.println("Attempt " + "enters " + getPipe(curPos) + " from " +
        // getPipe(prePos) + " at direction " + inDirection + " on clockorder: " +
        // PathClockOrder);
        List<Point2D> PointList = new ArrayList<>();
        for (int v : QueryModel.get(getPipe(curPos)).get(inDirection)) {
            Point2D nextPos = Point2D.getNextPosition(curPos, v);
            if (Point2D.isValid(rows, cols, nextPos) && !visited.containsKey(nextPos)) {
                visited.put(nextPos, true);
                PointList.add(nextPos);
            }
        }
        // System.out.println("Have " + PointList.size() + " valid adjacent inner
        // points");
        return PointList;
    }

    public static void main(String[] args) throws IOException, InterruptedException {
        CharacterLoop Day10 = new CharacterLoop();
        List<Point2D> pathList = Day10.Solution1();
        Day10.Solution2(pathList);
    }
}
