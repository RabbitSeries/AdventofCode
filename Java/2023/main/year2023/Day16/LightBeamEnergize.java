package year2023.Day16;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;

import JavaDataModel.*;

@AoCSolution()
public class LightBeamEnergize implements SolutionBase {
    List<String> ContraptionLayout;

    int rows, cols;

    Map<Character, Map<Integer, Integer>> nextDirection = new HashMap<>(
            Map.of('/', new HashMap<>(Map.of(0, 3, 1, 2, 2, 1, 3, 0)),
                    '\\', new HashMap<>(Map.of(0, 2, 1, 3, 2, 0, 3, 1))));

    int energizeTiles(Point2D entry, int face) {
        HashSet<Point2D> resSet = new HashSet<>();
        Queue<Pair<Point2D, Integer>> q = new LinkedList<>();
        HashSet<Pair<Point2D, Integer>> visited = new HashSet<>();
        q.add(new Pair<>(entry, face));
        while (!q.isEmpty()) {
            Pair<Point2D, Integer> curLight = q.poll();
            if (visited.contains(curLight)) {
                continue;
            }
            resSet.add(curLight.first);
            visited.add(curLight);
            char curTile = ContraptionLayout.get(curLight.first.first).charAt(curLight.first.second);
            if (curTile == '.' || (curTile == '|' && curLight.second < 2) || (curTile == '-' && curLight.second > 1)) {
                Point2D nextPos = Point2D.getNextPosition(curLight.first, curLight.second);
                if (Point2D.isValid(rows, cols, nextPos)) {
                    q.add(new Pair<Point2D, Integer>(nextPos, curLight.second));
                }
            } else if (nextDirection.containsKey(curTile)) {
                int nextDir = nextDirection.get(curTile).get(curLight.second);
                Point2D nextPos = Point2D.getNextPosition(curLight.first, nextDir);
                if (Point2D.isValid(rows, cols, nextPos)) {
                    q.add(new Pair<Point2D, Integer>(nextPos, nextDir));
                }
            } else {
                int nextDir = 2;
                if (curLight.second >= 2) {
                    nextDir = 0;
                }
                for (int i = 0; i < 2; i++) {
                    Point2D nextPos = Point2D.getNextPosition(curLight.first, nextDir + i);
                    if (Point2D.isValid(rows, cols, nextPos)) {
                        q.add(new Pair<>(nextPos, nextDir + i));
                    }
                }
            }
        }
        return resSet.size();
    }

    public void Solution1(BufferedReader input) throws IOException {
        ContraptionLayout = input.lines().toList();
        rows = ContraptionLayout.size();
        cols = ContraptionLayout.get(0).length();
        System.out.println("Solution 1: " + energizeTiles(new Point2D(0, 0), 3));
    }

    public void Solution2(BufferedReader input) {
        int maxEnergizedTiles = 0;
        for (int j = 0; j < cols; j++) {
            maxEnergizedTiles = Math.max(energizeTiles(new Point2D(0, j), 1), maxEnergizedTiles);
            maxEnergizedTiles = Math.max(energizeTiles(new Point2D(rows - 1, j), 0), maxEnergizedTiles);
        }
        for (int i = 0; i < rows; i++) {
            maxEnergizedTiles = Math.max(energizeTiles(new Point2D(i, 0), 2), maxEnergizedTiles);
            maxEnergizedTiles = Math.max(energizeTiles(new Point2D(i, cols - 1), 3), maxEnergizedTiles);
        }
        System.out.println("Solution 2: " + maxEnergizedTiles);
    }

    void printContraptionMap(Set<Point2D> resSet) {
        System.out.println(ContraptionLayout.stream().collect(Collectors.joining("\n")));
    }

    public static void main(String[] args) throws IOException {
        LightBeamEnergize Day16 = new LightBeamEnergize();
        Day16.Solution1(new BufferedReader(new FileReader("Day16/input.txt")));
        Day16.Solution2(new BufferedReader(new FileReader("Day16/input.txt")));
    }
}
