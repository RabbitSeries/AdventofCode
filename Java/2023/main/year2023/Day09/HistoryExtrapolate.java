package year2023.Day09;

import java.io.*;
import java.util.*;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import JavaDataModel.*;

@AoCSolution()
public class HistoryExtrapolate implements ISolution {
    List<List<Integer>> HistoryList;

    int predict(List<Integer> histroyList, boolean isForward) {
        if (histroyList.stream().allMatch(l -> Integer.compare(l, 0) == 0)) {
            return 0;
        }
        List<Integer> nextLevelList = IntStream.range(1, histroyList.size()).map(i -> histroyList.get(i) - histroyList.get(i - 1)).boxed().toList();
        return (isForward ? histroyList.getLast() : histroyList.getFirst()) + (isForward ? 1 : -1) * predict(nextLevelList, isForward);
    }

    public void Solution1(BufferedReader input) throws IOException {
        HistoryList = input.lines().map(line -> Stream.of(line.split("\\s+")).map(Integer::parseInt).toList()).toList();
        System.out.println("Solution 1: " + HistoryList.stream().mapToInt(l -> predict(l, true)).sum());
    }

    public void Solution2(BufferedReader input) throws IOException {
        System.out.println("Solution 2: " + HistoryList.stream().mapToInt(l -> predict(l, false)).sum());
    }

    public static void main(String[] args) throws IOException {
        HistoryExtrapolate Day09 = new HistoryExtrapolate();
        Day09.Solution1(new BufferedReader(new FileReader("Day09/input.txt")));
        Day09.Solution2(new BufferedReader(new FileReader("Day09/input.txt")));
    }
}
