package Day11;

import java.io.*;
import java.util.*;
import JavaDataModel.*;

public class CosmicDistance implements SolutionBase2023 {

    List<List<Integer>> CosmicMap;
    List<Pair<Long, Long>> GalaxyList;

    void readFile() throws IOException {
        CosmicMap = new ArrayList<>();
        GalaxyList = new ArrayList<>();
        BufferedReader input = new BufferedReader(new FileReader("Day11/input.txt"));
        String buf;
        // Line Scanning
        while ((buf = input.readLine()) != null) {
            List<Integer> row = new ArrayList<>();
            for (char c : buf.chars().mapToObj(c -> (char) c).toList()) {
                if (c == '#') {
                    row.add(GalaxyList.size());
                    GalaxyList.add(new Pair<>((long) (CosmicMap.size()), (long) (row.size())));
                } else {
                    row.add(-1);
                }
            }
            CosmicMap.add(row);
        }
        input.close();
    }

    long expandCosmic(long expandDistance) {
        int rows = CosmicMap.size(), cols = CosmicMap.get(0).size();
        for (int i = 0; i < rows; i++) {
            boolean lineEmpty = true;
            for (int cell : CosmicMap.get(i)) {
                if (cell >= 0) {
                    lineEmpty = false;
                }
            }
            if (lineEmpty) {
                for (int j = i + 1; j < rows; j++) {
                    for (int k = 0; k < cols; k++) {
                        if (CosmicMap.get(j).get(k).compareTo(0) >= 0) {
                            GalaxyList.get(CosmicMap.get(j).get(k)).first += expandDistance;
                        }
                    }
                }
            }
        }
        for (int i = 0; i < cols; i++) {
            boolean colEmpty = true;
            for (int j = 0; j < rows; j++) {
                if (CosmicMap.get(j).get(i).compareTo(0) >= 0) {
                    colEmpty = false;
                }
            }
            if (colEmpty) {
                for (int j = i + 1; j < cols; j++) {
                    for (int k = 0; k < rows; k++) {
                        if (CosmicMap.get(k).get(j).compareTo(0) >= 0) {
                            GalaxyList.get(CosmicMap.get(k).get(j)).second += expandDistance;
                        }
                    }
                }
            }
        }
        long res = 0, GalaxyCnt = GalaxyList.size();
        for (int i = 0; i < GalaxyCnt - 1; i++) {
            for (int j = i + 1; j < GalaxyCnt; j++) {
                res += ManhattanDistance(GalaxyList.get(i), GalaxyList.get(j));
            }
        }
        return res;
    }

    public void Solution1() throws IOException {
        readFile();
        System.out.println("Solution 1: " + expandCosmic(1));
        return;
    }

    public void Solution2() throws IOException {
        readFile();
        System.out.println("Solution 2: " + expandCosmic(1000000 - 1));
        return;
    }

    long ManhattanDistance(Pair<Long, Long> p1, Pair<Long, Long> p2) {
        return Math.abs(p1.first - p2.first) + Math.abs(p1.second - p2.second);
    }

    public static void main(String[] args) throws IOException {
        CosmicDistance Solution = new CosmicDistance();
        Solution.Solution1();
        Solution.Solution2();
    }
}
