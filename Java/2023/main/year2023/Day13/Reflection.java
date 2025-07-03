package year2023.Day13;

import java.io.*;
import java.util.*;
import java.util.stream.*;

import JavaDataModel.*;

@AoCSolution()
public class Reflection implements SolutionBase {

    List<List<List<Character>>> readFile(BufferedReader input) throws IOException {
        List<List<List<Character>>> IslandMaps = new ArrayList<>();
        String buf;
        while ((buf = input.readLine()) != null) {
            if (IslandMaps.isEmpty()) {
                IslandMaps.add(new ArrayList<>());
            }
            if (!buf.isEmpty()) {
                // IslandMaps.getLast().add(buf.trim().chars().mapToObj(c -> (char)
                // c).toList());
                // immutable list
                IslandMaps.getLast().add(buf.trim().chars().mapToObj(c -> (char) c)
                        .collect(Collectors.toCollection(ArrayList::new)));
            } else {
                IslandMaps.add(new ArrayList<>());
            }
        }
        input.close();
        return IslandMaps;
    }

    int checkHorizontalMirror(List<List<Character>> IslandMap, int originalHorizontalLine) {
        int row = IslandMap.size(), col = IslandMap.get(0).size();
        for (int i = 0; i < row - 1; i++) {
            boolean isMirror = true;
            int up = i, below = i + 1, gap = 0;
            for (gap = 0; up - gap >= 0 && below + gap < row; gap++) {
                for (int j = 0; j < col; j++) {
                    if (!IslandMap.get(up - gap).get(j).equals(IslandMap.get(below + gap).get(j))) {
                        isMirror = false;
                        break;
                    }
                }
                if (!isMirror) {
                    break;
                } else if (isMirror && (gap == up || below + gap == row - 1)
                        && ((i + 1) != originalHorizontalLine)) {
                    return i + 1;
                }
            }
        }
        return -1;
    }

    int checkVerticalMirror(List<List<Character>> IslandMap, int originalVerticalLine) {
        int row = IslandMap.size(), col = IslandMap.get(0).size();
        for (int i = 0; i < col - 1; i++) {
            boolean isMirror = true;
            int left = i, right = i + 1, gap = 0;
            for (gap = 0; left - gap >= 0 && right + gap < col; gap++) {
                for (int j = 0; j < row; j++) {
                    if (!IslandMap.get(j).get(left - gap)
                            .equals(IslandMap.get(j).get(right + gap))) {
                        isMirror = false;
                        break;
                    }
                }
                if (!isMirror) {
                    break;
                } else if (isMirror && (gap == left || right + gap == col - 1)
                        && ((i + 1) != originalVerticalLine)) {
                    return i + 1;
                }
            }
        }
        return -1;
    }

    public void Solution1(BufferedReader input) throws IOException {
        List<List<List<Character>>> IslandMaps = readFile(input);
        long res = 0;
        for (var IslandMap : IslandMaps) {
            int curRes = checkHorizontalMirror(IslandMap, -1);
            if (curRes < 0) {
                curRes = checkVerticalMirror(IslandMap, -1);
                res += (curRes < 0 ? 0 : curRes);
            } else {
                res += curRes * 100l;
            }
        }
        System.out.println("Solution 1: " + res);
    }

    public void Solution2(BufferedReader input) throws IOException {
        List<List<List<Character>>> IslandMaps = readFile(input);
        long res = 0;
        for (var IslandMap : IslandMaps) {
            int row = IslandMap.size(), col = IslandMap.get(0).size();
            int originalHorizontalLine = checkHorizontalMirror(IslandMap, -1),
                    originalVerticalLine = checkVerticalMirror(IslandMap, -1);
            assert (originalHorizontalLine * originalVerticalLine < 0);

            for (int i = 0; i < row; i++) {
                boolean mirrored = false;
                for (int j = 0; j < col; j++) {
                    char original = IslandMap.get(i).get(j);
                    IslandMap.get(i).set(j, (original == '.' ? '#' : '.'));
                    int curRes = checkHorizontalMirror(IslandMap, originalHorizontalLine);
                    if (curRes > 0) {
                        mirrored = true;
                        res += curRes * 100l;
                        break;
                    }
                    curRes = checkVerticalMirror(IslandMap, originalVerticalLine);
                    IslandMap.get(i).set(j, original);
                    if (curRes > 0) {
                        mirrored = true;
                        res += curRes;
                        break;
                    }
                }
                if (mirrored) {
                    break;
                }
            }
        }
        System.out.println("Solution 2: " + res);
    }

    public static void main(String[] args) throws IOException {
        Reflection Day13 = new Reflection();
        Day13.Solution1(new BufferedReader(new FileReader("Day13/input.txt")));
        Day13.Solution2(new BufferedReader(new FileReader("Day13/input.txt")));
    }
}
