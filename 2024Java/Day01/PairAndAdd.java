package Day01;

import java.util.Map.Entry;
import java.util.regex.*;
import java.util.*;
import java.io.*;

public class PairAndAdd {
    private Vector<Entry<Integer, Integer>> readFile(String filePath) throws IOException {
        Vector<Entry<Integer, Integer>> numList = new Vector<>();
        Pattern re = Pattern.compile("[0-9]+");
        try (BufferedReader in = new BufferedReader(new FileReader("input.txt"))) {
            String buf;
            while ((buf = in.readLine()) != null) {
                Matcher m = re.matcher(buf);
                List<Integer> pair = new ArrayList<>();
                while (m.find()) {
                    pair.add(Integer.valueOf(m.group(0), 10));
                }
                if (pair.size() == 2) {
                    numList.add(new AbstractMap.SimpleEntry<Integer, Integer>(pair.get(0),
                            pair.get(1)));
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return numList;
    }

    public void Solution1() throws IOException {
        Vector<Entry<Integer, Integer>> numList = readFile("input.txt");
        PriorityQueue<Integer> pq1 = new PriorityQueue<>(Comparator.reverseOrder()),
                pq2 = new PriorityQueue<>(Comparator.reverseOrder());
        for (Entry<Integer, Integer> numPair : numList) {
            pq1.add(numPair.getKey());
            pq2.add(numPair.getValue());
        }
        int distanceAddUp = 0;
        int elemCnt = pq1.size();
        for (int i = 0; i < elemCnt; i++) {
            distanceAddUp += Math.abs(pq1.poll() - pq2.poll());
        }
        System.out.println("Solution 1: " + distanceAddUp);
    }

    public void Solution2() throws IOException {
        Vector<Entry<Integer, Integer>> numList = readFile("input.txt");
        Map<Integer, Integer> elemCount = new HashMap<>();
        for (Entry<Integer, Integer> numPair : numList) {
            elemCount.put(numPair.getValue(), elemCount.getOrDefault(numPair.getValue(), 0) + 1);
        }
        int addUp = numList.stream()
                .mapToInt(entry -> entry.getKey() * elemCount.getOrDefault(entry.getKey(), 0))
                .sum();
        System.out.println("Solution 2: " + addUp);
    }

    public static void main(String[] args) throws IOException {
        PairAndAdd solution = new PairAndAdd();
        solution.Solution1();
        solution.Solution2();
        return;
    }
}
