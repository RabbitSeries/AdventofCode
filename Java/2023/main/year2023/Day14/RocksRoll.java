package year2023.Day14;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import JavaDataModel.AoCSolution;
import JavaDataModel.Pair;
import JavaDataModel.Point2D;
import JavaDataModel.ISolution;

@AoCSolution(day = 14)
public class RocksRoll implements ISolution {
    List<StringBuilder> Platform;

    Map<String, Integer> memo = new HashMap<>();

    List<Integer> resMemoList = new ArrayList<>();

    int row = 0, col = 0;

    int getRes() {
        int res = 0;
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (Platform.get(i).charAt(j) == 'O') {
                    res += row - i;
                }
            }
        }
        return res;
    }

    int[] dx = new int[] {
            -1, 0, 1, 0
    };

    int[] dy = new int[] {
            0, -1, 0, 1
    };

    String getHash() {
        return Platform.stream().collect(Collectors.joining());
    }

    Pair<Integer, Integer> tilt(int times, boolean once) {
        for (int step = 0; step < 4; step++) {
            for (int i = (step < 2 ? 0 : row - 1); (step < 2 ? i < row : i >= 0); i +=
                    (step < 2 ? 1 : -1)) {
                for (int j = (step < 2 ? 0 : col - 1); (step < 2 ? j < col : j >= 0); j +=
                        (step < 2 ? 1 : -1)) {
                    if (Platform.get(i).charAt(j) == 'O') {
                        Point2D curPos = new Point2D(i, j);
                        Platform.get(curPos.first).replace(curPos.second, curPos.second + 1, ".");
                        Point2D nextPos = new Point2D(curPos.first + dx[step], curPos.second + dy[step]);
                        while (Point2D.isValid(row, col, nextPos) && Platform.get(nextPos.first).charAt(nextPos.second) == '.') {
                            curPos = nextPos;
                            nextPos = new Point2D(curPos.first + dx[step], curPos.second + dy[step]);
                        }
                        Platform.get(curPos.first).replace(curPos.second, curPos.second + 1, "O");
                    }
                }
            }
            if (once) {
                return null;
            }
        }
        resMemoList.add(getRes());
        String curHash = getHash();
        if (memo.containsKey(curHash)) {
            return new Point2D(memo.get(curHash), times);
        }
        memo.put(curHash, times);
        return new Point2D(-1, times);
    }

    void readFile(BufferedReader input) throws IOException {
        Platform = input.lines().map(StringBuilder::new).toList();
        row = Platform.size();
        col = Platform.get(0).length();
    }

    public void Solution1(BufferedReader input) throws IOException {
        readFile(input);
        tilt(0, true);
        System.out.println("Solution 1: " + getRes());
    }

    public void Solution2(BufferedReader input) throws IOException {
        final int target = 1000000000;
        for (int i = 0; i < target; i++) {
            var cycleInfo = tilt(i, false);
            if (cycleInfo.first.compareTo(0) >= 0) {
                int cycle = cycleInfo.second - cycleInfo.first;
                int leftOver = target - cycleInfo.second - 1;
                int targetTilt = leftOver % cycle;
                System.out.println("Solution 2: " + resMemoList.get(cycleInfo.first + targetTilt));
                return;
            }
        }
        System.out.println("Solution 2: " + getRes());
    }

    public static void main(String[] args) throws IOException {
        RocksRoll Day14 = new RocksRoll();
        Day14.Solution1(new BufferedReader(new FileReader("Day14/input.txt")));
        Day14.Solution2(new BufferedReader(new FileReader("Day14/input.txt")));
    }
}
