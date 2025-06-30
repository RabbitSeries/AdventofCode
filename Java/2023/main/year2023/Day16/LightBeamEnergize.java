package year2023.Day16;

import java.io.*;
import java.util.*;
import JavaDataModel.*;

@AoCSolution()
public class LightBeamEnergize implements SolutionBase {
    List<List<Character>> ContraptionLayout;

    void readFile() throws IOException {
        ContraptionLayout = new ArrayList<>();
        BufferedReader input = new BufferedReader(new FileReader("Day16/input.txt"));
        String buf;
        while ((buf = input.readLine()) != null) {
            // List<Character> row = IntStream.range(0, buf.length()).mapToObj(i
            // ->
            // buf.charAt(i)).toList();
            ContraptionLayout
                    .add(buf.trim().chars().mapToObj(c -> (char) c).toList());
        }
        input.close();
    }

    HashMap<Character, HashMap<Integer, Integer>> nextDirection = new HashMap<>(
            Map.of('/', new HashMap<>(Map.of(0, 3, 1, 2, 2, 1, 3, 0)), '\\',
                    new HashMap<>(Map.of(0, 2, 1, 3, 2, 0, 3, 1))));

    int energizeTiles(Point2D entry, int face) {
        HashSet<Point2D> resSet = new HashSet<>();
        int rows = ContraptionLayout.size(),
                cols = ContraptionLayout.get(0).size();
        Queue<Pair<Point2D, Integer>> q = new LinkedList<>();
        HashSet<Pair<Point2D, Integer>> visited = new HashSet<>();
        q.add(new Pair<>(entry, face));
        while (!q.isEmpty()) {
            Pair<Point2D, Integer> curLight = q.poll();
            if (visited.contains(curLight))
                continue;
            resSet.add(curLight.first);
            visited.add(curLight);
            // printContraptionMap(resSet);
            char curTile = ContraptionLayout.get(curLight.first.first)
                    .get(curLight.first.second);
            if (curTile == '.' || (curTile == '|' && curLight.second < 2)
                    || (curTile == '-' && curLight.second > 1)) {
                Point2D nextPos = Point2D.getNextPosition(curLight.first,
                        curLight.second);
                if (Point2D.isValid(rows, cols, nextPos))
                    q.add(new Pair<Point2D, Integer>(nextPos, curLight.second));
            } else if (nextDirection.containsKey(curTile)) {
                int nextDir = nextDirection.get(curTile).get(curLight.second);
                Point2D nextPos =
                        Point2D.getNextPosition(curLight.first, nextDir);
                if (Point2D.isValid(rows, cols, nextPos))
                    q.add(new Pair<Point2D, Integer>(nextPos, nextDir));
            } else {
                int nextDir = 2;
                if (curLight.second >= 2) {
                    nextDir = 0;
                }
                for (int i = 0; i < 2; i++) {
                    Point2D nextPos = Point2D.getNextPosition(curLight.first,
                            nextDir + i);
                    if (Point2D.isValid(rows, cols, nextPos))
                        q.add(new Pair<>(nextPos, nextDir + i));
                }
            }
        }
        // printContraptionMap(resSet);
        return resSet.size();
    }

    public void Solution1() throws IOException {
        readFile();
        System.out
                .println("Solution 1: " + energizeTiles(new Point2D(0, 0), 3));
    }

    public void Solution2() {
        int maxEnergizedTiles = 0;
        // Point2D targetEntry = null;
        int rows = ContraptionLayout.size(),
                cols = ContraptionLayout.get(0).size();
        for (int j = 0; j < cols; j++) {
            maxEnergizedTiles = Math.max(energizeTiles(new Point2D(0, j), 1),
                    maxEnergizedTiles);
            maxEnergizedTiles =
                    Math.max(energizeTiles(new Point2D(rows - 1, j), 0),
                            maxEnergizedTiles);
            // memo.put(new Pair<>(curEntry, 1), curEnergized);
        }
        for (int i = 0; i < rows; i++) {
            maxEnergizedTiles = Math.max(energizeTiles(new Point2D(i, 0), 2),
                    maxEnergizedTiles);
            maxEnergizedTiles =
                    Math.max(energizeTiles(new Point2D(i, cols - 1), 3),
                            maxEnergizedTiles);
        }
        System.out.println("Solution 2: " + maxEnergizedTiles);
    }

    void printContraptionMap(HashSet<Point2D> resSet) {
        System.out.println();
        List<List<Character>> canvas = ContraptionLayout.stream()
                .map(list -> (List<Character>) (new ArrayList<>(list)))
                .toList();
        for (var p : resSet)
            canvas.get(p.first).set(p.second, '#');
        canvas.forEach(l -> {
            l.forEach(c -> System.out.print(c));
            System.out.println();
        });
    }

    public static void main(String[] args) throws IOException {
        LightBeamEnergize Solution = new LightBeamEnergize();
        Solution.Solution1();
        Solution.Solution2();
    }
}
