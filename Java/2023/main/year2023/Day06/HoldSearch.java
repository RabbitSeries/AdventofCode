package year2023.Day06;

import java.io.*;
import java.util.*;
import java.util.stream.*;

import JavaDataModel.*;

@AoCSolution()
public class HoldSearch implements ISolution {
    List<Pair<Integer, Integer>> recordList;

    void readFile(BufferedReader input) throws IOException {
        String[] lines = input.lines().toArray(String[]::new);
        Iterator<Integer> TimeList = Stream.of(lines[0].split(":")[1].trim().split("\\s+")).map(Integer::parseInt).iterator();
        Iterator<Integer> DistanceList = Stream.of(lines[1].split(":")[1].trim().split("\\s+")).map(Integer::parseInt).iterator();
        recordList = Stream.generate(() -> {
            if (TimeList.hasNext() && DistanceList.hasNext()) {
                return new Pair<>(TimeList.next(), DistanceList.next());
            }
            return null;
        }).takeWhile(Objects::nonNull).toList();
    }

    long binarySearch(long totalTime, long distance) {
        long s = 1, t = totalTime / 2;
        while (s != t) {
            long mid = (s + t) / 2;
            long val = (totalTime - mid) * mid;
            if (val >= distance) {
                t = mid;
            } else {
                s = mid + 1;
            }
        }
        return (totalTime / 2 - s + 1) * 2 - (totalTime % 2 == 0 ? 1 : 0);
    }

    public void Solution1(BufferedReader input) throws IOException {
        readFile(input);
        System.out.println("Solution 1: " + recordList.stream().map(K_V -> binarySearch(K_V.getKey(), K_V.getValue())).reduce(1L, (a, b) -> a * b));
    }

    public void Solution2(BufferedReader input) throws IOException {
        long totalTime = Long.parseLong(recordList.stream().map(K_V -> K_V.getKey().toString()).collect(Collectors.joining()));
        long distance = Long.parseLong(recordList.stream().map(K_V -> K_V.getValue().toString()).collect(Collectors.joining()));
        System.out.println("Solution 2: " + binarySearch(totalTime, distance));
    }

    public static void main(String[] args) throws IOException {
        HoldSearch solution = new HoldSearch();
        solution.Solution1(new BufferedReader(new FileReader("Day06/input.txt")));
        solution.Solution2(new BufferedReader(new FileReader("Day06/input.txt")));
    }
}
