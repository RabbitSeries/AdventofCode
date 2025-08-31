package year2023.Day05;

import java.io.*;
import java.util.*;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import JavaDataModel.*;

@AoCSolution()
public class LocationCascadingMapper implements ISolution {
    List<Long> Seeds;

    List<List<TenaryTuple<Long, Long, Long>>> MapBlocks;

    long SearchMap(long source, int mId) {
        if (mId >= MapBlocks.size()) {
            return source;
        }
        for (var rangeInfo : MapBlocks.get(mId)) {
            long from = rangeInfo.getE1(), to = rangeInfo.getE2(), range = rangeInfo.getE3();
            if (source >= from && source < from + range) {
                return SearchMap(source - from + to, mId + 1);
            }
        }
        return SearchMap(source, mId + 1);
    }

    void readFile(BufferedReader input) throws IOException {
        MapBlocks = new ArrayList<>();
        Stream.of(input.lines().map(l -> l + "\n").reduce("", String::concat).split("\\n\\n")).forEach(block -> {
            if (block.contains("seeds")) {
                Seeds = Stream.of(block.split(":")[1].trim().split("\\s+")).map(Long::parseLong).toList();
            } else {
                MapBlocks.add(Stream.of(block.split("\\n"))
                        .filter(line -> !line.contains("map"))
                        .map(line -> Stream.of(line.split("\\s+")).map(Long::parseLong).toArray(Long[]::new))
                        .map(arr -> new TenaryTuple<>(arr[1], arr[0], arr[2])).toList());
            }
        });
    }

    public void Solution1(BufferedReader input) throws IOException {
        readFile(input);
        System.out.println("Solution 1: " + Seeds.stream()
                .mapToLong(token -> SearchMap(token, 0)).reduce(Long.MAX_VALUE, Long::min));
    }

    List<Pair<Long, Long>> SearchInterval(int mId, List<Pair<Long, Long>> itvList) {
        if (mId >= MapBlocks.size()) {
            return itvList;
        }

        List<Pair<Long, Long>> nextItvList = itvList.stream().flatMap(itv -> {
            List<Pair<Long, Long>> matchedList = new ArrayList<>(), unmatchedList = new ArrayList<>(List.of(itv));
            MapBlocks.get(mId).stream().takeWhile(__ -> !unmatchedList.isEmpty()).forEach(rangeInfo -> {
                long from = rangeInfo.getE1(), to = rangeInfo.getE2(), fromEnd = from + rangeInfo.getE3();
                List<Pair<Long, Long>> processList = new ArrayList<>(unmatchedList);
                unmatchedList.clear();
                for (Pair<Long, Long> curItv : processList) {
                    long itvStart = curItv.getKey(), itvEnd = curItv.getValue();
                    Pair<Long, Long> mapped = new Pair<>(Long.max(itvStart, from), Long.min(itvEnd, fromEnd));
                    if (mapped.getKey() >= mapped.getValue()) {
                        unmatchedList.add(curItv);
                        continue;
                    }
                    matchedList.add(new Pair<>(mapped.getKey() - from + to, mapped.getValue() - from + to));
                    // *----|------------|----*
                    // *----|------*-----|----
                    // -----|------*-----|----*
                    if (itvStart < from) {
                        unmatchedList.add(new Pair<>(itvStart, from));
                    }
                    if (itvEnd > fromEnd) {
                        unmatchedList.add(new Pair<>(fromEnd, itvEnd));
                    }
                }
            });
            matchedList.addAll(unmatchedList);
            return matchedList.stream();
        }).toList();
        return SearchInterval(mId + 1, nextItvList);
    }

    public void Solution2(BufferedReader input) throws IOException {
        System.out.println("Solution 2: " + IntStream.range(0, Seeds.size()).filter(i -> i % 2 == 0).mapToLong(i -> {
            return SearchInterval(0, List.of(new Pair<>(Seeds.get(i), Seeds.get(i) + Seeds.get(i + 1))))
                    .stream().map(Pair::getKey).reduce(Long.MAX_VALUE, Long::min);
        }).reduce(Long.MAX_VALUE, Long::min));
    }

    public static void main(String[] args) throws IOException {
        LocationCascadingMapper Day05 = new LocationCascadingMapper();
        Day05.Solution1(new BufferedReader(new FileReader("Day05/input.txt")));
        Day05.Solution2(new BufferedReader(new FileReader("Day05/input.txt")));
    }
}
