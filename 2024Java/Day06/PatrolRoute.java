package Day06;

import java.io.*;
import java.util.Map.Entry;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Collectors;
import java.util.*;
import java.util.AbstractMap.SimpleEntry;

import JavaDataModel.*;

public class PatrolRoute {

    static Character[] face = new Character[] {'^', '>', 'v', '<'};
    static HashMap<Character, Integer> faceId =
            new HashMap<>(Map.of('^', 0, '>', 1, 'v', 2, '<', 3));
    static int[] dx = new int[] {-1, 0, 1, 0};
    static int[] dy = new int[] {0, 1, 0, -1};
    Point2D guardPos;

    List<List<Character>> readFile() throws IOException {
        List<List<Character>> routeMap = new ArrayList<>();
        String linebuf;
        BufferedReader input = new BufferedReader(new FileReader("./Day06/input.txt"));
        while ((linebuf = input.readLine()) != null) {
            List<Character> row = linebuf.chars().mapToObj(l -> (char) l).collect(ArrayList::new,
                    ArrayList::add, ArrayList::addAll);
            if (!linebuf.isEmpty() && linebuf.matches(".*[\\^>v<].*")) {
                guardPos = new Point2D(routeMap.size(), linebuf.split("[\\^>v<]")[0].length());
            }
            if (!row.isEmpty()) {
                routeMap.add(row);
            }
        }
        input.close();
        return routeMap;
    }

    void patrol(Point2D curPos, List<List<Character>> routeMap) {
        int rows = routeMap.size(), cols = routeMap.get(0).size();
        int curFace;
        while (Point2D.isValid(rows, cols, curPos)) {
            // Determine the face
            curFace = faceId.get(routeMap.get(curPos.getKey()).get(curPos.getValue()));
            Point2D nextPos =
                    new Point2D(dx[curFace] + curPos.getKey(), dy[curFace] + curPos.getValue());
            if (Point2D.isValid(rows, cols, nextPos)) {
                if (routeMap.get(nextPos.getKey()).get(nextPos.getValue()).equals('#')) {
                    curFace = (curFace + 1) % 4;
                    routeMap.get(curPos.getKey()).set(curPos.getValue(), face[curFace]);
                } else {
                    routeMap.get(curPos.getKey()).set(curPos.getValue(), 'X');
                    curPos = nextPos.extractPosPoint2D();
                    routeMap.get(nextPos.getKey()).set(nextPos.getValue(), face[curFace]);
                }
            } else {
                routeMap.get(curPos.getKey()).set(curPos.getValue(), 'X');
                return;
            }
        }
    }

    void Solution1() throws IOException {
        List<List<Character>> routeMap = readFile();
        patrol(guardPos, routeMap);
        int distinctPosCnt = 0;
        for (List<Character> row : routeMap) {
            for (char c : row) {
                // cout << c;
                if (c == 'X') {
                    distinctPosCnt++;
                }
            }
            // cout << endl;
        }
        System.out.println("Solution 1: " + distinctPosCnt);

    }

    // Thus reduced the copy time consumption.
    boolean isPatrolCircle(Point2D curPos, List<List<Character>> routeMap, int rows, int cols) {
        boolean isCircle = false;
        // HashMap<Point2D, Entry<Boolean, Character>> passed = new HashMap<>();
        List<List<Entry<Boolean, Character>>> passed = routeMap.stream()
                .map(line -> line.stream()
                        .map(cell -> (Entry<Boolean, Character>) new SimpleEntry<>(false, '.'))
                        .collect(Collectors.toList()))
                .collect(Collectors.toList());
        // int times = 0;
        int curFace = faceId.get(routeMap.get(curPos.getKey()).get(curPos.getValue()));
        Entry<Boolean, Character> curStatus;
        while (Point2D.isValid(rows, cols, curPos)) {
            // times++;
            curStatus = passed.get(curPos.getKey()).get(curPos.getValue());
            if (curStatus.getKey() && curStatus.getValue().equals(face[curFace])) {
                return true;
            }
            // Determine the face
            Point2D nextPos =
                    new Point2D(dx[curFace] + curPos.getKey(), dy[curFace] + curPos.getValue());
            if (Point2D.isValid(rows, cols, nextPos)) {
                passed.get(curPos.getKey()).set(curPos.getValue(),
                        new SimpleEntry<>(true, face[curFace]));
                if (routeMap.get(nextPos.getKey()).get(nextPos.getValue()).equals('#')) {
                    curFace = (curFace + 1) % 4;
                } else {
                    // routeMap[i--][j] = '|';
                    curPos = nextPos.extractPosPoint2D();
                }
            } else {
                return false;
            }
        }
        return isCircle;
    }

