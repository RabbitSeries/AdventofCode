package Day18;

import java.io.*;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import JavaDataModel.Pair;
import JavaDataModel.Point2D;

public class LavaInterior {
    List<DigPlan> DigPlanList;

    HashMap<String, Integer> DirectionTranslate = new HashMap<>(Map.of("U", 0, "D", 1, "L", 2, "R", 3));

    void readFile() throws IOException {
        DigPlanList = new ArrayList<>();
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        String buf;
        while ((buf = input.readLine()) != null) {
            Matcher m = Pattern.compile("(\\w)\\s(\\d+)\\s\\(#([\\d\\w]+)\\)").matcher(buf);
            // Perform find first;
            if (m.find()) {
                DigPlanList.add(new DigPlan(DirectionTranslate.get(m.group(1)), Integer.parseInt(m.group(2)), m.group(3)));
            }
        }
        input.close();
    }

    HashSet<Point2D> Boundary, visited;

    Point2D corner1 = new Point2D(0, 0), corner2 = new Point2D(0, 0), startPos = new Point2D(0, 0);

    void Solution1() throws IOException {
        readFile();
        Boundary = new HashSet<>(Set.of(startPos));
        for (DigPlan curPlan : DigPlanList) {
            int curDirection = curPlan.first, curPushLen = curPlan.second.first;
            for (int i = 0; i < curPushLen; i++) {
                startPos.first += Point2D.dx[curDirection];
                startPos.second += Point2D.dy[curDirection];
                Boundary.add(new Point2D(startPos.first, startPos.second));
                corner1.first = Math.min(corner1.first, startPos.first);
                corner1.second = Math.min(corner1.second, startPos.second);
                corner2.first = Math.max(corner2.first, startPos.first);
                corner2.second = Math.max(corner2.second, startPos.second);
            }
        }
        int interiorCnt = Boundary.size();
        visited = new HashSet<>(Boundary);
        for (int i = corner1.first; i <= corner2.first; i++) {
            for (int j = corner1.second; j <= corner2.second; j++) {
                Point2D curPos = new Point2D(i, j);
                if (!visited.contains(curPos)) {
                    HashSet<Point2D> curVisited = new HashSet<>(Set.of(curPos));
                    if (BFS(curPos, curVisited)) {
                        interiorCnt += curVisited.size();
                    }
                }
            }
        }
        System.out.println("Solution 1: " + interiorCnt);
    }

    void Solution2() {
        // Scan by Line Segment
        for (var digplan : DigPlanList) {
            digplan.update();
        }
    }

    boolean BFS(Point2D curPos, HashSet<Point2D> curVisited) {
        Queue<Point2D> q = new LinkedList<>();
        q.add(curPos);
        boolean isInterior = true;
        while (!q.isEmpty()) {
            curPos = q.poll();
            for (int k = 0; k < 4; k++) {
                Point2D nextPos = Point2D.getNextPosition(curPos, k);
                if (isValid(corner1, corner2, nextPos)) {
                    if (!visited.contains(nextPos) && !curVisited.contains(nextPos)) {
                        q.add(nextPos);
                        curVisited.add(nextPos);
                    }
                } else {
                    isInterior = false;
                }
            }
        }
        // if (DFS(curPos, curVisited)) {
        // interiorCnt += curVisited.size();
        // }
        visited.addAll(curVisited);
        return isInterior;
    }

    boolean DFS(Point2D curPos, HashSet<Point2D> curVisited) {
        boolean isInterior = true;
        curVisited.add(curPos);
        for (int i = 0; i < 4; i++) {
            Point2D nextPos = Point2D.getNextPosition(curPos, i);
            if (isValid(corner1, corner2, curPos)) {
                if (!Boundary.contains(nextPos) && !curVisited.contains(nextPos)) {
                    curVisited.add(nextPos);
                    isInterior = isInterior && DFS(nextPos, curVisited);
                }
            } else
                isInterior = false;
        }
        return isInterior;
    }

    boolean isValid(Point2D corner1, Point2D corner2, Point2D curPos) {
        return curPos.first >= corner1.first && curPos.second >= corner1.second && curPos.first <= corner2.first && curPos.second <= corner2.second;
    }

    public static void main(String[] args) throws IOException {
        LavaInterior Day18 = new LavaInterior();
        Day18.Solution1();
    }

    class DigPlan extends Pair<Integer, Pair<Integer, String>> {
        public DigPlan(int direction, int depth, String hexRGB) {
            super(direction, new Pair<>(depth, hexRGB));
        }

        void update() {
            String hexRGB = this.second.second;
            this.second.first = Integer.parseInt(hexRGB.substring(0, hexRGB.length() - 1));
            this.first = Integer.parseInt(hexRGB.substring(hexRGB.length() - 1));
        }
    }
}
