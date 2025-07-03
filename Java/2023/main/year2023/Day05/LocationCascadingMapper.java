package year2023.Day05;

import java.io.*;
import java.lang.Math;
import java.util.AbstractMap.*;
import java.util.*;
import java.util.Map.*;

import JavaDataModel.*;

// TODO If each mapperlist is too large， a BST tree were used.
@AoCSolution()
public class LocationCascadingMapper implements SolutionBase {
    ArrayList<Long> seedID;

    ArrayList<ArrayList<Entry<Entry<Long, Long>, Long>>> mapperList;

    long res;

    Long SearchMap(ArrayList<Entry<Entry<Long, Long>, Long>> mapper, long source) {
        for (Entry<Entry<Long, Long>, Long> rangeInfo : mapper) {
            long range = rangeInfo.getValue(), sourceStarter = rangeInfo.getKey().getValue();
            if (source >= sourceStarter && source < sourceStarter + range) {
                return source - sourceStarter + rangeInfo.getKey().getKey();
            }
        }

        return source;
    }

    void readFile(BufferedReader input) throws IOException {
        mapperList = new ArrayList<>();

        String buf;
        while ((buf = input.readLine()) != null) {
            if (buf.isEmpty()) {
                continue;
            }

            if (buf.indexOf("seeds") != -1) {
                seedID = new ArrayList<>();
                for (String id : buf.split(":")[1].trim().split("\\s+")) {
                    seedID.add(Long.parseLong(id));
                }
                continue;
            }

            if (buf.indexOf("map") != -1) {
                mapperList.add(new ArrayList<>());
                while ((buf = input.readLine()) != null) {
                    if (buf.isEmpty()) {
                        break;
                    }
                    String[] mapInfo = buf.split("\\s+");
                    mapperList.getLast()
                            .add(new SimpleEntry<>(
                                    new SimpleEntry<>(Long.parseLong(mapInfo[0]),
                                            Long.parseLong(mapInfo[1])),
                                    Long.parseLong(mapInfo[2])));
                }
                // mapperList.getLast().sort(Comparator.comparing(entry ->
                // entry.getKey().getValue()));
                continue;
            }
        }
        input.close();
    }

    public void Solution1(BufferedReader input) throws IOException {
        long res = Long.MAX_VALUE;
        readFile(input);
        for (long token : seedID) {
            for (ArrayList<Entry<Entry<Long, Long>, Long>> searchList : mapperList) {
                token = SearchMap(searchList, token);
            }
            res = Math.min(token, res);
        }
        System.out.println("Solution 1: " + res);
    }

    class Interval {
        Interval(long _s, long _t) {
            s = _s;
            t = _t;
        }

        public long s;

        public long t;
    }

    ArrayList<Interval> SearchInterval(ArrayList<Entry<Entry<Long, Long>, Long>> IntervalMapList,
        Interval itv) {
        // If not sort get error, don't know why yet.
        // IntervalMapList.sort(Comparator.comparing(entry ->
        // entry.getKey().getValue()));
        // IntervalMapList
        // .forEach(entry -> {
        // System.out.print(entry);
        // System.out.print(" ");
        // System.out.println(entry.getKey().getValue() + entry.getValue() - 1);
        // });

        ArrayList<Interval> resList = new ArrayList<>();
        Queue<Interval> q = new LinkedList<>();
        q.add(itv);
        while (!q.isEmpty()) {
            Interval curItv = q.poll();
            long itvStart = curItv.s, itvEnd = curItv.t;
            boolean splited = false;

            for (Entry<Entry<Long, Long>, Long> mapRange : IntervalMapList) {

                long mapStart = mapRange.getKey().getValue();
                long mapEnd = mapStart + mapRange.getValue() - 1;
                long mapToStart = mapRange.getKey().getKey();
                long mapToEnd = mapToStart + mapRange.getValue() - 1;

                if (itvStart >= mapStart && itvStart <= mapEnd) {
                    splited = true;
                    if (itvEnd <= mapEnd) {
                        // |-----*-----*---|
                        resList.add(new Interval(itvStart - mapStart + mapToStart,
                                itvEnd - mapStart + mapToStart));
                        return resList;
                    } else {
                        // |-----*--------|---*
                        splited = true;
                        resList.add(new Interval(itvStart - mapStart + mapToStart, mapToEnd));
                        q.add(new Interval(mapEnd + 1, itvEnd));
                        break;
                    }
                } else if (itvStart < mapStart) {
                    if (itvEnd >= mapStart && itvEnd <= mapEnd) {
                        // *----|-----*------|
                        splited = true;
                        resList.add(new Interval(mapToStart, itvEnd - itvStart + mapToStart));
                        q.add(new Interval(itvStart, mapStart - 1));
                        break;
                    } else if (itvEnd > mapEnd) {
                        // *----|------------|----*
                        splited = true;
                        resList.add(new Interval(mapToStart, mapToEnd));
                        q.add(new Interval(itvStart, mapStart - 1));
                        q.add(new Interval(mapEnd + 1, itvEnd));
                        break;
                    }
                }
            }
            if (!splited) {
                resList.add(new Interval(itvStart, itvEnd));
                return resList;
            }
        }
        return resList;
    }

    public void Solution2(BufferedReader input) throws IOException {
        res = Long.MAX_VALUE;
        readFile(input);

        for (int i = 0; i < seedID.size(); i += 2) {

            ArrayList<Interval> curLevelList = new ArrayList<>();

            curLevelList.add(new Interval(seedID.get(i), seedID.get(i) + seedID.get(i + 1) - 1));

            for (var searchList : mapperList) {

                ArrayList<Interval> nextLevelList = new ArrayList<>();
                for (Interval itv : curLevelList) {
                    nextLevelList.addAll(SearchInterval(searchList, itv));
                }
                curLevelList = nextLevelList;
            }

            curLevelList.forEach(itv -> {
                res = Math.min(itv.s, res);
            });
        }

        System.out.println("Solution 2: " + res);
    }

    public static void main(String[] args) throws IOException {
        LocationCascadingMapper Day05 = new LocationCascadingMapper();
        Day05.Solution1(new BufferedReader(new FileReader("Day05/input.txt")));
        Day05.Solution2(new BufferedReader(new FileReader("Day05/input.txt")));
    }
}
