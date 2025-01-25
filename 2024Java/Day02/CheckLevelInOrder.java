package Day02;

import java.io.*;
import java.util.Vector;

public class CheckLevelInOrder {

    boolean checkDecrease(Vector<Integer> Levels) {
        boolean res = true;
        int i = 0;
        while (i < Levels.size() - 1) {
            if (!(Levels.elementAt(i) > Levels.elementAt(i + 1)
                    && Levels.elementAt(i) - Levels.elementAt(i + 1) <= 3)) {
                return false;
            }
            i++;
        }
        return res;
    }

    boolean checkIncrease(Vector<Integer> Levels) {
        boolean res = true;
        int i = 0;
        while (i < Levels.size() - 1) {
            if (!(Levels.elementAt(i) < Levels.elementAt(i + 1)
                    && Levels.elementAt(i + 1) - Levels.elementAt(i) <= 3)) {
                return false;
            }
            i++;
        }
        return res;
    }

    boolean canBeSafeByRemovingOneLevel(Vector<Integer> Levels) {
        for (int i = 0; i < Levels.size(); i++) {
            Vector<Integer> newLevels = new Vector<>(Levels);
            newLevels.remove(i);
            if (checkIncrease(newLevels) || checkDecrease(newLevels)) {
                return true;
            }
        }
        return false;
    }

    boolean AnalyseOrder(BufferedReader input, boolean enableRemoval) throws IOException {
        String buf;
        if ((buf = input.readLine()) == null) {
            return false;
        }
        String[] lineInput = buf.split("\s");
        Vector<Integer> Levels = new Vector<>();
        for (String tmpLevel : lineInput) {
            Levels.add(Integer.parseInt(tmpLevel));
        }
        if (Levels.isEmpty())
            return false;
        if (!enableRemoval) {
            return checkDecrease(Levels) || checkIncrease(Levels);
        } else {
            return checkDecrease(Levels) || checkIncrease(Levels) || canBeSafeByRemovingOneLevel(Levels);
        }
    }

    public void Solution1() throws IOException {
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        int cnt = 0;
        while (input.ready()) {
            if (AnalyseOrder(input, false)) {
                cnt++;
            }
        }
        System.out.println("Solution 1: " + cnt);
    }

    public void Solution2() throws IOException {
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        int cnt = 0;
        while (input.ready()) {
            if (AnalyseOrder(input, true)) {
                cnt++;
            }
        }
        System.out.println("Solution 2: " + cnt);
    }

    public static void main(String[] args) throws IOException {
        CheckLevelInOrder solution = new CheckLevelInOrder();
        solution.Solution1();
        solution.Solution2();
    }
}