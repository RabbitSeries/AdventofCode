package year2023.Day08;

import java.io.*;
import java.util.*;
import java.util.Map.*;
import java.util.AbstractMap.*;
import java.util.regex.*;
import java.util.stream.Collectors;

import JavaDataModel.*;

@AoCSolution()
public class CamelInstruction implements ISolution {
    // Solution 1
    List<Character> Instructions;

    Map<String, Entry<String, String>> Network;

    List<String> StartList = new ArrayList<>();

    void readFile(BufferedReader input) throws IOException {
        Instructions = input.readLine().chars().mapToObj(l -> (char) l).toList();
        Pattern re = Pattern.compile("(\\w+)\\s+=\\s+\\((\\w+),\\s+(\\w+)\\)");
        Network = input.lines()
                .map(line -> re.matcher(line)).filter(matcher -> matcher.find())
                .collect(Collectors.toMap(k -> {
                    if (k.group(1).endsWith("A")) {
                        StartList.add(k.group(1));
                    }
                    return k.group(1);
                }, k -> new SimpleEntry<>(k.group(2), k.group(3))));
    }

    int insId(int len) {
        return len % Instructions.size();
    }

    public void Solution1(BufferedReader input) throws IOException {
        readFile(input);
        int nCnt = 0;
        String start = "AAA";
        while (!start.equals("ZZZ")) {
            Entry<String, String> nextDes = Network.get(start);
            start = Instructions.get(insId(nCnt)).equals('L') ? nextDes.getKey() : nextDes.getValue();
            nCnt++;
        }
        System.out.println("Solution 1: " + nCnt);
    }

    // This problem have an implicit condition: every start node end with 'A' only
    // reaches one specific node end with 'Z'.
    int findAllPath(String curPos) {
        int pathLen = 0;
        Map<String, Integer> Visited = new HashMap<>();
        while (!Visited.containsKey(curPos + insId(pathLen)) || !curPos.endsWith("Z")) {
            int curInsId = insId(pathLen);
            Visited.put(curPos + curInsId, pathLen);
            Entry<String, String> nextDesPair = Network.get(curPos);
            curPos = Instructions.get(curInsId).equals('L') ? nextDesPair.getKey() : nextDesPair.getValue();
            pathLen++;
        }
        return Visited.get(curPos + insId(pathLen));
    }

    static long gcd(long a, long b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    static long lcm(long a, long b) {
        return a * b / gcd(a, b);
    }

    public void Solution2(BufferedReader input) throws IOException {
        System.out.println("Solution 2: " + StartList.stream().mapToLong(start -> findAllPath(start)).reduce(1L, CamelInstruction::lcm));
    }

    public static void main(String[] args) throws IOException {
        CamelInstruction Day08 = new CamelInstruction();
        Day08.Solution1(new BufferedReader(new FileReader("Day08/input.txt")));
        Day08.Solution2(new BufferedReader(new FileReader("Day08/input.txt")));
    }
}
