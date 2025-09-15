package year2023.Day12;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
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

    /**
     * @deprecated Low performance
     * @since 1.0.1
     */
    Map<String, Long> memo = new HashMap<>();

    /**
     * @deprecated Low performance
     * @since 1.0.1
     */
    @Deprecated(since = "1.0.1")
    long iterateMatch(String m, int startPos, List<Integer> groups, int startGroup) {
        if (startGroup == groups.size()) {
            if (m.substring(startPos - 1).contains("#")) {
                return 0;
            }
            return 1;
        } else if (memo.containsKey(startPos + "," + startGroup)) {
            return memo.get(startPos + "," + startGroup);
        }
        int curGroupSize = groups.get(startGroup);
        long res = IntStream.rangeClosed(startPos, m.length() - curGroupSize)
                .takeWhile(i -> i == 0 || m.charAt(i - 1) != '#')
                .filter(i -> match(m, i, curGroupSize))
                .mapToLong(i -> iterateMatch(m, i + curGroupSize + 1, groups, startGroup + 1))
                .sum();
        memo.put(startPos + "," + startGroup, res);
        return res;
    }

    class NodeInfo implements Comparable<NodeInfo> {
        public int pos, group;

        public NodeInfo(int pos, int group) {
            this.pos = pos;
            this.group = group;
        }

        @Override
        public int compareTo(NodeInfo node1) {
            return Integer.compare(pos, node1.pos);
        }

        @Override
        public int hashCode() {
            return Objects.hash(this.pos, this.group);
        }

        @Override
        public boolean equals(Object obj) {
            if (obj.getClass() != this.getClass()) {
                return false;
            }
            NodeInfo rhs = (NodeInfo) obj;
            return this.pos == rhs.pos && this.group == rhs.group;
        }
    }

    long DP(String m, List<Integer> groups) {// 162 ms
        // dp meaning must be consistant with groups mapped value, in this case is count
        long[][] dp = new long[m.length() + 1][groups.size() + 1]; // the former i(not include) and j(not include)'s matched times
        dp[0][0] = 1;
        for (int i : IntStream.rangeClosed(0, m.length()).toArray()) {
            IntStream.rangeClosed(0, groups.size()).filter(j -> dp[i][j] != 0).forEach(j -> {
                // dp[i-1][j-1] != 0, continue to match at i, j_th group
                if (i < m.length() && m.charAt(i) != '#') {
                    // opt to match current group at i+1 for '.', '?'
                    dp[i + 1][j] += dp[i][j];
                }
                // Attempt to match current group
                if (j < groups.size()) {
                    int group = groups.get(j);
                    if (match(m, i, group)) {
                        int nextPos = i + group + 1; // skip a white space
                        // Continue to match at nextPos on j+1 group
                        // Or Accumulate to final position
                        dp[Math.min(nextPos, m.length())][j + 1] += dp[i][j];
                    }
                }
            });
        }
        return dp[m.length()][groups.size()];
    }

    long heuristics(String m, List<Integer> groups) { // 169 ms
        var begin = new NodeInfo(0, 0);
        var end = new NodeInfo(m.length(), groups.size());
        Queue<NodeInfo> pq = new LinkedList<>(List.of(begin));
        // Queue<NodeInfo> pq = new PriorityQueue<>(List.of(begin)); // Sort also works
        Map<NodeInfo, Long> matchCount = new HashMap<>(Map.of(begin, 1L));
        while (!pq.isEmpty()) {
            NodeInfo node = pq.poll();
            int i = node.pos, j = node.group;
            if (i < m.length() && m.charAt(i) != '#') {
                NodeInfo nextNode = new NodeInfo(i + 1, j);
                if (!matchCount.containsKey(nextNode)) {
                    pq.add(nextNode);
                }
                matchCount.compute(nextNode, (k, v) -> (v == null ? 0 : v) + matchCount.get(node));
            }
            if (j < groups.size()) {
                int group = groups.get(j);
                if (match(m, i, group)) {
                    NodeInfo nextNode = new NodeInfo(Math.min(i + group + 1, m.length()), j + 1);
                    if (!matchCount.containsKey(nextNode)) {
                        pq.add(nextNode);
                    }
                    matchCount.compute(nextNode, (k, v) -> (v == null ? 0 : v) + matchCount.get(node));
                }
            }
        }
        return matchCount.getOrDefault(end, 0L);
    }

    void unitTest() {
        assert (DP("?###????????", List.of(3, 2, 1)) == 10);
        assert (heuristics("?###????????", List.of(3, 2, 1)) == 10);
        assert (DP("?#??#????.????#", List.of(8, 1, 1)) == 6);
        assert (heuristics("?#??#????.????#", List.of(8, 1, 1)) == 6);
        System.out.println("Unit test assertation successful");
    }

    public void Solution1(BufferedReader input) throws IOException {
        readFile(input);
        System.out.println("Solution 1: " + SpringLists.stream().mapToLong(K_V -> {
            return heuristics(K_V.getKey(), K_V.getValue());
        }).sum());
    }

    public void Solution2(BufferedReader input) throws IOException {
        System.out.println("Solution 2: " + SpringLists.stream().mapToLong(K_V -> {
            String unfoldMatchString = Collections.nCopies(5, K_V.getKey()).stream()
                    .collect(Collectors.joining("?"));
            List<Integer> unfoldGroupts = Collections.nCopies(5, K_V.getValue()).stream()
                    .flatMap(groupList -> groupList.stream()).collect(Collectors.toList());
            // return iterateMatch(unfoldMatchString, 0, unfoldGroupts, 0);
            return heuristics(unfoldMatchString, unfoldGroupts);
        }).sum());
    }

    public static void main(String[] args) throws IOException {
        CharacterMatcher Day12 = new CharacterMatcher();
        var now = System.currentTimeMillis();
        Day12.unitTest();
        Day12.Solution1(new BufferedReader(new FileReader("Day12/input.txt")));
        Day12.Solution2(new BufferedReader(new FileReader("Day12/input.txt")));
        System.out.println(System.currentTimeMillis() - now);
    }
}
