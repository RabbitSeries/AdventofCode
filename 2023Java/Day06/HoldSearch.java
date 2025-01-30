package Day06;

import java.io.*;
import java.util.*;
import java.util.Map.*;
import java.util.AbstractMap.*;
import java.util.stream.*;

public class HoldSearch {
    ArrayList<Entry<Integer, Integer>> recordList;

    void readFile() throws IOException {
        recordList = new ArrayList<>();
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        String buf;
        while ((buf = input.readLine()) != null) {
            if (buf.indexOf("Time:") != -1) {
                for (Integer timeout : Arrays.stream(buf.split(":")[1].trim().split("\\s+"))
                        .map(Integer::parseInt)
                        .collect(Collectors.toCollection(ArrayList::new))) {
                    recordList.add(new SimpleEntry<>(timeout, 0));
                }
            } else if (buf.indexOf("Distance") != -1) {
                int id = 0;
                for (Integer integer : Arrays.stream(buf.split(":")[1].trim().split("\\s+"))
                        .map(token -> {
                            return Integer.parseInt(token);
                        })
                        .collect(ArrayList<Integer>::new, ArrayList<Integer>::add, ArrayList<Integer>::addAll)) {
                    recordList.get(id).setValue(integer);
                    id++;
                }
            }
        }
        input.close();
    }

    void Solution1() throws IOException {
        readFile();
        int targetCnt = 1;
        // targetCnt = (timeout - t) * t for target > distance: (const - t)*t - distance
        // > 0 for t <= timeout
        for (Entry<Integer, Integer> timeout : recordList) {
            int s = 1, t = timeout.getKey() / 2;
            while (s != t) {
                int mid = (s + t) / 2;
                int val = (timeout.getKey() - mid) * mid;
                if (val > timeout.getValue()) {
                    t = mid;
                } else {
                    s = mid + 1;
                }
            }
            if (timeout.getKey() % 2 == 1)
                targetCnt *= (timeout.getKey() / 2 - s + 1) * 2;
            else {
                targetCnt *= (timeout.getKey() / 2 - s + 1) * 2 - 1;
            }
        }
        System.out.println("Solution 1: " + targetCnt);
    }

    void Solution2() throws IOException {
        readFile();
        long realTimeout = Long
                .parseLong(recordList.stream().map(token -> token.getKey().toString()).collect(Collectors.joining()));
        long realDistance = Long
                .parseLong(recordList.stream().map(token -> token.getValue().toString()).collect(Collectors.joining()));
        long targetCnt = 1;

        long s = 1, t = realTimeout / 2;
        while (s != t) {
            long mid = (s + t) / 2;
            long val = (realTimeout - mid) * mid;
            if (val > realDistance) {
                t = mid;
            } else {
                s = mid + 1;
            }
        }
        if (realTimeout % 2 == 1)
            targetCnt *= (realTimeout / 2 - s + 1) * 2;
        else {
            targetCnt *= (realTimeout / 2 - s + 1) * 2 - 1;
        }

        System.out.println("Solution 2: " + targetCnt);
    }

    public static void main(String[] args) throws IOException {
        HoldSearch Solution = new HoldSearch();
        Solution.Solution1();
        Solution.Solution2();
    }
}
