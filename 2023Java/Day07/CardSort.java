package Day07;

import java.io.*;
import java.util.*;
import java.util.Map.*;
import java.util.stream.*;
import java.util.AbstractMap.*;

public class CardSort {
    List<Entry<String, Integer>> cardBits;

    static String Rule1Cards = "AKQJT98765432";
    static String Rule2Cards = "AKQT98765432J";

    enum Priority {
        HighCar, OnePair, TwoPair, ThreeOfAKind, FullHouse, FourOfAKind, FiveOfAKind
    }

    int cardHoldCompare(String hold1, String hold2, String rule) {
        for (int i = 0; i < 5; i++) {
            int cp1 = rule.indexOf(hold1.charAt(i)), cp2 = rule.indexOf(hold2.charAt(i));
            if (cp1 != cp2) {
                return -1 * Integer.compare(cp1, cp2);
            }
        }
        return 0;
    }

    Priority rule1Priority(String str) {
        // Be familiar with stream programming, but also learn yourself the way of
        // manual implementation.
        long maxSameChCnt =
                str.chars().boxed().collect(Collectors.groupingBy(l -> l, Collectors.counting()))
                        .values().stream().max(Long::compareTo).orElse(0l);

        Priority[] priorityValues = Priority.values();

        if (maxSameChCnt == 1)
            return Priority.HighCar;

        if (maxSameChCnt == 2) {
            if (str.chars().boxed().collect(Collectors.groupingBy(l -> l, Collectors.counting()))
                    .values().stream().filter(l -> (int) (long) l == 2).count() == 2) {
                return Priority.TwoPair;
            } else {
                return Priority.OnePair;
            }
        }

        if (maxSameChCnt == 3) {
            var list = str.chars().boxed().collect(Collectors.groupingBy(l -> l)).values().stream()
                    .collect(Collectors.groupingBy(List::size)).getOrDefault(2, null);
            if (list == null) {
                return Priority.ThreeOfAKind;
            } else {
                return Priority.FullHouse;
            }
        }

        return priorityValues[(int) maxSameChCnt + 1];
    }

    Priority wildCards(String str) {
        long wildCardCnt = str.chars().filter(ascii -> (char) ascii == 'J').count();

        if (wildCardCnt == 0) {
            return rule1Priority(str);
        }

        long maxSameChCnt = str.chars().filter(ascii -> (char) ascii != 'J').boxed()
                .collect(Collectors.groupingBy(l -> l, Collectors.counting())).values().stream()
                .max((l1, l2) -> Long.compare(l1, l2)).orElse(0l);

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

    void readFile() throws IOException {
        cardBits = new ArrayList<>();
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        String buf;
        while ((buf = input.readLine()) != null) {
            String[] line = buf.trim().split("\\s+");
            cardBits.add(new SimpleEntry<>(line[0], Integer.parseInt(line[1])));
        }
        input.close();
    }

    void Solution1() throws IOException {
        long res = 0;
        readFile();
        cardBits.sort((e1, e2) -> {
            int pr = rule1Priority(e1.getKey()).compareTo(rule1Priority(e2.getKey()));
            return (pr == 0 ? cardHoldCompare(e1.getKey(), e2.getKey(), Rule1Cards) : pr);
            // return Integer.compare(e1.getValue(), e2.getValue());
        });
        long i = 0;
        for (Entry<String, Integer> cardBit : cardBits) {
            i++;
            res += (long) cardBit.getValue() * i;
        }
        System.out.println("Solution 1: " + res);
        // System.out.println(cardBits);
    }

    void Solution2() throws IOException {
        long res = 0;
        readFile();
        cardBits.sort((e1, e2) -> {
            int pr = wildCards(e1.getKey()).compareTo(wildCards(e2.getKey()));
            return (pr == 0 ? cardHoldCompare(e1.getKey(), e2.getKey(), Rule2Cards) : pr);
            // return Integer.compare(e1.getValue(), e2.getValue());
        });
        long i = 0;
        for (Entry<String, Integer> cardBit : cardBits) {
            i++;
            res += (long) cardBit.getValue() * i;
        }
        System.out.println("Solution 2: " + res);
    }

    public static void main(String[] args) throws IOException {
        long now = System.nanoTime();
        CardSort Day07 = new CardSort();
        Day07.Solution1();
        Day07.Solution2();
        long end = System.nanoTime();
        System.out.printf("%.3f", (end - now) / 1000000.0);
    }
}
