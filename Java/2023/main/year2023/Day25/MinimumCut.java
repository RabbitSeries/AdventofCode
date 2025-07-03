package year2023.Day25;

import java.io.*;
import java.util.*;

import JavaDataModel.*;

@AoCSolution()
public class MinimumCut implements SolutionBase {
    void readFile(BufferedReader input) throws Exception {
        input.lines().forEach(l -> {
            var split = l.split(":");
            int inWire = Hasher(split[0].trim());
            Arrays.stream(split[1].trim().split(" ")).forEach(w -> {
                int wHash = Hasher(w);
                WireGraph.computeIfAbsent(inWire, s -> new HashSet<>()).add(wHash);
                WireGraph.computeIfAbsent(wHash, s -> new HashSet<>()).add(inWire);
            });
        });
        input.close();
    }

    int Hasher(String WireName) {
        return WireName.chars().reduce(0, (i, c) -> i * 26 + c - 'a');
    }

    Map<Integer, Set<Integer>> WireGraph = new HashMap<>();

    int CutCount(Map<Integer, Set<Integer>> Cliques) {
        if (Cliques.size() != 2)
            return -1;
        List<Integer> CliqueNames = Cliques.keySet().stream().toList();
        int cutCnt = 0;
        int from = CliqueNames.get(0), to = CliqueNames.get(1);
        for (var fromV : Cliques.get(from)) {
            for (var conn : WireGraph.get(fromV)) {
                if (Cliques.get(to).contains(conn)) {
                    cutCnt++;
                }
            }
        }
        return cutCnt;
    }

    int FindFather(Map<Integer, Integer> Hierachy, int name) {
        if (Hierachy.get(name).equals(name)) {
            return name;
        }
        return Hierachy.compute(name, (k, e) -> FindFather(Hierachy, Hierachy.get(e)));
    }

    Map<Integer, Set<Integer>> Karger(long seed, long timeout) {
        var now = System.currentTimeMillis();
        Random rand = new Random(seed);
        Map<Integer, Set<Integer>> Cliques = new HashMap<>();
        Map<Integer, Integer> Hierachy = new HashMap<>();
        Set<Integer> NodesHavingEdge = new HashSet<>(WireGraph.keySet());
        HashMap<Integer, Set<Integer>> visited = new HashMap<>();
        WireGraph.keySet().forEach(e -> {
            visited.put(e, new HashSet<>());
            Cliques.put(e, new HashSet<>(Set.of(e)));
            Hierachy.put(e, e);
        });
        while (Cliques.size() != 2) {
            if (System.currentTimeMillis() - now > timeout) {
                break;
            }
            // Get father
            List<Integer> NodeList = NodesHavingEdge.stream().toList();
            Integer father = NodeList.get(rand.nextInt(NodeList.size()));
            // Get child
            List<Integer> ChildList = WireGraph.get(father).stream()
                    .filter(child -> !visited.get(father).contains(child) && !visited.get(child).contains(father))
                    .toList();
            if (ChildList.isEmpty()) {
                NodesHavingEdge.remove(father);
                continue;
            }
            Integer child = ChildList.get(rand.nextInt(ChildList.size()));
            visited.get(father).add(child);
            visited.get(child).add(father);
            int FatherCliqueName = FindFather(Hierachy, father), ChildCliqueName = FindFather(Hierachy, child);
            Hierachy.put(ChildCliqueName, FatherCliqueName);
            if (FatherCliqueName != ChildCliqueName) {
                Cliques.get(FatherCliqueName).addAll(Cliques.get(ChildCliqueName));
                Cliques.remove(ChildCliqueName);
            }
        }
        return Cliques;
    }

    public void Solution1(BufferedReader input) throws Exception {
        readFile(input);
        // long timeout = 70;
        // while (true) {
        // long seed = System.currentTimeMillis();
        // var res = Karger(seed, timeout);
        // if (CutCount(res) == 3) {
        // System.out.println("Using seed: " + seed);
        // System.out.println("Solution 1: " + res.values().stream().mapToInt(s -> s.size()).reduce(1, (i, v) -> i * v));
        // break;
        // }
        // }
        // 1747570281531 - 40ms
        System.out.println("Solution 1: " + Karger(1747570281531L, Long.MAX_VALUE).values().stream().mapToInt(s -> s.size()).reduce(1, (i, v) -> i * v));
    }

    public void Solution2(BufferedReader input) throws Exception {
        System.out.println("\t\t\t------All 50 stars AoC Finished!!!!------");
    }

    public static void main(String[] args) throws Exception {
        MinimumCut Day25 = new MinimumCut();
        Day25.Solution1(new BufferedReader(new FileReader("Day25/input.txt")));
        Day25.Solution2(new BufferedReader(new FileReader("Day25/input.txt")));
    }
}

// var sorted =
// crossCnt.entrySet().stream().sorted(Comparator.comparing(Entry<String,
// Integer>::getValue).reversed());
// var itr = sorted.iterator();
// System.out.println(itr.next().getKey() + "," + itr.next().getValue());
// System.out.println(itr.next().getKey() + "," + itr.next().getValue());
// System.out.println(itr.next().getKey() + "," + itr.next().getValue());
// System.out.println(itr.next().getKey() + "," + itr.next().getValue());
// System.out.println(itr.next().getKey() + "," + itr.next().getValue());
// System.out.println(itr.next().getKey() + "," + itr.next().getValue());
// BufferedWriter output = new BufferedWriter(new FileWriter("output.txt"));
// for (Entry<String, Integer> e :
// crossCnt.entrySet().stream().sorted(Comparator.comparing(Entry<String,
// Integer>::getValue).reversed()).toList()) {
// output.write(e.getKey() + "," + e.getValue());
// output.newLine();
// }
// output.close();// The buffer is never flushed before closed.