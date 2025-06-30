package year2023.Day04;

import java.io.*;
import java.util.*;

import JavaDataModel.*;

@AoCSolution()
public class CardPoints implements SolutionBase {
    HashSet<Integer> winningNumbers;

    public void Solution1() throws IOException {
        Integer res = 0;
        BufferedReader input = new BufferedReader(new FileReader("Day04/input.txt"));
        String buf;
        while ((buf = input.readLine()) != null) {
            winningNumbers = new HashSet<>();

            String[] cards = buf.split(":")[1].split("\\|");
            for (String split : cards[0].trim().split("\\s+")) {
                winningNumbers.add(Integer.parseInt(split));
            }
            Integer point = 0;
            for (String split : cards[1].trim().split("\\s+")) {
                Integer holdingCard = Integer.parseInt(split);
                if (winningNumbers.contains(holdingCard)) {
                    if (point.equals(0)) {
                        point = 1;
                    } else {
                        point *= 2;
                    }
                }
            }
            res += point;
        }
        System.out.println("Solution 1: " + res);
        input.close();
    }

    public void Solution2() throws IOException {
        Integer res = 0, currentBonus, currentLine = 0;
        ArrayList<Integer> bonusRecord = new ArrayList<>();
        try (BufferedReader input = new BufferedReader(new FileReader("Day04/input.txt"))) {
            String buf;
            while ((buf = input.readLine()) != null) {
                if (currentLine == bonusRecord.size()) {
                    // The original one.
                    bonusRecord.add(1);
                }
                currentBonus = bonusRecord.get(currentLine);
                res += currentBonus;

                winningNumbers = new HashSet<>();

                String[] cards = buf.split(":")[1].split("\\|");
                for (String split : cards[0].trim().split("\\s+")) {
                    winningNumbers.add(Integer.parseInt(split));
                }

                Integer bonus = 0;

                for (String split : cards[1].trim().split("\\s+")) {
                    Integer holdingCard = Integer.parseInt(split);
                    if (winningNumbers.contains(holdingCard)) {
                        bonus++;
                    }
                }

                for (int i = 1; i <= bonus; i++) {
                    if (currentLine + i < bonusRecord.size()) {
                        bonusRecord.set(currentLine + i,
                                bonusRecord.get(currentLine + i) + currentBonus);
                    } else {
                        if (!(currentLine + i == bonusRecord.size())) {
                            throw new ExceptionInInitializerError("Not expected.");
                        }
                        bonusRecord.add(currentBonus + 1);
                    }
                }
                currentLine++;
            }
            input.close();
        } catch (NumberFormatException | ExceptionInInitializerError e) {
            e.printStackTrace();
        }
        System.out.println("Solution 2: " + res);
        return;
    }
}
