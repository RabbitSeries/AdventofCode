package year2023.Day03;

import java.io.*;
import java.util.*;
import java.util.stream.IntStream;

import JavaDataModel.*;

@AoCSolution()
public class PartNumberSearch implements SolutionBase {
    List<String> schematic;

    List<Point2D> GearPositions = new ArrayList<>();

    Map<Point2D, Integer> NumPositions = new HashMap<>();

    int rows, cols;

    void readFile(BufferedReader input) throws IOException {
        schematic = input.lines().toList();

        rows = schematic.size();
        cols = schematic.get(0).length();

        IntStream.range(0, rows).forEach(i -> {
            Optional<Point2D> num = Optional.empty();
            for (int j : IntStream.range(0, cols).toArray()) {
                char ch = schematic.get(i).charAt(j);
                if (Character.isDigit(ch)) {
                    if (!num.isPresent()) {
                        num = Optional.of(new Point2D(i, j));
                    }
                    NumPositions.compute(num.get(), (k, v) -> v == null ? Integer.valueOf(String.valueOf(ch)) : Integer.valueOf(v.toString().concat(String.valueOf(ch))));
                } else {
                    if (num.isPresent()) {
                        num = Optional.empty();
                    }
                    if (ch == '*') {
                        GearPositions.add(new Point2D(i, j));
                    }
                }
            }
        });
    }

    public void Solution1(BufferedReader input) throws IOException {
        readFile(input);
        System.out.println("Solution 1: " + NumPositions.keySet().stream().filter(pos -> {
            for (int i = 0; i < NumPositions.get(pos).toString().length(); i++) {
                Point2D curPos = new Point2D(pos.getKey(), pos.getValue() + i);
                for (int direction = 0; direction < 8; direction++) {
                    Point2D nextPos = Point2D.getNextPosition(curPos, direction);
                    if (Point2D.isValid(rows, cols, nextPos)) {
                        char ch = schematic.get(nextPos.getKey()).charAt(nextPos.getValue());
                        if (!Character.isDigit(ch) && ch != '.') {
                            return true;
                        }
                    }
                }
            }
            return false;
        }).mapToInt(k -> NumPositions.get(k)).sum());
    }

    Integer searchNumber(int rows, int cols, Point2D pos, Set<Point2D> visited) {
        int startIndex = pos.getValue(), endIndex = pos.getValue(), x = pos.getKey();
        Point2D nextPos = new Point2D(x, startIndex - 1);
        // Search before
        while (nextPos.getValue() >= 0 && Character.isDigit(schematic.get(x).charAt(nextPos.getValue()))) {
            startIndex--;
            visited.add(nextPos);
            nextPos = new Point2D(x, startIndex - 1);
        }
        // Search forward
        nextPos = new Point2D(x, endIndex + 1);
        while (nextPos.getValue() < cols && Character.isDigit(schematic.get(x).charAt(nextPos.getValue()))) {
            endIndex++;
            visited.add(nextPos);
            nextPos = new Point2D(x, endIndex + 1);
        }
        return Integer.parseInt(schematic.get(x).substring(startIndex, endIndex + 1));
    }

    public void Solution2(BufferedReader input) throws IOException {
        System.out.println("Solution 2: " + GearPositions.stream().map(pos -> {
            List<Integer> nums = new ArrayList<>();
            Set<Point2D> visited = new HashSet<>();
            for (int i = 0; i < 8; i++) {
                Point2D nextPos = Point2D.getNextPosition(pos, i);
                if (Point2D.isValid(rows, cols, nextPos) && Character.isDigit(schematic.get(nextPos.getKey()).charAt(nextPos.getValue()))) {
                    if (!visited.contains(nextPos)) {
                        visited.add(nextPos);
                        nums.add(searchNumber(rows, cols, nextPos, visited));
                    }
                }
            }
            return nums;
        }).filter(l -> l.size() == 2).mapToInt(l -> l.get(0) * l.get(1)).sum());
    }

    public static void main(String[] args) throws Exception {
        PartNumberSearch solution = new PartNumberSearch();
        solution.Solution1(new BufferedReader(new FileReader("Day03/input.txt")));
        solution.Solution2(new BufferedReader(new FileReader("Day03/input.txt")));
    }
}
