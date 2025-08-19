package year2023.Day07;

import java.io.*;
import java.util.*;
import java.util.Map.*;
import java.util.stream.*;

import JavaDataModel.*;

import java.util.AbstractMap.*;

@AoCSolution()
public class CardSort implements SolutionBase {
    List<Entry<String, Integer>> cardBits;

    static String Rule1Cards = "AKQJT98765432";

    static String Rule2Cards = "AKQT98765432J";

    enum Priority {
        HighCar, OnePair, TwoPair, ThreeOfAKind, FullHouse, FourOfAKind, FiveOfAKind
    }

    int cardHoldCompare(String hold, String rule) {
        return hold.chars().map(c -> rule.indexOf(c)).reduce(0, (a, b) -> a * rule.length() + b);
    }

    Priority rule1Priority(String str) {
        long maxSameChCnt = str.chars().boxed()
                .collect(Collectors.groupingBy(l -> l, Collectors.counting()))
                // .values().stream().max(Long::compare).orElse(0l);
                .values().stream().reduce(0L, Long::max);

        switch ((int) maxSameChCnt) {
            case 1:
                return Priority.HighCar;
            case 2:
                return str.chars().boxed()
                        .collect(Collectors.groupingBy(Integer::valueOf, Collectors.counting()))
                        .values().stream().filter(l -> l.longValue() == 2).count() == 2 ? Priority.TwoPair : Priority.OnePair;
            case 3:
                return str.chars().boxed()
                        .collect(Collectors.groupingBy(Integer::valueOf, Collectors.counting()))
                        // .values().stream().collect(Collectors.groupingBy(List::size))
                        .containsValue(2L) ? Priority.FullHouse : Priority.ThreeOfAKind;
            default:
                return Priority.values()[(int) maxSameChCnt + 1];
        }
    }

    Priority wildCards(String str) {
        long wildCardCnt = str.chars().filter(ascii -> (char) ascii == 'J').count();

        if (wildCardCnt == 0) {
            return rule1Priority(str);
        }

        long maxSameChCnt = str.chars().filter(ascii -> (char) ascii != 'J').boxed()
                .collect(Collectors.groupingBy(l -> l, Collectors.counting())).values().stream()
                .max(Long::compare).orElse(0l);

        long totalSameChCnt = wildCardCnt + maxSameChCnt;

        if (totalSameChCnt == 2) {
            return Priority.OnePair;
        }

        if (totalSameChCnt == 3) {
            if (maxSameChCnt == 1) {
                return Priority.ThreeOfAKind;
            } else if (maxSameChCnt == 2) {
                long counter = str.chars().filter(ascii -> (char) ascii != 'J').boxed()
                        .collect(Collectors.groupingBy(l -> l, Collectors.counting())).values()
                        .stream().filter(l -> l == 2).count();
                if (counter == 2) {
                    return Priority.FullHouse;
                } else if (counter == 1) {
                    return Priority.ThreeOfAKind;
                }
            }
        }
        // totalSameChCnt == 4 , 1+3 (1) , 2+2 (1), 3+1(1)
        if (totalSameChCnt == 4)
            return Priority.FourOfAKind;
        else if (totalSameChCnt == 5) {
            return Priority.FiveOfAKind;
        }
        throw new ExceptionInInitializerError("Unexpected Total SameChcnt");
    }

    void readFile(BufferedReader input) throws IOException {
        cardBits = input.lines()
                .map(line -> line.trim().split("\\s+"))
                .map(line -> new SimpleEntry<>(line[0], Integer.parseInt(line[1])))
                .collect(ArrayList::new, ArrayList::add, ArrayList::addAll);
    }

    public void Solution1(BufferedReader input) throws IOException {
        readFile(input);
        cardBits.sort(Comparator.<Entry<String, Integer>, Priority> comparing(e -> rule1Priority(e.getKey()))
                .thenComparing(Comparator.<Entry<String, Integer>, Integer> comparing(e -> cardHoldCompare(e.getKey(), Rule1Cards)).reversed()));
        System.out.println("Solution 1: " + IntStream.range(0, cardBits.size()).mapToLong(i -> cardBits.get(i).getValue() * (i + 1)).sum());
    }

    public void Solution2(BufferedReader input) throws IOException {
        cardBits.sort(Comparator.<Entry<String, Integer>, Priority> comparing(e -> wildCards(e.getKey()))
                .thenComparing(Comparator.<Entry<String, Integer>, Integer> comparing(e -> cardHoldCompare(e.getKey(), Rule2Cards)).reversed()));
        System.out.println("Solution 2: " + IntStream.range(0, cardBits.size()).mapToLong(i -> cardBits.get(i).getValue() * (i + 1)).sum());
    }

    public static void main(String[] args) throws IOException {
        // long now = System.nanoTime();
        CardSort Day07 = new CardSort();
        Day07.Solution1(new BufferedReader(new FileReader("Day07/input.txt")));
        Day07.Solution2(new BufferedReader(new FileReader("Day07/input.txt")));
        // long end = System.nanoTime();
        // System.out.printf("%.3f", (end - now) / 1000000.0);
    }
}
