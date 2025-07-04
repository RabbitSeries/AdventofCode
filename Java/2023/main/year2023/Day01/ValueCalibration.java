package year2023.Day01;

import java.io.*;
import java.util.*;

import JavaDataModel.AoCSolution;
import JavaDataModel.*;

@AoCSolution()
public class ValueCalibration implements SolutionBase {
    ArrayList<String> readFile(BufferedReader input) throws IOException {
        ArrayList<String> maplist = new ArrayList<>();
        String buf;
        while ((buf = input.readLine()) != null) {
            if (buf.length() > 1)
                maplist.add(buf);
        }
        input.close();
        return maplist;
    }

    Integer res, digitVal;

    HashMap<String, Integer> digits;

    void InitDataModel() {
        res = 0;
        digits = new HashMap<>(Map.of("zero", 0, "one", 1, "two", 2, "three", 3, "four", 4, "five", 5, "six", 6, "seven", 7, "eight", 8, "nine", 9));
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

    public void Solution1(BufferedReader input) throws IOException {
        res = 0;
        digits = new HashMap<>();
        ArrayList<String> maplist = readFile(input);
        DualSearch(maplist);
        System.out.println("Solution 1: " + res);
    }

    public void Solution2(BufferedReader input) throws IOException {
        InitDataModel();
        ArrayList<String> maplist = readFile(input);
        DualSearch(maplist);
        System.out.println("Solution 2: " + res);
    }
}
