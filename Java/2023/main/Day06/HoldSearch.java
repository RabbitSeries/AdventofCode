package Day06;

import java.io.*;
import java.util.*;
import java.util.Map.*;
import java.util.AbstractMap.*;
import java.util.stream.*;

import JavaDataModel.SolutionBase2023;

public class HoldSearch implements SolutionBase2023 {
    ArrayList<Entry<Integer, Integer>> recordList;

    void readFile() throws IOException {
        recordList = new ArrayList<>();
        BufferedReader input = new BufferedReader(new FileReader("Day06/input.txt"));
        String buf;
        while ((buf = input.readLine()) != null) {
            if (buf.indexOf("Time:") != -1) {
                for (Integer timeout : Arrays.stream(buf.split(":")[1].trim().split("\\s+"))
                        .map(Integer::parseInt).toList()) {
                    recordList.add(new SimpleEntry<>(timeout, 0));
                }
            } else if (buf.indexOf("Distance") != -1) {
                int id = 0;
                for (Integer integer : Arrays.stream(buf.split(":")[1].trim().split("\\s+"))
                        .map(token -> {
                            return Integer.parseInt(token);
                        }).toList()) {
                    recordList.get(id).setValue(integer);
                    id++;
                }
            }
        }
        input.close();
    }

    long binarySearch(long timeout, long target) {
        long s = 1, t = timeout / 2;
        while (s != t) {
            long mid = (s + t) / 2;
            long val = (timeout - mid) * mid;
            if (val > target) {
                t = mid;
            } else {
                s = mid + 1;
            }
        }
        if (timeout % 2 == 1)
            return (timeout / 2 - s + 1) * 2;
        else {
            return (timeout / 2 - s + 1) * 2 - 1;
        }
    }

    public void Solution1() throws IOException {
        readFile();
        long targetCnt = 1;
        // targetCnt = (timeout - t) * t for target > distance: (const - t)*t - distance
        // > 0 for t <= timeout
        for (Entry<Integer, Integer> timeout : recordList) {
            targetCnt *= binarySearch(timeout.getKey(), timeout.getValue());
        }
        System.out.println("Solution 1: " + targetCnt);
    }

    public void Solution2() throws IOException {
        readFile();
        long realTimeout = Long.parseLong(recordList.stream()
                .map(token -> token.getKey().toString()).collect(Collectors.joining()));
        long realDistance = Long.parseLong(recordList.stream()
                .map(token -> token.getValue().toString()).collect(Collectors.joining()));
        long targetCnt = 1;

        targetCnt *= binarySearch(realTimeout, realDistance);

        System.out.println("Solution 2: " + targetCnt);
    }
}
