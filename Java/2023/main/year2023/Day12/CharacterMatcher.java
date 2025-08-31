package year2023.Day12;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import org.hipparchus.util.Pair;

import JavaDataModel.*;

@AoCSolution()
public class CharacterMatcher implements ISolution {

    List<Pair<String, List<Integer>>> SpringLists;

    boolean match(String m, int startPos, int groupSize) {
        if (groupSize + startPos > m.length()) {
            return false;
        }
        String groupSequence = m.substring(startPos, startPos + groupSize);
        for (int i = 0; i < groupSize; i++) {
            if (groupSequence.charAt(i) == '.') {
                return false;
            }
        }
        if (startPos != 0 && m.charAt(startPos - 1) == '#') {
            return false;
        }
        if (groupSize + startPos < m.length() && m.charAt(groupSize + startPos) == '#') {
            return false;
        }
        return true;
    }

    void readFile(BufferedReader input) throws IOException {
        SpringLists = input.lines().map(line -> line.split("\\s+"))
                .map(recordings -> new Pair<>(recordings[0],
                        Stream.of(recordings[1].trim().split(",")).map(Integer::parseInt).toList()))
                .toList();
    }

    Map<String, Long> memo = new HashMap<>();

    long iterateMatch(String m, int startPos, List<Integer> groups, int startGroup) {
        if (startGroup == groups.size()) {
            if (m.substring(startPos - 1).contains("#")) {
                return 0;
            }
            return 1;
        } else if (memo.containsKey(startPos + "," + startGroup)) {
            return memo.get(startPos + "," + startGroup);
        }
        long res = 0;
        int curGroupSize = groups.get(startGroup);
        for (int i = startPos; i + curGroupSize - 1 < m.length(); i++) {
            if (i > 0 && m.charAt(i - 1) == '#') {
                break;
            }
            if (match(m, i, curGroupSize)) {
                res += iterateMatch(m, i + curGroupSize + 1, groups, startGroup + 1);
            }
        }
        memo.put(startPos + "," + startGroup, res);
        return res;
    }

    class nodeInfo implements Comparable<nodeInfo> {
        public int pos, group;

        public nodeInfo(int pos, int group) {
            this.pos = pos;
            this.group = group;
        }

        @Override
        public int compareTo(nodeInfo node1) {
            return Integer.compare(pos, node1.pos);
        }
    }

    // heuristic search
    // Yeah, reverse topological order should be starting from m.lenth()-1
    // Then DP order should also be m[i][j] = m[i+curGroupSize][j], ehhhh I don't
    // know
    long DP(String m, List<Integer> groups) {// error
        PriorityQueue<nodeInfo> pq = new PriorityQueue<>(Comparator.reverseOrder());
        String str = "asdasd";
        Arrays.stream(str.split("asdad")).map(s -> s.length());
        HashMap<String, Long> visited = new HashMap<>();
        pq.add(new nodeInfo(0, 0));
        while (!pq.isEmpty()) {
            nodeInfo curNode = pq.poll();
            int curPos = curNode.pos, curGroup = curNode.group;
            if (curPos >= m.length()) {
                continue;
            }
            if (curGroup == groups.size()) {
                boolean cleaned = true;
                for (int i = curPos; i < m.length(); i++) {
                    if (m.charAt(i) == '#') {
                        cleaned = false;
                    }
                }
                if (cleaned) {
                    long curRes = visited.getOrDefault(m.length() + "," + curGroup, (long) 0);
                    visited.put(m.length() + "," + curGroup, curRes + 1);
                }
                continue;
            }
            while (curPos < m.length() && m.charAt(curPos) == '.') {
                curPos++;
            }
            for (int nextPos = curPos; nextPos < m.length(); nextPos++) {
                if ((nextPos > 0 && m.charAt(nextPos - 1) == '#')) {
                    break;
                }
                if (match(m, nextPos, groups.get(curGroup))) {
                    if (!visited.containsKey(nextPos + "," + curGroup)) {
                        visited.put(nextPos + "," + curGroup, (long) 1);
                        pq.add(new nodeInfo(nextPos + groups.get(curGroup) + 1, curGroup + 1));
                    } else {
                        long curRes = visited.get(nextPos + "," + curGroup);
                        visited.put(nextPos + "," + curGroup, curRes + 1);
                    }
                }
            }
        }
        return visited.getOrDefault(m.length() + "," + groups.size(), (long) 0);
    }

    void unitTest() {
        memo = new HashMap<>();
        assert (iterateMatch("?###????????", 0, List.of(3, 2, 1), 0) == 10);
        memo = new HashMap<>();
        assert (iterateMatch("?#??#????.????#", 0, List.of(8, 1, 1), 0) == 6);
        System.out.println("Unit test assertation successful");
    }

    public void Solution1(BufferedReader input) throws IOException {
        readFile(input);
        System.out.println("Solution 1: " + SpringLists.stream().mapToLong(K_V -> {
            memo.clear();
            return iterateMatch(K_V.getKey(), 0, K_V.getValue(), 0);
        }).sum());
    }

    public void Solution2(BufferedReader input) throws IOException {
        System.out.println("Solution 2: " + SpringLists.stream().mapToLong(K_V -> {
            memo.clear();
            String unfoldMatchString = Collections.nCopies(5, K_V.getKey()).stream()
                    .collect(Collectors.joining("?"));
            List<Integer> unfoldGroupts = Collections.nCopies(5, K_V.getValue()).stream()
                    .flatMap(groupList -> groupList.stream()).collect(Collectors.toList());
            return iterateMatch(unfoldMatchString, 0, unfoldGroupts, 0);
        }).sum());
    }

    // void Solution2_DP(BufferedReader input) throws IOException {
    // readFile(input);
    // long res = 0, problemSetSize = SpringLists.size();
    // for (int i = 0; i < problemSetSize; i++) {

    // // System.out.println("Processing: " + (i + 1) + "/" + problemSetSize);
    // String unfoldMatchString = Collections.nCopies(5, SpringLists.get(i)).stream()
    // .collect(Collectors.joining("?"));
    // List<Integer> unfoldGroupts = Collections.nCopies(5, DamagedGroups.get(i)).stream()
    // .flatMap(groupList -> groupList.stream()).collect(Collectors.toList());
    // res += DP(unfoldMatchString, unfoldGroupts);
    // }
    // System.out.println("Solution 2 DP: " + res);
    // }

    public static void main(String[] args) throws IOException {

        CharacterMatcher Day12 = new CharacterMatcher();
        // Solution.unitTest();
        Day12.unitTest();
        Day12.Solution1(new BufferedReader(new FileReader("Day12/input.txt")));
        Day12.Solution2(new BufferedReader(new FileReader("Day12/input.txt")));
        // Day12.Solution2_DP(new BufferedReader(new FileReader("Day12/input.txt")));
    }
}
