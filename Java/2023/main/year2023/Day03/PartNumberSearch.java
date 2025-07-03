package year2023.Day03;

import java.io.*;
import java.util.*;
import java.util.Map.Entry;
import JavaDataModel.*;

@AoCSolution()
public class PartNumberSearch implements SolutionBase {
    ArrayList<ArrayList<Character>> schematic;

    ArrayList<Map.Entry<Point2D, Integer>> partNumberPos;

    ArrayList<Character> row;

    ArrayList<Point2D> gearPos;

    boolean startOfDigit = false;

    int digitSize = 0;

    String catString = "";

    Integer res = 0;

    Integer ratioSum = 0;

    void readFile(BufferedReader input) throws IOException {
        schematic = new ArrayList<>();
        partNumberPos = new ArrayList<>();
        gearPos = new ArrayList<>();
        String buf;
        while ((buf = input.readLine()) != null && buf.length() > 0) {
            startOfDigit = false;
            row = new ArrayList<>();
            buf.chars().forEach(ascii -> {
                Character ch = (char) ascii;
                if (Character.isDigit(ch)) {
                    if (!startOfDigit) {
                        startOfDigit = true;
                        digitSize = 1;
                        partNumberPos.add(new AbstractMap.SimpleEntry<Point2D, Integer>(
                                new Point2D(schematic.size(), row.size()), 1));
                    } else {
                        digitSize++;
                        partNumberPos.getLast().setValue(digitSize);
                    }
                } else {
                    if (ch == '*') {
                        gearPos.add(new Point2D(schematic.size(), row.size()));
                    }
                    if (startOfDigit) {
                        startOfDigit = false;
                    }
                }
                row.add((char) ascii);
            });
            schematic.add(row);
        }
        input.close();
    }

    public void Solution1(BufferedReader input) throws IOException {
        readFile(input);
        Integer rows = schematic.size(), cols = schematic.get(0).size();
        for (Entry<Point2D, Integer> posInfo : partNumberPos) {
            Point2D pos = posInfo.getKey();
            Integer numLen = posInfo.getValue();
            catString = "";
            schematic.get(pos.getKey()).subList(pos.getValue(), pos.getValue() + numLen)
                    .forEach(num -> catString += num);
            Integer num = Integer.parseInt(catString);
            boolean added = false;
            for (int i = 0; i < numLen; i++) {
                for (int direction = 0; direction < 8; direction++) {
                    Point2D nextPos = new Point2D(pos.getKey() + Point2D.dx[direction],
                            pos.getValue() + i + Point2D.dy[direction]);
                    if (Point2D.isValid(rows, cols, nextPos)) {
                        char ch = schematic.get(nextPos.getKey()).get(nextPos.getValue());
                        if (!Character.isDigit(ch) && ch != '.') {
                            res += num;
                            added = true;
                            break;
                        }
                    }
                }
                if (added) {
                    break;
                }
            }
            if (!added) {
                // System.out.println(num + " is not added");
            }
        }
        System.out.println("Solution 1: " + res);
    }

    Integer searchNumber(int rows, int cols, Point2D pos, HashMap<Point2D, Boolean> visit) {
        int startIndex = pos.getValue(), endIndex = pos.getValue(), x = pos.getKey();
        visit.put(pos, true);
        Point2D nextPos = new Point2D(x, startIndex - 1);
        while (Point2D.isValid(rows, cols, nextPos)
                && Character.isDigit(schematic.get(x).get(startIndex - 1))) {
            if (visit.containsKey(nextPos)) {
                visit.put(nextPos, true);
            }
            startIndex--;
            nextPos = new Point2D(x, startIndex - 1);
        }
        nextPos = new Point2D(x, endIndex + 1);
        while (Point2D.isValid(rows, cols, nextPos)
                && Character.isDigit(schematic.get(x).get(endIndex + 1))) {
            if (visit.containsKey(nextPos)) {
                visit.put(nextPos, true);
            }
            endIndex++;
            nextPos = new Point2D(x, endIndex + 1);
        }
        catString = "";
        schematic.get(x).subList(startIndex, endIndex + 1).forEach(digit -> {
            catString += String.valueOf((char) digit);
        });
        return Integer.parseInt(catString);
    }

    public void Solution2(BufferedReader input) throws IOException {
        readFile(input);
        Integer rows = schematic.size(), cols = schematic.get(0).size();
        for (Point2D pos : gearPos) {
            int numberCnt = 0;
            Integer[] numPoint2D = new Integer[2];
            HashMap<Point2D, Boolean> visit = new HashMap<>();

            for (int i = 0; i < 8; i++) {
                Point2D nextPos =
                        new Point2D(pos.getKey() + Point2D.dx[i], pos.getValue() + Point2D.dy[i]);
                if (Point2D.isValid(rows, cols, nextPos))
                    visit.put(nextPos, false);
            }

            for (int i = 0; i < 8; i++) {
                Point2D nextPos =
                        new Point2D(pos.getKey() + Point2D.dx[i], pos.getValue() + Point2D.dy[i]);
                if (Point2D.isValid(rows, cols, nextPos) && Character
                        .isDigit(schematic.get(nextPos.getKey()).get(nextPos.getValue()))) {
                    try {
                        if (!visit.get(nextPos)) {
                            if (numberCnt < 2) {
                                numPoint2D[numberCnt++] = searchNumber(rows, cols, nextPos, visit);
                            } else {
                                numberCnt++;
                                break;
                            }
                        }
                    } catch (Exception e) {
                        System.out.println(visit.toString());
                        e.printStackTrace();
                    }
                }
            }
            if (numberCnt == 2) {
                ratioSum += numPoint2D[0] * numPoint2D[1];
            }
        }
        System.out.println("Solution 2: " + ratioSum);
    }
}
