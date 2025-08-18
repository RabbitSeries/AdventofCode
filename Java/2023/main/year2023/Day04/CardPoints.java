package year2023.Day04;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import JavaDataModel.*;

@AoCSolution()
public class CardPoints implements SolutionBase {

    List<Set<Integer>> WinNumsList = new ArrayList<>();

    List<List<Integer>> CardsList = new ArrayList<>();

    List<Long> WinResultList;

    int CardN = 0;

    void readFile(BufferedReader input) {
        input.lines().toList().forEach(line -> {
            String[] cards = line.split(":")[1].split("\\|");
            WinNumsList.add(Stream.of(cards[0].trim().split("\\s+")).map(Integer::parseInt).collect(Collectors.toSet()));
            CardsList.add(Stream.of(cards[1].trim().split("\\s+")).map(Integer::parseInt).toList());
        });
        CardN = WinNumsList.size();
    }

    public void Solution1(BufferedReader input) {
        readFile(input);
        WinResultList = IntStream.range(0, CardN)
                .mapToObj(i -> CardsList.get(i).stream().filter(c -> WinNumsList.get(i).contains(c)).count()).toList();
        System.out.println("Solution 1: " + WinResultList.stream().filter(n -> n > 0).mapToLong(n -> (long) Math.pow(2, n - 1)).sum());
    }

    public void Solution2(BufferedReader input) throws IOException {
        List<Integer> dp = IntStream.range(0, CardN).mapToObj(__ -> 1).collect(Collectors.toCollection(ArrayList::new));
        IntStream.range(0, CardN).forEach(i -> {
            long bonus = WinResultList.get(i);
            for (int j = 1; j <= bonus; j++) {
                if (i + j < dp.size()) {
                    dp.set(i + j, dp.get(i + j) + dp.get(i));
                }
            }
        });
        System.out.println("Solution 2: " + dp.stream().reduce(0, Integer::sum));
    }

    public static void main(String[] args) throws Exception {
        CardPoints solution = new CardPoints();
        solution.Solution1(new BufferedReader(new FileReader("Day04/input.txt")));
        solution.Solution2(new BufferedReader(new FileReader("Day04/input.txt")));
    }
}
