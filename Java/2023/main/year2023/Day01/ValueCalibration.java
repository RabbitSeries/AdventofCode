package year2023.Day01;

import java.io.*;
import java.util.*;
import java.util.stream.IntStream;

import JavaDataModel.*;

@AoCSolution()
public class ValueCalibration implements SolutionBase {
    List<String> maplist = null;

    Optional<Integer> isDigit(String line, int index, boolean reverse, Map<String, Integer> additional) {
        Optional<Integer> res = Optional.empty();
        if (additional != null) {
            res = additional.keySet().stream().filter(digit -> {
                if (!reverse) {
                    return index + 1 >= digit.length() && line.substring(index + 1 - digit.length(), index + 1).equals(digit);
                } else {
                    return index - 1 + digit.length() < line.length() && line.substring(index, index - 1 + digit.length() + 1).equals(digit);
                }
            }).map(digit -> additional.get(digit)).findFirst();
        }
        if (!res.isPresent() && Character.isDigit(line.charAt(index))) {
            res = Optional.of(Integer.valueOf(String.valueOf(line.charAt(index))));
        }
        return res;
    }

    Integer DualSearch(List<String> maplist, Map<String, Integer> additional) {
        return maplist.stream().map(line -> {
            StringBuilder val = new StringBuilder("");
            IntStream.range(0, line.length()).takeWhile(i -> {
                Optional<Integer> digitVal = isDigit(line, i, false, additional);
                if (digitVal.isPresent()) {
                    val.append(digitVal.get());
                    for (int j = line.length() - 1; j >= 0; j--) {
                        digitVal = isDigit(line, j, true, additional);
                        if (digitVal.isPresent()) {
                            val.append(digitVal.get());
                            return false; // Break
                        }
                    }
                    return false; // Break
                }
                return true;
            }).toArray(); // Triggers the lazy computation
            return val.toString();
        }).filter(val -> val.length() == 2).map(Integer::parseInt).reduce(0, Integer::sum);
    }

    public void Solution1(BufferedReader input) throws IOException {
        maplist = input.lines().filter(line -> line.length() > 1).toList();
        System.out.println("Solution 1: " + DualSearch(maplist, null));
    }

    public void Solution2(BufferedReader input) throws IOException {
        System.out.println("Solution 2: " + DualSearch(maplist,
                new HashMap<>(Map.of("zero", 0, "one", 1, "two", 2, "three", 3, "four", 4, "five", 5, "six", 6, "seven", 7, "eight", 8, "nine", 9))));
    }

    public static void main(String[] args) throws Exception {
        ValueCalibration solution = new ValueCalibration();
        solution.Solution1(new BufferedReader(new FileReader("Day01/input.txt")));
        solution.Solution2(new BufferedReader(new FileReader("Day01/input.txt")));
    }
}
