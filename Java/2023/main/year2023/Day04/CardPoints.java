package year2023.Day04;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import JavaDataModel.*;

@AoCSolution()
public class CardPoints implements ISolution {

    List<Pair<Set<Integer>, List<Integer>>> CardsList;

    List<Long> WinResultList;

    int CardN = 0;

    void readFile(BufferedReader input) {
        CardsList = input.lines().map(line -> {
            String[] cards = line.split(":")[1].split("\\|");
            return new Pair<>(Stream.of(cards[0].trim().split("\\s+")).map(Integer::parseInt).collect(Collectors.toSet()),
                    Stream.of(cards[1].trim().split("\\s+")).map(Integer::parseInt).toList());
        }).toList();
        CardN = CardsList.size();
    }

    public void Solution1(BufferedReader input) {
        readFile(input);
        WinResultList = CardsList.stream().map(K_V -> K_V.getValue().stream().filter(V -> K_V.getKey().contains(V)).count()).toList();
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
