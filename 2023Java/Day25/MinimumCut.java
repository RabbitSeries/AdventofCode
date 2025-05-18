package Day25;

import java.io.*;
import java.util.*;

public class MinimumCut {
    void readFile() throws Exception {
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        input.lines().forEach(l -> {
            var split = l.split(":");
            String inWire = split[0].trim();
            Arrays.stream(split[1].trim().split(" ")).forEach(w -> {
                WireGraph.computeIfAbsent(inWire, s -> new HashSet<>()).add(w);
                WireGraph.computeIfAbsent(w, s -> new HashSet<>()).add(inWire);
            });
        });
        input.close();
    }

    Map<String, Set<String>> WireGraph = new HashMap<>();

    int CutCount(Map<String, Set<String>> Cliques) {
        if (Cliques.size() != 2)
            return -1;
        List<String> CliqueNames = Cliques.keySet().stream().toList();
        int cutCnt = 0;
        String from = CliqueNames.get(0), to = CliqueNames.get(1);
        for (var fromV : Cliques.get(from)) {
            for (var conn : WireGraph.get(fromV)) {
                if (Cliques.get(to).contains(conn)) {
                    cutCnt++;
                }
            }
        }
        return cutCnt;
    }

    String FindFather(Map<String, String> Hierachy, String name) {
        if (Hierachy.get(name).equals(name)) {
            return name;
        }
        return Hierachy.compute(name, (k, e) -> FindFather(Hierachy, Hierachy.get(e)));
    }

    void Solution1() throws Exception {
        readFile();
        Random rand = new Random(System.nanoTime());
        // unionset - k,v : parent, clique-member(Include parent)
        Map<String, Set<String>> Cliques = new HashMap<>();
        while (CutCount(Cliques) != 3) {
            Cliques.clear();
            Map<String, String> Father = new HashMap<>();
            Set<String> NodesHavingEdge = new HashSet<>(WireGraph.keySet());// Returns a Set view of the keys contained
                                                                            // in this map.
            // The set is backed by the map, so changes to the map are reflected in the set,
            // and vice-versa.
            HashMap<String, Set<String>> visited = new HashMap<>();
            WireGraph.keySet().forEach(e -> {
                visited.put(e, new HashSet<>());
                Cliques.put(e, new HashSet<>(Set.of(e)));
                Father.put(e, e);
            });
            // while (!NodesHavingEdge.isEmpty() && CutCount(Cliques) != 3) { // This cost over 3 minutes
            while (Cliques.size() != 2) { // 300 ms
                // Get father
                List<String> NodeList = NodesHavingEdge.stream().toList();
                String father = NodeList.get(rand.nextInt(NodeList.size()));
                // Get child
                List<String> ChildList = WireGraph.get(father).stream()
                        .filter(child -> !visited.get(father).contains(child) && !visited.get(child).contains(father))
                        .toList();
                if (ChildList.isEmpty()) {
                    NodesHavingEdge.remove(father);
                    continue;
                }
                String child = ChildList.get(rand.nextInt(ChildList.size()));
                visited.get(father).add(child);
                visited.get(child).add(father);
                String FatherCliqueName = FindFather(Father, father);
                String ChildCliqueName = FindFather(Father, child);
                Father.put(ChildCliqueName, FatherCliqueName);
                if (!FatherCliqueName.equals(ChildCliqueName)) {
                    Cliques.get(FatherCliqueName).addAll(Cliques.get(ChildCliqueName));
                    Cliques.remove(ChildCliqueName);
                }
            }
        }
        System.out
                .println("Solution 1: " + Cliques.values().stream().mapToInt(s -> s.size()).reduce(1, (i, v) -> i * v));
    }

    void Solution2() throws Exception {
        System.out.println("\t\t\t------All 50 stars AoC Finished!!!!------");
    }

    public static void main(String[] args) throws Exception {
        MinimumCut Day25 = new MinimumCut();
        Day25.Solution1();
        Day25.Solution2();
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