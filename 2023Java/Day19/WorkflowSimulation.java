package Day19;

import java.io.*;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import JavaDataModel.*;

public class WorkflowSimulation implements SolutionBase2023 {

    class Condition {

        public String dest;

        public char property;

        public Comparator<Integer> cmp;

        public int threshold;

        public Condition(String dst, char p, Comparator<Integer> cp, int tsh) {
            dest = dst;
            property = p;
            cmp = cp;
            threshold = tsh;
        }
    };

    class Part {
        HashMap<Character, Integer> prop = new HashMap<>();

        public Part(String is) {
            Scanner sc = new Scanner(is);
            prop.put('x', sc.nextInt());
            prop.put('m', sc.nextInt());
            prop.put('a', sc.nextInt());
            prop.put('s', sc.nextInt());
            sc.close();
        }

        public int get(char c) {
            return prop.get(c);
        }

        public long rating() {
            return prop.values().stream().mapToLong(i -> (long) i).sum();
        }
    };

    HashMap<String, List<Condition>> ruleLists = new HashMap<>();

    List<Part> acceptedList = new ArrayList<>(), rejectedList = new ArrayList<>();

    List<Part> PartList = new ArrayList<>();

    HashMap<Character, Comparator<Integer>> Comparators = new HashMap<>(
            Map.of(
                    '<', Comparator.comparing(Integer::intValue).reversed(),
                    '>', Comparator.comparing(Integer::intValue)));

    void readFile() throws IOException {
        BufferedReader input = new BufferedReader(new FileReader("Day19/input.txt"));
        for (String buf; (buf = input.readLine()) != null;) {
            Matcher workflowMatch = Pattern.compile("^.+(?=\\{.*\\}$)").matcher(buf);
            if (workflowMatch.find()) {
                String workflowName = workflowMatch.group(0);
                ruleLists.putIfAbsent(workflowName, new ArrayList<>());
                var rules = Pattern.compile("(?<=\\{)(.+)(?=\\})").matcher(buf);
                rules.find();
                buf = rules.group(1);
                for (String curCondition : buf.split(",")) {
                    Matcher parser = Pattern.compile("(\\w)([><=])(\\d+):(\\w+)").matcher(curCondition);
                    if (parser.find()) {
                        char propery = parser.group(1).charAt(0);
                        char cmp = parser.group(2).charAt(0);
                        int threshold = Integer.parseInt(parser.group(3));
                        String dest = parser.group(4);
                        ruleLists.get(workflowName).add(new Condition(dest, propery, Comparators.get(cmp), threshold));
                    } else {
                        ruleLists.get(workflowName).add(new Condition(curCondition, '\0', null, 0));
                    }
                }
            } else {
                break;
            }
        }
        for (String buf; (buf = input.readLine()) != null;) {
            Matcher PartMatch = Pattern.compile("x=(\\d+),m=(\\d+),a=(\\d+),s=(\\d+)").matcher(buf);
            PartMatch.find();
            Stream<String> contents = IntStream.rangeClosed(1, PartMatch.groupCount()).<String> mapToObj(PartMatch::group);
            PartList.add(new Part(contents.collect(Collectors.joining(" "))));
        }
        input.close();
    }

    void Simulate(Part p) {
        String curDest = "in";
        while (!curDest.startsWith("R") && !curDest.startsWith("A")) {
            int i = 0;
            var curRuleList = ruleLists.get(curDest);
            for (Condition it : ruleLists.get(curDest)) {
                if (i == curRuleList.size() - 1 || it.cmp.compare(p.get(it.property), it.threshold) > 0) {
                    curDest = it.dest;
                    break;
                }
                i++;
            }
        }
        if (curDest.equals("A")) {
            acceptedList.add(p);
        }
    }

    class Constraint {
        HashMap<Character, Pair<Integer, Integer>> propConstraints = new HashMap<>();

        public Constraint() {
        }

        public Constraint addPair(Character prop, Integer l, Integer r) {
            propConstraints.put(prop, new Pair<>(l, r));
            return this;
        }

