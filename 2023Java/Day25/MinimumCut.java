package Day25;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.Set;

public class MinimumCut {
    void readFile() throws Exception {
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        input.lines().forEach(l -> {
            var split = l.split(":");
            String inWire = split[0].trim();
            WireGraph.putIfAbsent(inWire, new HashSet<>());
            Arrays.stream(split[1].trim().split("\\s+")).forEach(w -> {
                WireGraph.get(inWire).add(w.trim());
                WireGraph.putIfAbsent(w.trim(), new HashSet<>());
                WireGraph.get(w.trim()).add(inWire);
            });
        });
        input.close();
    }

    Map<String, Set<String>> WireGraph = new HashMap<>();

    Map<String, Integer> PassCnt = new HashMap<>();

    int CliqueCount(Map<String, String> cut) {
        Map<String, Set<String>> visited = new HashMap<>();
        cut.forEach((u, v) -> {
            visited.putIfAbsent(u, new HashSet<>());
            visited.get(u).add(v);
            visited.putIfAbsent(v, new HashSet<>());
            visited.get(v).add(u);
        });
        int cliqueCnt = 0;
        for (String entry : WireGraph.keySet()) {
            if (!visited.containsKey(entry)) {
                cliqueCnt++;
                visited.put(entry, new HashSet<>());
                Queue<String> q = new LinkedList<>(List.of(entry));
                while (!q.isEmpty()) {
                    String curNode = q.poll();
                    for (String nextV : WireGraph.get(curNode)) {
                        if (!visited.get(curNode).contains(nextV)) {
                            visited.get(curNode).add(nextV);
                            visited.putIfAbsent(nextV, new HashSet<>());
                            visited.get(nextV).add(curNode);
                            q.add(nextV);
                        }
                    }
                }
            }
        }
        return cliqueCnt;
    }

    void Solution1() throws Exception {
        readFile();
        // Ehhhhhhhhhhhhhhhhhhhhh, something like Karger's algorithm, TODO
    }

    void Solution2() throws Exception {

    }

    public static void main(String[] args) throws Exception {
        MinimumCut Day25 = new MinimumCut();
        Day25.Solution1();
        Day25.Solution2();
    }
}

// var sorted = crossCnt.entrySet().stream().sorted(Comparator.comparing(Entry<String, Integer>::getValue).reversed());
// var itr = sorted.iterator();
// System.out.println(itr.next().getKey() + "," + itr.next().getValue());
// System.out.println(itr.next().getKey() + "," + itr.next().getValue());
// System.out.println(itr.next().getKey() + "," + itr.next().getValue());
// System.out.println(itr.next().getKey() + "," + itr.next().getValue());
// System.out.println(itr.next().getKey() + "," + itr.next().getValue());
// System.out.println(itr.next().getKey() + "," + itr.next().getValue());
// BufferedWriter output = new BufferedWriter(new FileWriter("output.txt"));
// for (Entry<String, Integer> e : crossCnt.entrySet().stream().sorted(Comparator.comparing(Entry<String, Integer>::getValue).reversed()).toList()) {
// output.write(e.getKey() + "," + e.getValue());
// output.newLine();
// }
// output.close();// The buffer is never flushed before closed.