    int threadTask(int startRow, int endRow, Point2D guardPos, List<List<Character>> routeMap,
            AtomicInteger count) {
        int distinctPlacementLocal = 0;
        for (int i = startRow; i < endRow; i++) {
            for (int j = 0; j < routeMap.get(i).size(); j++) {
                // count.compareAndSet(count.get(), count.get() + 1);
                count.incrementAndGet();
                if (i == (int) guardPos.getKey() && j == (int) guardPos.getValue()
                        || routeMap.get(i).get(j).equals('#')) {
                    continue;
                }
                Character curCell = routeMap.get(i).get(j);
                routeMap.get(i).set(j, '#');
                if (isPatrolCircle(guardPos, routeMap, routeMap.size(), routeMap.get(0).size())) {
                    distinctPlacementLocal++;
                }
                routeMap.get(i).set(j, curCell);
            }
        }
        return distinctPlacementLocal;
    }

    void Solution2() throws IOException, InterruptedException, ExecutionException {
        List<List<Character>> routeMap = readFile();

        int numThreads = Runtime.getRuntime().availableProcessors();
        ExecutorService executor = Executors.newFixedThreadPool(numThreads);
        int rowsPerThread = routeMap.size() / numThreads;
        AtomicInteger count = new AtomicInteger(0);

        List<Future<Integer>> futures = new ArrayList<>();
        for (int t = 0; t < numThreads; t++) {
            int startRow = t * rowsPerThread;
            int endRow = (t == numThreads - 1) ? routeMap.size() : (t + 1) * rowsPerThread;
            // User async to start mission.
            futures.add(executor.submit(() -> {
                List<List<Character>> threadMapList = new ArrayList<>();
                threadMapList.addAll(routeMap);
                return threadTask(startRow, endRow, guardPos, routeMap, count);
            }));
        }
        // Java mutex is too complex, and set value is not thread safe.
        // int lastChecked = 0, total = routeMap.size() * routeMap.get(0).size();
        // while (lastChecked < total) {
        // int checked = count.get();
        // if (checked != lastChecked) {
        // lastChecked = checked;
        // System.out.println("\033[2J\033[1;1H");
        // System.out.println("Progress - Part 2");
        // System.out.println(
        // "Positions checked: " + checked + "/" + total + " (" + (checked * 100.0 /
        // total) + "%)");
        // System.out.println("Loop positions found: " + count.get());
        // Thread.sleep(80);
        // }
        // }

        int totalDistinctPlacement = 0;
        // Accumulate all res.
        for (var fut : futures) {
            totalDistinctPlacement += fut.get(); // Wait and get.
        }
        System.out.println("Solution 2: " + totalDistinctPlacement);
        executor.shutdown();
    }

    public static void main(String[] args)
            throws IOException, InterruptedException, ExecutionException {
        var now = System.currentTimeMillis();
        PatrolRoute Day06 = new PatrolRoute();
        Day06.Solution1();
        Day06.Solution2();
        var end = System.currentTimeMillis();
        System.out.println((end - now) / 1000.0 + " seconds");

    }
}
