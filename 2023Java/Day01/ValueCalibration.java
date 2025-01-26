package Day01;

import java.io.*;
import java.util.*;

public class ValueCalibration {
    ArrayList<String> readFile() throws IOException {
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        ArrayList<String> maplist = new ArrayList<>();
        String buf;
        while ((buf = input.readLine()) != null) {
            if (buf.length() > 1)
                maplist.add(buf);
        }
        return maplist;
    }

    Integer res, digitVal;
    HashMap<String, Integer> digits;

    void InitDataModel() {
        res = 0;
        digits = new HashMap<>();
        digits.put("zero", 0);
        digits.put("one", 1);
        digits.put("two", 2);
        digits.put("three", 3);
        digits.put("four", 4);
        digits.put("five", 5);
        digits.put("six", 6);
        digits.put("seven", 7);
        digits.put("eight", 8);
        digits.put("nine", 9);
    }

    void isDigit(String line, int index, boolean reverse) {
        digitVal = -1;

        digits.keySet().stream().forEach((String digit) -> {
            if (!reverse) {
                if (index + 1 >= digit.length()
                        && line.substring(index + 1 - digit.length(), index + 1).equals(digit)) {
                    digitVal = digits.get(digit);
                    return;
                }
            } else {
                if (index - 1 + digit.length() < line.length()
                        && line.substring(index, index - 1 + digit.length() + 1).equals(digit)) {
                    digitVal = digits.get(digit);
                    return;
                }
            }
        });

        if (digitVal == -1 && Character.isDigit(line.charAt(index))) {
            digitVal = Integer.valueOf(String.valueOf(line.charAt(index)));
        }
    }

    void DualSearch(ArrayList<String> maplist) {
        maplist.forEach((String line) -> {
            String val = "";
            for (int i = 0; i < line.length(); i++) {
                isDigit(line, i, false);
                if (digitVal != -1) {
                    val += digitVal.toString();
                    for (int j = line.length() - 1; j >= 0; j--) {
                        isDigit(line, j, true);
                        if (digitVal != -1) {
                            val += digitVal.toString();
                            break;
                        }
                    }
                    break;
                }
            }
            if (val.length() == 2) {
                res += Integer.valueOf(val);
            }
        });
    }

    void Solution1() throws IOException {
        res = 0;
        digits = new HashMap<>();
        ArrayList<String> maplist = readFile();
        DualSearch(maplist);
        System.out.println("Solution 1: " + res);
    }

    void Solution2() throws IOException {
        InitDataModel();
        ArrayList<String> maplist = readFile();
        DualSearch(maplist);
        System.out.println("Solution 2: " + res);
    }

    public static void main(String[] args) throws IOException {
        // BufferedReader input = new BufferedReader(new InputStreamReader(System.in));
        // System.out.println("Hello");
        ValueCalibration Solution = new ValueCalibration();
        Solution.Solution1();
        Solution.Solution2();
    }
}
