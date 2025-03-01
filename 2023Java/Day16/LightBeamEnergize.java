package Day16;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;
import JavaDataModel.Pair;
import JavaDataModel.Point2D;

public class LightBeamEnergize {
    List<List<Character>> ContraptionLayout;

    void readFile() throws IOException {
        ContraptionLayout = new ArrayList<>();
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        String buf;
        while ((buf = input.readLine()) != null) {
            // List<Character> row = IntStream.range(0, buf.length()).mapToObj(i ->
            // buf.charAt(i)).toList();
            ContraptionLayout.add(buf.trim().chars().mapToObj(c -> (char) c).toList());
        }
        input.close();
    }

    HashMap<Character, HashMap<Integer, Integer>> nextDirection =
            new HashMap<>(Map.of('/', new HashMap<>(Map.of(0, 3, 1, 2, 2, 1, 3, 0)), '\\',
                    new HashMap<>(Map.of(0, 2, 1, 3, 2, 0, 3, 1))));

    void Solution1() throws IOException {
        readFile();
        int rows = ContraptionLayout.size(), cols = ContraptionLayout.get(0).size();
        HashSet<Point2D> resSet = new HashSet<>();
        Point2D entry = new Point2D(0, 0);
        Queue<Pair<Point2D, Integer>> q = new LinkedList<>();
        HashSet<Pair<Point2D, Integer>> visited = new HashSet<>();
        q.add(new Pair<>(entry, 3));
        while (!q.isEmpty()) {
            Pair<Point2D, Integer> curLight = q.poll();
            if (visited.contains(curLight))
                continue;
            resSet.add(curLight.first);
            visited.add(curLight);
            // printContraptionMap(resSet);
            char curTile =
                    ContraptionLayout.get(curLight.first.getKey()).get(curLight.first.getValue());
            if (curTile == '.' || (curTile == '|' && curLight.second < 2)
                    || (curTile == '-' && curLight.second > 1)) {
                Point2D nextPos = Point2D.getNextPosition(curLight.first, curLight.second);
                if (Point2D.isValid(rows, cols, nextPos))
                    q.add(new Pair<Point2D, Integer>(nextPos, curLight.second));
            } else if (nextDirection.containsKey(curTile)) {
                int nextDir = nextDirection.get(curTile).get(curLight.second);
                Point2D nextPos = Point2D.getNextPosition(curLight.first, nextDir);
                if (Point2D.isValid(rows, cols, nextPos))
                    q.add(new Pair<Point2D, Integer>(nextPos, nextDir));
            } else {
                int nextDir = 2;
                if (curLight.second >= 2) {
                    nextDir = 0;
                }
                Point2D nextPos1 = Point2D.getNextPosition(curLight.first, nextDir);
                if (Point2D.isValid(rows, cols, nextPos1))
                    q.add(new Pair<>(nextPos1, nextDir));
                Point2D nextPos2 = Point2D.getNextPosition(curLight.first, nextDir + 1);
                if (Point2D.isValid(rows, cols, nextPos2))
                    q.add(new Pair<>(nextPos2, nextDir + 1));
            }
        }

        System.out.println("Solution 1: " + resSet.size());
    }

    void Solution2() {
        int maxTiles = 0;
    }

    void printContraptionMap(HashSet<Point2D> resSet) {
        List<List<Character>> canvas = ContraptionLayout.stream()
                .map(list -> (List<Character>) (new ArrayList<>(list))).toList();
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
    }
}
