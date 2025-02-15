package Day08;

import java.io.*;
import java.util.*;
import java.util.Map.*;
import java.util.AbstractMap.*;
import java.util.regex.*;

public class CamelInstruction {
    // Solution 1
    List<Character> Instructions;
    HashMap<String, Entry<String, String>> Network;

    // Solution 2
    HashMap<String, Entry<String, Integer>> PathList;
    List<String> StartList;
    List<Integer> StepCnt;

    void readFile() throws IOException {
        Network = new HashMap<>();
        StartList = new ArrayList<>();
        StepCnt = new ArrayList<>();
        PathList = new HashMap<>();

        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        String buf;
        buf = input.readLine();
        Instructions = buf.chars().mapToObj(l -> (char) l).toList();
        // Instructions = buf.chars().mapToObj(l -> (char)
        // l).collect(Collectors.toCollection(ArrayList::new));

        while ((buf = input.readLine()) != null) {
            if (buf.isEmpty()) {
                continue;
            }
            Matcher re = Pattern.compile("(\\w+)\\s+=\\s+\\((\\w+),\\s+(\\w+)\\)").matcher(buf);
            if (re.find() && re.groupCount() == 3) {
                // System.out.println(re.group());
                Network.put(re.group(1), new SimpleEntry<>(re.group(2), re.group(3)));
                if (re.group(1).charAt(2) == 'A') {
                    StartList.add(re.group(1));
                }
            }
        }
        input.close();
    }

    void Solution1() throws IOException {
        readFile();
        int curInsId = 0;
        int nCnt = 0;
        String start = "AAA";
        while (!start.equals("ZZZ")) {

            Entry<String, String> nextDes = Network.get(start);
            if (nextDes == null) {
                System.out.println("Map incomplete");
                return;
            }
            if (Instructions.get(curInsId).equals('L')) {
                start = nextDes.getKey();
            } else {
                start = nextDes.getValue();
            }

            nCnt++;
            curInsId = (curInsId + 1) % Instructions.size();

        }
        System.out.println("Solution 1: " + nCnt);

    }

    // This problem have an implicit condition: every start node end with 'A' only
    // reaches one specific node end with 'Z'.
    void findAllPath(String start) {
        int curInsId = 0;
        int curPathLength = 0;
        HashMap<String, Integer> Visited = new HashMap<>();
        String curPos = start;
        while (true) {
            Entry<String, String> nextDesPair = Network.get(curPos);

            if (nextDesPair == null) {
                System.out.println("Map incomplete");
                return;
            }

            String nextDes;
            if (Instructions.get(curInsId).equals('L')) {
                nextDes = nextDesPair.getKey();
            } else {
                nextDes = nextDesPair.getValue();
            }

            curInsId = (curInsId + 1) % Instructions.size();
            curPathLength++;
            if (Visited.getOrDefault(nextDes + curInsId, 0) == 0) {
                curPos = nextDes;
                Visited.put(curPos + curInsId, curPathLength);
            } else {
                if (nextDes.charAt(2) == 'Z') {
                    PathList.put(start, new SimpleEntry<>(nextDes, Visited.get(nextDes + curInsId)));
                    // PathList.get(start).put();
                    return;
                }
                curPos = nextDes;
            }
        }

    }

    long gcd(long a, long b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    long lcm(long a, long b) {
        return a * b / gcd(a, b);
    }

    void Solution2() throws IOException {
        readFile();
        for (String start : StartList) {
            findAllPath(start);
        }
        long res = 1;
        for (int i = 0; i < PathList.size(); i++) {
            System.out.println("" + PathList.keySet().toArray()[i] + " " +
                    PathList.get(PathList.keySet().toArray()[i]).getKey()
                    + " " + PathList.get(PathList.keySet().toArray()[i]).getValue());
            res = lcm(res, PathList.get(PathList.keySet().toArray()[i]).getValue());
        }
        System.out.println("Solution 2: " + res);
    }

    public static void main(String[] args) throws IOException {
        CamelInstruction Day08 = new CamelInstruction();
        Day08.Solution1();
        Day08.Solution2();
    }
}
