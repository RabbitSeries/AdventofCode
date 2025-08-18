package year2023.Day02;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import JavaDataModel.*;

@AoCSolution()
public class CubeClassify implements SolutionBase {

    List<List<Map<String, Integer>>> GameList;

    void readFile(BufferedReader input) throws IOException {
        GameList = input.lines()
                .map(line -> Stream.of(line.split(":")[1].split(";"))
                        .map(grab -> Stream.of(grab.split(","))
                                .map(cubeInfo -> {
                                    String[] arr = cubeInfo.trim().split("\\s");
                                    return new Pair<>(arr[1], Integer.parseInt(arr[0]));
                                })
                                .collect(Collectors.toMap(Pair::getKey, Pair::getValue)))
                        .toList())
                .toList();
    }

    public void Solution1(BufferedReader input) throws IOException {
        readFile(input);
        Map<String, Integer> available = new HashMap<>(Map.of("red", 12, "green", 13, "blue", 14));
        System.out.println("Solution 1: " + IntStream.range(0, GameList.size())
                .filter(gameId -> GameList.get(gameId).stream()
                        .allMatch(m -> m.keySet().stream().allMatch(k -> m.get(k) <= available.get(k))))
                .map(i -> i + 1).sum());
    }

    public void Solution2(BufferedReader input) throws IOException {
        System.out.println("Solution 2: " + GameList.stream().mapToInt(game -> {
            Map<String, Integer> res = new HashMap<>();
            for (var m : game) {
                m.forEach((k, v) -> {
                    res.putIfAbsent(k, v);
                    if (res.get(k) < v) {
                        res.put(k, v);
                    }
                });
            }
            return res.values().stream().reduce(1, (a, b) -> a * b);
        }).sum());
    }

    public static void main(String[] args) throws Exception {
        CubeClassify solution = new CubeClassify();
        solution.Solution1(new BufferedReader(new FileReader("Day02/input.txt")));
        solution.Solution2(new BufferedReader(new FileReader("Day02/input.txt")));
    }
}
