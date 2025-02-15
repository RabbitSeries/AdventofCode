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

    Map<String, Long> memo;

    long iterateMatch(String m, int startPos, List<Integer> groups, int startGroup) {
        if (startGroup == groups.size()) {
            for (int i = startPos - 1; i < m.length(); i++) {
                if (m.charAt(i) == '#') {
                    return 0;
                }
            }
            return 1;
        } else {
            if (memo.containsKey(startPos + "," + startGroup)) {
                return memo.get(startPos + "," + startGroup);
            }
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

    public static void main(String[] args) throws IOException {

        CharacterMatcher Solution = new CharacterMatcher();
        // Solution.unitTest();
        Solution.unitTest();
        Solution.Solution1();
        Solution.Solution2();
    }
}
