package year2023.Day13;

import java.io.*;
import java.util.*;
import java.util.stream.*;

import JavaDataModel.*;

@AoCSolution(day = 13)
public class Reflection implements ISolution {
    List<List<String>> IslandMaps;

    void readFile(BufferedReader input) {
        IslandMaps = Stream.of(input.lines()
                .map(l -> l.concat("\n"))
                .collect(Collectors.joining()).split("\\n\\n"))
                .map(block -> Arrays.asList(block.split("\\n"))).toList();
    }

    int checkHorizontalMirror(List<String> IslandMap, int originalHorizontalLine) {
        int row = IslandMap.size(), col = IslandMap.get(0).length();
        for (int i = 0; i < row - 1; i++) {
            boolean isMirror = true;
            int up = i, below = i + 1, gap = 0;
            for (gap = 0; up - gap >= 0 && below + gap < row; gap++) {
                for (int j = 0; j < col; j++) {
                    if (!(IslandMap.get(up - gap).charAt(j) == IslandMap.get(below + gap).charAt(j))) {
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

    int checkVerticalMirror(List<String> IslandMap, int originalVerticalLine) {
        int row = IslandMap.size(), col = IslandMap.get(0).length();
        for (int i = 0; i < col - 1; i++) {
            boolean isMirror = true;
            int left = i, right = i + 1, gap = 0;
            for (gap = 0; left - gap >= 0 && right + gap < col; gap++) {
                for (int j = 0; j < row; j++) {
                    if (!(IslandMap.get(j).charAt(left - gap) == IslandMap.get(j).charAt(right + gap))) {
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
        readFile(input);
        System.out.println("Solution 1: " + IslandMaps.stream().mapToLong(IslandMap -> {
            int curRes = checkHorizontalMirror(IslandMap, -1);
            if (curRes < 0) {
                curRes = checkVerticalMirror(IslandMap, -1);
                return (curRes < 0 ? 0 : curRes);
            } else {
                return curRes * 100l;
            }
        }).sum());
    }

    public void Solution2(BufferedReader input) throws IOException {
        long res = 0;
        for (var IslandMap : IslandMaps) {
            int row = IslandMap.size(), col = IslandMap.get(0).length();
            int originalHorizontalLine = checkHorizontalMirror(IslandMap, -1),
                    originalVerticalLine = checkVerticalMirror(IslandMap, -1);

            assert (originalHorizontalLine * originalVerticalLine < 0);

            for (int i = 0; i < row; i++) {
                boolean mirrored = false;
                for (int j = 0; j < col; j++) {
                    StringBuilder stringBuilder = new StringBuilder(IslandMap.get(i));
                    char original = IslandMap.get(i).charAt(j);
                    IslandMap.set(i, stringBuilder.replace(j, j + 1, String.valueOf(original == '.' ? '#' : '.')).toString());
                    int curRes = checkHorizontalMirror(IslandMap, originalHorizontalLine);
                    if (curRes > 0) {
                        mirrored = true;
                        res += curRes * 100l;
                        break;
                    }
                    curRes = checkVerticalMirror(IslandMap, originalVerticalLine);
                    IslandMap.set(i, stringBuilder.replace(j, j + 1, String.valueOf(original)).toString());
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
