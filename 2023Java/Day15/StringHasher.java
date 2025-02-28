package Day15;

import java.util.*;
import java.util.Map.Entry;
import java.util.stream.Collectors;

import DataModel.Pair;

import java.io.*;

public class StringHasher {
    List<String> stepList;

    void readFile() throws IOException {
        stepList = new ArrayList<>();
        String buf;
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        while ((buf = input.readLine()) != null) {
            stepList.addAll(Arrays.stream(buf.trim().split(",")).toList());
        }
        input.close();
    }

    int hash(String s) {
        return s.chars().reduce(0, (init, c) -> (init + c) * 17 % 256);
    }

    String extractLabel(String s) {
        return s.split("[=-]")[0];
    }

    void Solution1() throws IOException {
        readFile();
        long res = 0;
        for (var s : stepList)
            res += hash(s);
        System.out.println("Solution 1: " + res);
    }

    void PrintRes(HashMap<String, Pair<Integer, Integer>> LensPosList) {
        List<List<Entry<String, Pair<Integer, Integer>>>> output = LensPosList.entrySet().stream()
                .collect(Collectors.groupingBy(l -> hash(l.getKey()))).entrySet().stream()
                .sorted((e1, e2) -> Integer.compare(e1.getKey(), e2.getKey())).map(entry -> entry.getValue()).toList();
        for (int curBox = 0; curBox < output.size(); curBox++) {
            List<Entry<String, Pair<Integer, Integer>>> curPrintBox = output.get(curBox);
            System.out.print("Box " + hash(curPrintBox.getFirst().getKey()) + ": ");
            curPrintBox.sort((e1, e2) -> Integer.compare(e1.getValue().first, e2.getValue().first));
            curPrintBox.forEach(
                    e -> System.out.print(e.getValue().first + ": [" + e.getKey() + " " + e.getValue().second + "] "));
            System.out.println();
        }
    }

    void Solution2() throws IOException {
        List<ArrayList<Pair<String, Integer>>> BoxList = Collections
                .nCopies(256, new ArrayList<Pair<String, Integer>>()).stream().map(list -> new ArrayList<>(list))
                .toList();
        HashMap<String, Pair<Integer, Integer>> LensPosList = new HashMap<>();// Label -> Slot,focalLength
        for (var s : stepList) {
            String label = extractLabel(s);
            List<Pair<String, Integer>> SlotList = BoxList.get(hash(label));
            if (s.indexOf("=") != -1) {
                int focalLength = Integer.parseInt(s.substring(s.indexOf("=") + 1));
                if (LensPosList.containsKey(label)) {
                    if (LensPosList.get(label).second != focalLength) {
                        int whichSlot = LensPosList.get(label).first;
                        SlotList.set(whichSlot, new Pair<>(label, focalLength));
                        LensPosList.put(label, new Pair<>(whichSlot, focalLength));
                    }
                } else {
                    // add
                    LensPosList.put(label, new Pair<>(SlotList.size(), focalLength));
                    SlotList.add(new Pair<>(label, focalLength));
                }
            } else {
                if (LensPosList.containsKey(label)) {
                    int whichSlot = LensPosList.get(label).first;
                    SlotList.remove(whichSlot);
                    LensPosList.remove(label);
                    for (int i = 0; i < SlotList.size(); i++) {
                        LensPosList.put(SlotList.get(i).first, new Pair<Integer, Integer>(i, SlotList.get(i).second));
                    }
                }
            }
            // System.out.println("After " + "\"" + s + "\":");
            // PrintRes(LensPosList);
            // System.out.println();

        }
        System.out.println("Solution 2: " + LensPosList.entrySet().stream()
                .mapToLong(e -> (hash(e.getKey()) + 1) * (e.getValue().first + 1) *
                        e.getValue().second)
                .sum());
    }

    public static void main(String[] args) throws IOException {
        StringHasher Solution = new StringHasher();
        Solution.Solution1();
        Solution.Solution2();
    }
}