package Day05;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;

public class OrderingUpdates {

    HashMap<Integer, Set<Integer>> readRules() throws IOException {
        HashMap<Integer, Set<Integer>> rules = new HashMap<>();
        BufferedReader RuleInput = new BufferedReader(new FileReader("RuleInput.txt"));
        String linebuf;
        while ((linebuf = RuleInput.readLine()) != null) {
            int num1 = Integer.parseInt(linebuf.split("\\|")[0]),
                    num2 = Integer.parseInt(linebuf.split("\\|")[1]);
            rules.putIfAbsent(num1, new HashSet<>());
            rules.get(num1).add(num2);
        }
        RuleInput.close();
        return rules;
    }

    List<List<Integer>> readUpdates() throws IOException {
        List<List<Integer>> updates = new ArrayList<>();
        String linebuf;
        BufferedReader UpdatesInput = new BufferedReader(new FileReader("UpdatesInput.txt"));
        while ((linebuf = UpdatesInput.readLine()) != null) {
            if (!linebuf.isEmpty()) {
                List<Integer> update = Arrays.asList(linebuf.trim().split(",")).stream()
                        .map(l -> Integer.parseInt(l)).collect(Collectors.toList());

                updates.add(update);
            }
        }
        UpdatesInput.close();
        return updates;
    }

    boolean isCorrectOrder(List<Integer> update, HashMap<Integer, Set<Integer>> rules) {
        for (int i = 0; i < update.size(); i++) {
            for (int j = 0; j < i; j++) {
                if (rules.getOrDefault(update.get(i), null) != null
                        && rules.get(update.get(i)).contains(update.get(j))) {
                    return false;
                }
            }
        }
        return true;
    }

    void Solution1() throws IOException {
        HashMap<Integer, Set<Integer>> rules = readRules();
        List<List<Integer>> updates = readUpdates();
        int sum = 0;
        for (var update : updates) {
            if (isCorrectOrder(update, rules)) {
                int middle = update.size() / 2;
                sum += update.get(middle);
            }
        }
        System.out.println("Solution 1: " + sum);
    }

    void reOrder(List<Integer> update, HashMap<Integer, Set<Integer>> rules) {
        for (int i = 0; i < update.size(); i++) {
            for (int j = i; j < update.size(); j++) {
                boolean ahead = true;
                for (int k = i; k < update.size(); k++) {
                    if (k == j) {
                        continue;
                    }
                    if (rules.getOrDefault(update.get(k), null) != null
                            && rules.get(update.get(k)).contains(update.get(j))) {
                        ahead = false;
                        break;
                    }
                }
                if (ahead) {
                    var t = update.get(i);
                    update.set(i, update.get(j));
                    update.set(j, t);
                    break;
                }
            }
        }
    }

    int Solution2() throws IOException {
        HashMap<Integer, Set<Integer>> rules = readRules();
        List<List<Integer>> updates = readUpdates();
        int sum = 0;
        for (List<Integer> update : updates) {
            if (!isCorrectOrder(update, rules)) {
                reOrder(update, rules);
                int middle = update.size() / 2;
                sum += update.get(middle);
            }
        }
        System.out.println("Solution 2: " + sum);
        return 0;
    }

    public static void main(String[] args) throws IOException {
        OrderingUpdates Day05 = new OrderingUpdates();
        Day05.Solution1();
        Day05.Solution2();
    }
}
