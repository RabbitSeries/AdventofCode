package year2023.Day09;

import java.io.*;
import java.util.*;

import JavaDataModel.*;

@AoCSolution()
public class HistoryExtrapolate implements SolutionBase {
    ArrayList<List<Integer>> HistoryList;

    void readFile() throws IOException {
        HistoryList = new ArrayList<>();
        BufferedReader input = new BufferedReader(new FileReader("Day09/input.txt"));
        String buf;
        while ((buf = input.readLine()) != null) {
            HistoryList.add(
                    Arrays.stream(buf.trim().split("\\s+")).map(l -> Integer.parseInt(l)).toList());
        }
        input.close();
    }

    int predict(List<Integer> histroyList, IntegerRef addUp, boolean isForward) {
        if (histroyList.stream().allMatch(l -> Integer.compare(l, 0) == 0)) {
            return 0;
        }
        ArrayList<Integer> nextLevelList = new ArrayList<>();

        for (int i = 1; i < histroyList.size(); i++) {
            nextLevelList.add(histroyList.get(i) - histroyList.get(i - 1));
        }
        int extrapolate = (isForward ? histroyList.getLast() : histroyList.getFirst())
                + (isForward ? 1 : -1) * predict(nextLevelList, addUp, isForward);
        addUp.setVal(addUp.getVal() + extrapolate);
        return extrapolate;
    }

    public void Solution1() throws IOException {
        readFile();
        long res = 0;
        for (List<Integer> historyList : HistoryList) {
            IntegerRef addUp = new IntegerRef(0);

            res += predict(historyList, addUp, true);
        }
        System.out.println("Solution 1: " + res);
    }

    public void Solution2() throws IOException {
        readFile();
        long res = 0;
        for (List<Integer> historyList : HistoryList) {
            IntegerRef addUp = new IntegerRef(0);
            res += predict(historyList, addUp, false);
        }
        System.out.println("Solution 2: " + res);
    }

    public static void main(String[] args) throws IOException {
        HistoryExtrapolate Day09 = new HistoryExtrapolate();
        Day09.Solution1();
        Day09.Solution2();
    }

    static class IntegerRef {
        Integer val;

        IntegerRef(int v) {
            val = v;
        }

        public Integer getVal() {
            return this.val;
        }

        public void setVal(Integer val) {
            this.val = val;
        }

    }
}
