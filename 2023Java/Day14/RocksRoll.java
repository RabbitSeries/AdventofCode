package Day14;

import java.util.*;
import java.util.stream.Collectors;
import java.io.*;

import JavaDataModel.*;

public class RocksRoll {
    List<List<Character>> Platform;
    HashMap<String, Integer> memo;
    List<Integer> resMemoList;
    int row = 0, col = 0;

    int getRes() {
        int res = 0;
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (Platform.get(i).get(j).equals('O')) {
                    res += row - i;
                }
            }
        }
        return res;
    }

    int[] dx = new int[] {-1, 0, 1, 0};
    int[] dy = new int[] {0, -1, 0, 1};

    String getHash() {
        return Platform.stream()
                .map(list -> list.stream().map(c -> c.toString()).collect(Collectors.joining()))
                .collect(Collectors.joining());
    }

    Pair<Integer, Integer> tilt(int times) {
        for (int step = 0; step < 4; step++) {
            for (int i = (step < 2 ? 0 : row - 1); (step < 2 ? i < row : i >= 0); i +=
                    (step < 2 ? 1 : -1)) {
                for (int j = (step < 2 ? 0 : col - 1); (step < 2 ? j < col : j >= 0); j +=
                        (step < 2 ? 1 : -1)) {
                    if (Platform.get(i).get(j).equals('O')) {
                        Point2D curPos = new Point2D(i, j);
                        Platform.get(curPos.first).set(curPos.second, '.');
                        Point2D nextPos =
                                new Point2D(curPos.first + dx[step], curPos.second + dy[step]);
                        while (Point2D.isValid(row, col, nextPos)
                                && Platform.get(nextPos.first).get(nextPos.second).equals('.')) {
                            curPos = nextPos;
                            nextPos =
                                    new Point2D(curPos.first + dx[step], curPos.second + dy[step]);
                        }
                        Platform.get(curPos.first).set(curPos.second, 'O');
                    }
                }
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

    void printMap() {
        for (var row : Platform) {
            System.out.println(row.stream().map(c -> c.toString()).collect(Collectors.joining()));
        }
        System.out.println();
    }

    void readFile() throws IOException {
        Platform = new ArrayList<>();
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        String buf;
        while ((buf = input.readLine()) != null) {
            Platform.add(buf.chars().mapToObj(c -> (char) c)
                    .collect(Collectors.toCollection(ArrayList::new)));
        }
        input.close();
        row = Platform.size();
        col = Platform.get(0).size();
        memo = new HashMap<>();
        resMemoList = new ArrayList<>();
    }

    void Solution1() throws IOException {
        readFile();
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (Platform.get(i).get(j).equals('O')) {
                    Point2D curPos = new Point2D(i, j);
                    Platform.get(curPos.first).set(curPos.second, '.');
                    Point2D nextPos = new Point2D(curPos.first + dx[0], curPos.second + dy[0]);
                    while (Point2D.isValid(row, col, nextPos)
                            && Platform.get(nextPos.first).get(nextPos.second).equals('.')) {
                        curPos = nextPos;
                        nextPos = new Point2D(curPos.first + dx[0], curPos.second + dy[0]);
                    }
                    Platform.get(curPos.first).set(curPos.second, 'O');
                }
            }
        }
        System.out.println("Solution 1: " + getRes());
    }

    void Solution2() throws IOException {
        readFile();
        final int target = 1000000000;
        for (int i = 0; i < target; i++) {
            var cycleInfo = tilt(i);
            if (cycleInfo.first.compareTo(0) >= 0) {
                int cycle = cycleInfo.second - cycleInfo.first,
                        leftOver = target - cycleInfo.second - 1;
                int targetTilt = leftOver % cycle;
                // for (int j = 0; j < targetTilt; j++) {
                // i++;
                // tilt(i);
                // }
                System.out.println("Memoization found at preprocessed: " + cycleInfo.first);
                System.out.println("Saved " + leftOver + targetTilt);
                System.out.println("Solution 2: " + resMemoList.get(cycleInfo.first + targetTilt));
                return;
            }
        }
        System.out.println("Solution 2: " + getRes());
    }

    public static void main(String[] args) throws IOException {
        RocksRoll Solution = new RocksRoll();
        Solution.Solution1();
        Solution.Solution2();
    }
}
