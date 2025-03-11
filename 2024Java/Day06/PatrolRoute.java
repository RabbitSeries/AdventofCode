package Day06;

import java.io.*;
import java.util.concurrent.*;
import java.util.stream.Collectors;
import java.util.*;

import JavaDataModel.*;

//TODO Why is this problem so slow?
public class PatrolRoute {

    static Character[] face = new Character[] {
            '^', '>', 'v', '<'
    };

    static HashMap<Character, Integer> faceId =
            new HashMap<>(Map.of('^', 0, '>', 1, 'v', 2, '<', 3));

    static final int[] dx = new int[] {
            -1, 0, 1, 0
    };

    static final int[] dy = new int[] {
            0, 1, 0, -1
    };

    Point2D guardPos;

    List<List<Character>> routeMap;

    void readFile() throws IOException {
        routeMap = new ArrayList<>();
        String linebuf;
        BufferedReader input = new BufferedReader(new FileReader("./input.txt"));
        while ((linebuf = input.readLine()) != null) {
            List<Character> row = linebuf.chars().mapToObj(l -> (char) l).collect(ArrayList::new, ArrayList::add, ArrayList::addAll);
            if (!linebuf.isEmpty() && linebuf.matches(".*[\\^>v<].*")) {
                guardPos = new Point2D(routeMap.size(), linebuf.split("[\\^>v<]")[0].length());
            }
            if (!row.isEmpty()) {
                routeMap.add(row);
            }
        }
        input.close();
    }

    int patrol(Point2D curPos) {
        int rows = routeMap.size(), cols = routeMap.get(0).size();
        int curFace = faceId.get(routeMap.get(curPos.first).get(curPos.second));
        HashSet<Point2D> resSet = new HashSet<>(Set.of(curPos));
        while (Point2D.isValid(rows, cols, curPos)) {
            // Determine the face
            Point2D nextPos = new Point2D(dx[curFace] + curPos.first, dy[curFace] + curPos.second);
            if (Point2D.isValid(rows, cols, nextPos)) {
                if (routeMap.get(nextPos.first).get(nextPos.second).equals('#')) {
                    curFace = (curFace + 1) % 4;
                } else {
                    curPos = nextPos;
                    resSet.add(curPos);
                }
            } else
                break;
        }
        return resSet.size();
    }

    void Solution1() throws IOException {
        readFile();
        System.out.println("Solution 1: " + patrol(guardPos));
    }

    // Thus reduced the copy time consumption.
    boolean isPatrolCircle(Point2D curPos, int rows, int cols, List<List<Character>> curMap) {
        int curFace = faceId.get(curMap.get(curPos.first).get(curPos.second));
        HashSet<Pair<Point2D, Integer>> visitedHashSet = new HashSet<>();
        while (Point2D.isValid(rows, cols, curPos)) {
            Pair<Point2D, Integer> curStatus = new Pair<>(curPos, curFace);
            if (visitedHashSet.contains(curStatus)) {
                return true;
            }
            visitedHashSet.add(curStatus);
            Point2D nextPos = new Point2D(dx[curFace] + curPos.first, dy[curFace] + curPos.second);
            if (Point2D.isValid(rows, cols, nextPos)) {
                if (curMap.get(nextPos.first).get(nextPos.second).equals('#')) {
                    curFace = (curFace + 1) % 4;
                } else {
                    curPos = nextPos;
                }
            } else {
                break;
            }
        }
        return false;
    }

    int threadTask(int startRow, int endRow, Point2D guardPos, List<List<Character>> curMap) {
        int distinctPlacementLocal = 0;
        for (int i = startRow; i < endRow; i++) {
            for (int j = 0; j < curMap.get(i).size(); j++) {
                // count.compareAndSet(count.get(), count.get() + 1);
                // count.incrementAndGet();
                if (i == (int) guardPos.first && j == (int) guardPos.second || curMap.get(i).get(j).equals('#')) {
                    continue;
                }
                curMap.get(i).set(j, '#');
                if (isPatrolCircle(guardPos, curMap.size(), curMap.get(0).size(), curMap)) {
                    distinctPlacementLocal++;
                }
                curMap.get(i).set(j, '.');
            }
        }
        return distinctPlacementLocal;
    }

    void Solution2()
        throws IOException, InterruptedException, ExecutionException {
        // int numThreads = 1;
        int numThreads = Runtime.getRuntime().availableProcessors();
        ExecutorService executor = Executors.newFixedThreadPool(numThreads);
        int rowsPerThread = routeMap.size() / numThreads;
        // AtomicInteger count = new AtomicInteger(0);
        List<Future<Integer>> futures = new ArrayList<>();
        for (int t = 0; t < numThreads; t++) {
            // User async to start mission.
            int startRow = t * rowsPerThread;
            int endRow = (t == numThreads - 1) ? routeMap.size() : (t + 1) * rowsPerThread;
            futures.add(executor.submit(() -> {
                List<List<Character>> curMap = routeMap.stream().map(row -> (List<Character>) new ArrayList<>(row.stream().map(c -> (char) c)
                        .collect(Collectors.toCollection(ArrayList::new)))).toList();
                return threadTask(startRow, endRow, guardPos, curMap);
            }));
        }
        int totalDistinctPlacement = 0;
        // Accumulate all res.
        for (var fut : futures) {
            totalDistinctPlacement += fut.get(); // Wait and get.
        }
        System.out.println("Solution 2: " + totalDistinctPlacement);
        executor.shutdown();
    }

    public static void main(String[] args) throws IOException, InterruptedException, ExecutionException {
        var now = System.currentTimeMillis();
        PatrolRoute Day06 = new PatrolRoute();
        Day06.Solution1();
        Day06.Solution2();
        var end = System.currentTimeMillis();
        System.out.println((end - now) / 1000.0 + " seconds");

    }
}