package Day12;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;

public class CharacterMatcher {

    List<String> SpringLists;
    List<List<Integer>> DamagedGroups;

    boolean match(String m, int startPos, int groupSize) {
        if (groupSize + startPos > m.length()) {
            return false;
        }
        // StringBuilder groupSequence = new StringBuilder(m.substring(startPos,
        // startPos + groupSize));
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

    void readFile() throws IOException {
        SpringLists = new ArrayList<>();
        DamagedGroups = new ArrayList<>();
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        String buf;
        while ((buf = input.readLine()) != null) {
            String[] recordings = buf.split("\\s+");
            SpringLists.add(recordings[0]);
            DamagedGroups.add(List.of(recordings[1].trim().split(",")).stream().map(s -> Integer.parseInt(s)).toList());
        }
        input.close();
    }

    HashMap<String, Long> memo;

    long iterateMatch(String m, int startPos, List<Integer> groups, int startGroup) {
        if (startGroup == groups.size()) {
            for (int i = startPos - 1; i < m.length(); i++) {
                if (m.charAt(i) == '#') {
                    return 0;
                }
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
    }// Solution 2: 620189727003627

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

    void Solution1() throws IOException {
        readFile();
        long res = 0, problemSetSize = SpringLists.size();
        for (int i = 0; i < problemSetSize; i++) {
            memo = new HashMap<>();
            res += iterateMatch(SpringLists.get(i), 0, DamagedGroups.get(i), 0);
        }
        System.out.println("Solution 1: " + res);
    }

    void Solution2() throws IOException {
        readFile();
        memo = new HashMap<>();
        long res = 0, problemSetSize = SpringLists.size();
        for (int i = 0; i < problemSetSize; i++) {
            // System.out.println("Processing: " + (i + 1) + "/" + problemSetSize);
            String unfoldMatchString = Collections.nCopies(5, SpringLists.get(i)).stream()
                    .collect(Collectors.joining("?"));
            List<Integer> unfoldGroupts = Collections.nCopies(5, DamagedGroups.get(i)).stream()
                    .flatMap(groupList -> groupList.stream()).collect(Collectors.toList());
            memo = new HashMap<>();
            res += iterateMatch(unfoldMatchString, 0, unfoldGroupts, 0);
        }
        System.out.println("Solution 2: " + res);
    }

    void Solution2_DP() throws IOException {
        readFile();
        long res = 0, problemSetSize = SpringLists.size();
        for (int i = 0; i < problemSetSize; i++) {

            // System.out.println("Processing: " + (i + 1) + "/" + problemSetSize);
            String unfoldMatchString = Collections.nCopies(5, SpringLists.get(i)).stream()
                    .collect(Collectors.joining("?"));
            List<Integer> unfoldGroupts = Collections.nCopies(5, DamagedGroups.get(i)).stream()
                    .flatMap(groupList -> groupList.stream()).collect(Collectors.toList());
            res += DP(unfoldMatchString, unfoldGroupts);
        }
        System.out.println("Solution 2 DP: " + res);
    }

    public static void main(String[] args) throws IOException {

        CharacterMatcher Solution = new CharacterMatcher();
        // Solution.unitTest();
        Solution.unitTest();
        Solution.Solution1();
        Solution.Solution2();
        Solution.Solution2_DP();
    }
}
