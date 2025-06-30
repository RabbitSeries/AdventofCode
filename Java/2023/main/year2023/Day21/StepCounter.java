package year2023.Day21;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;

import JavaDataModel.*;

@AoCSolution()
public class StepCounter implements SolutionBase {
    List<String> Farm = null, ZeroFarm = null;

    int ROW = 0;

    int COL = 0;

    Point2D start = new Point2D(-1, -1);

    void readFile() throws Exception {
        BufferedReader input = new BufferedReader(new FileReader("Day21/input.txt"));
        Farm = input.lines().map(line -> {
            var sb = new StringBuilder(line);
            if (start.second == -1) {
                start.first++;
                start.second = line.indexOf('S');
                if (start.second != -1) {
                    sb.setCharAt(start.second, '.');
                }
            }
            return sb.toString();
        }).toList();
        ROW = Farm.size();
        COL = Farm.get(0).length();
        ZeroFarm = Farm;
        input.close();// Java is not RAII
    }

    void printToDisk(Set<Point2D> visited) throws Exception {
        BufferedWriter output = new BufferedWriter(new FileWriter("output.txt"));
        for (int i = 0; i < Farm.size(); i++) {
            for (int j = 0; j < Farm.get(i).length(); j++) {
                if (visited.contains(new Point2D(i, j))) {
                    output.append('O');
                } else {
                    if (start.first.equals(i) && start.second.equals(j))
                        output.append('S');
                    else
                        output.append(' ');
                }
            }
            output.newLine();
        }
        output.close();
    }

    int[] dx = new int[] {
            -1, 1, 0, 0
    };

    int[] dy = new int[] {
            0, 0, -1, 1
    };

    boolean isValid(Point2D pos) {
        return pos.first >= 0 && pos.second >= 0 && pos.first < Farm.size() && pos.second < Farm.get(0).length();
    }

    Point2D getNextPoint(Point2D Original, int direction) {
        Point2D nextPoint = new Point2D(Original.first + dx[direction], Original.second + dy[direction]);
        // nextPoint.first = (nextPoint.first + ROW) % ROW;
        // nextPoint.second = (nextPoint.second + COL) % COL;
        return nextPoint;
    }

    int ManhattanDistance(Point2D p, Point2D q) {
        return Math.abs(p.first - q.first) + Math.abs(p.second - q.second);
    }

    int SimpleBFS(Point2D zero, int stepCnt, int boundary) throws Exception {
        Set<Point2D> visited = new HashSet<>();
        Queue<Point2D> q = new LinkedList<>();
        q.add(zero);
        for (int curStep = 0; curStep < stepCnt; curStep++) {
            Queue<Point2D> nextLevel = new LinkedList<>();
            visited.clear();
            while (!q.isEmpty()) {
                Point2D curPoint = q.poll();
                for (int i = 0; i < 4; i++) {
                    Point2D nextPos = getNextPoint(curPoint, i);
                    if (isValid(nextPos) && ManhattanDistance(nextPos, zero) <= boundary && Farm.get(nextPos.first).charAt(nextPos.second) == '.' && !visited.contains(nextPos)) {
                        visited.add(nextPos);
                        nextLevel.add(nextPos);
                    }
                }
            }
            q = nextLevel;
            // if (curStep % 2 == stepCnt % 2) {
            // printToDisk(visited);
            // Thread.sleep(100);
            // }
        }
        // printToDisk(visited);
        return visited.size();
    }

    void FoldTo(int times) {
        start.first = ROW / 2 + ROW * times;
        start.second = COL / 2 + COL * times;
        Farm = Collections.nCopies(2 * times + 1,
                ZeroFarm.stream().map(
                        row -> Collections.nCopies(2 * times + 1, row).stream().collect(Collectors.joining()))
                        .toList())
                .stream().flatMap(l -> l.stream()).toList();
    }

    public void Solution1() throws Exception {
        readFile();
        // int times = 1;
        // FoldTo(times);
        System.out.println("Solution 1: " + SimpleBFS(start, 64, 64));
        // System.out.println("Solution 1: " + SimpleBFS(start, ROW + ROW / 2 + (ROW + ROW / 2 + 1) % 2, ROW + ROW / 2));
        // System.out.println("Solution 1: " + SimpleBFS(start, ROW * times + ROW / 2 + times % 2 + 2, ROW * times + ROW / 2 + times % 2));

        // 131*0+65+2 3752 (also fully filled)
        // 131*2+65 + 2 93252 (means the square has already been filled)
        // 131*4+65 + 2 301856 (means the square has already been filled)
    }

    int getOdd(int val) {
        return val + (val + 1) % 2;
    }

    int getEven(int val) {
        return val + (val) % 2;

    }

    public void Solution2() throws Exception {
        final int STEP = 26501365;
        long Layer = STEP / COL;
        long oddBase = SimpleBFS(start, getOdd(ROW + ROW / 2), ROW + ROW / 2);
        long oddCorner = oddBase - SimpleBFS(start, getOdd(ROW / 2), ROW / 2);
        long evenBase = SimpleBFS(start, getEven(ROW + ROW / 2), ROW + ROW / 2);
        long evenCorner = evenBase - SimpleBFS(start, getEven(ROW / 2), ROW / 2);
        System.out.println("Solution 2: " + ((Layer + 1) * (Layer + 1) * oddBase + Layer * Layer * evenBase - (Layer + 1) * oddCorner + Layer * evenCorner));
    }

    public void LagrangeSolution() throws Exception {
        FoldTo(2);
        int[] steps = {
                ROW / 2, ROW + ROW / 2, ROW * 2 + ROW / 2
        };
        long[] f = new long[3];
        for (int i = 0; i < 3; i++) {
            f[i] = SimpleBFS(start, steps[i], steps[i]);
            System.out.println("f(" + i + ") = " + f[i]);
        }
        final long STEP = 26501365;
        long N = STEP / ROW;
        long term0 = f[0] * (N - 1) * (N - 2) / ((0 - 1) * (0 - 2));
        long term1 = f[1] * (N - 0) * (N - 2) / ((1 - 0) * (1 - 2));
        long term2 = f[2] * (N - 0) * (N - 1) / ((2 - 0) * (2 - 1));

        long result = term0 + term1 + term2;
        System.out.println("Final Result: " + result);
    }

    public static void main(String[] args) throws Exception {
        StepCounter Day21 = new StepCounter();
        Day21.Solution1();
        Day21.Solution2();
        // Day21.LagrangeSolution();
    }
}