        public Optional<Constraint> joined(Condition rhs) {
            Pair<Integer, Integer> rhsItv = getItv(rhs);
            return joinedAt(rhs.property, rhsItv);
        }

        public Optional<Constraint> excluded(Condition rhs) {
            Pair<Integer, Integer> rhsItv = getItv(rhs);
            if (rhsItv.first.equals(1)) {
                return joinedAt(rhs.property, new Pair<>(rhs.threshold, 4000));
            } else { // if ( rhsItv.second == 4000 )
                return joinedAt(rhs.property, new Pair<>(1, rhs.threshold));
            }
        }

        public long CountProp() {
            return propConstraints.entrySet().stream().mapToLong(entry -> entry.getValue().second - entry.getValue().first + 1).reduce(1L, (init, value) -> init * value);
        }

        Optional<Constraint> joinedAt(char prop, Pair<Integer, Integer> rhsItv) {
            var curItv = propConstraints.get(prop);
            int l = Math.max(curItv.first, rhsItv.first), r = Math.min(curItv.second, rhsItv.second);
            if (l > r) {
                return Optional.empty();
            }
            return Optional.of(ReplacedAt(prop, new Pair<>(l, r)));
        }

        Constraint(Constraint constraint) {
            constraint.propConstraints.forEach((k, v) -> propConstraints.put(k, new Pair<>(v.first, v.second)));
        }

        Constraint ReplacedAt(char prop, Pair<Integer, Integer> itv) {
            var tmp = new Constraint(this);
            tmp.propConstraints.put(prop, itv);
            return tmp;
        }

        Pair<Integer, Integer> getItv(Condition rhs) {
            if (rhs.cmp.compare(0, rhs.threshold) > 0) { // cmp is less
                return new Pair<>(1, rhs.threshold - 1);
            } else { // if ( rhs.cmp( 4001, rhs.threshold ) ) cmp is greater
                return new Pair<>(rhs.threshold + 1, 4000);
            }
        }
    };

    Stream<Constraint> BFSPathList() {
        List<Constraint> res = new ArrayList<>();
        Queue<Pair<String, Constraint>> q = new LinkedList<>();
        q.add(new Pair<>("in",
                (new Constraint())
                        .addPair('x', 1, 4000)
                        .addPair('m', 1, 4000)
                        .addPair('a', 1, 4000)
                        .addPair('s', 1, 4000)));
        while (!q.isEmpty()) {
            var top = q.poll();
            var curWkflN = top.first;
            var curCstr = top.second;
            if (curWkflN.equals("A")) {
                res.add(curCstr);
                continue;
            }
            if (ruleLists.containsKey(curWkflN)) {
                var CdtList = ruleLists.get(curWkflN);
                int i = 0;
                Optional<Constraint> flowCstr = Optional.of(curCstr);
                for (; i < CdtList.size() - 1 && flowCstr.isPresent(); i++) {
                    Condition itr = CdtList.get(i);
                    var nCstr = flowCstr.get().joined(itr);
                    if (nCstr.isPresent()) {
                        q.add(new Pair<>(itr.dest, nCstr.get()));
                    }
                    flowCstr = flowCstr.get().excluded(itr);
                }
                if (i == CdtList.size() - 1 && flowCstr.isPresent()) {
                    Condition itr = CdtList.get(i);
                    q.add(new Pair<>(itr.dest, flowCstr.get()));
                }
            }
        }
        return res.stream();
    }

    public void Solution1() throws IOException {
        readFile();
        PartList.forEach(p -> Simulate(p));
        System.out.println("Solution 1: " + acceptedList.stream().mapToLong(Part::rating).sum());
    }

    public void Solution2() {
        System.out.println("Solution 2: " + BFSPathList().mapToLong(Constraint::CountProp).sum());
    }

    public static void main(String[] args) throws IOException {
        var now = System.nanoTime();
        WorkflowSimulation Day19 = new WorkflowSimulation();
        Day19.Solution1();
        Day19.Solution2();
        var end = System.nanoTime();
        System.out.println((end - now) / 1000000);
    }
}