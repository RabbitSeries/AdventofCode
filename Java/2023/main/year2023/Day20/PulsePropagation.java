package year2023.Day20;

import java.io.*;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import JavaDataModel.*;

@AoCSolution()
public class PulsePropagation implements SolutionBase {
    enum ModuleType {
        Conjunction, BroadCaster, FlipFlop
    }

    // A module's ModuleType and Optional On/Off status
    HashMap<String, Pair<ModuleType, Optional<Boolean>>> Modules = new HashMap<>();

    // A module's output module namelist
    HashMap<String, List<String>> OutputLists = new HashMap<>();

    // Conjunction Module's input module status memory
    HashMap<String, HashMap<String, Boolean>> CjctModuleInputStatus = new HashMap<>();

    void readFile() throws IOException {
        BufferedReader input = new BufferedReader(new FileReader("Day20/input.txt"));
        Pattern WirePattern = Pattern.compile("(?<InWire>.+)->(?<OutWireList>.+)");
        for (String buf; (buf = input.readLine()) != null;) {
            Matcher WireMatch = WirePattern.matcher(buf);
            if (WireMatch.find()) {
                String InWire = WireMatch.group("InWire").trim();
                List<String> OutWireList = Arrays.stream(WireMatch.group("OutWireList").trim().split(",")).map(name -> name.trim()).toList();
                if (InWire.charAt(0) == '%') {
                    InWire = InWire.substring(1);
                    // Is on
                    Modules.putIfAbsent(InWire, new Pair<>(ModuleType.FlipFlop, Optional.of(false)));
                } else if (InWire.charAt(0) == '&') {
                    InWire = InWire.substring(1);
                    Modules.putIfAbsent(InWire, new Pair<>(ModuleType.Conjunction, Optional.empty()));
                } else {
                    Modules.putIfAbsent(InWire, new Pair<>(ModuleType.BroadCaster, Optional.empty()));
                }
                OutputLists.computeIfAbsent(InWire, s -> new ArrayList<>()).addAll(OutWireList);
                for (String s : OutWireList) {
                    CjctModuleInputStatus.computeIfAbsent(s, __ -> new HashMap<>()).putIfAbsent(InWire, false);
                }
            }
        }
        input.close();
    }

    // List<Boolean> pulseList;

    Pair<Long, Long> PulseSimulate(String From, boolean stat, String To) {
        // pulseList = new ArrayList<>();
        Queue<Pair<String, Boolean>> q = new LinkedList<>();
        q.add(new Pair<>(From, stat));
        long lowCnt = 1, highCnt = 0;
        while (!q.isEmpty()) {
            var curPulse = q.poll();
            for (String outWire : OutputLists.get(curPulse.first)) {
                if (!curPulse.second) {
                    lowCnt++;
                } else {
                    highCnt++;
                }
                if (Modules.containsKey(outWire)) {
                    Optional<Boolean> nextPulse = Optional.empty();
                    if (Modules.get(outWire).first.equals(ModuleType.Conjunction)) {
                        if (curPulse.first.equals(To)) {
                            // pulseList.add(curPulse.second);
                            continue;// Forbit the conjunction module's propagation to next conjunction module
                        }
                        CjctModuleInputStatus.get(outWire).put(curPulse.first, curPulse.second);
                        nextPulse = Optional.of(!CjctModuleInputStatus.get(outWire).values().stream().reduce(true, (init, e) -> init && e));
                    } else {// if (Modules.get(outWire).first.equals(ModuleType.FlipFlop)) {
                        if (curPulse.second == false) {// low pulse
                            // Filp
                            Modules.get(outWire).second = Optional.of(!Modules.get(outWire).second.get());
                            nextPulse = Modules.get(outWire).second;
                        }
                    }
                    if (nextPulse.isPresent()) {
                        q.add(new Pair<>(outWire, nextPulse.get()));
                    }
                }

            }
        }
        return new Pair<Long, Long>(lowCnt, highCnt);
    }

    public void Solution1() throws IOException {
        readFile();
        long lowRes = 0, highRes = 0;
        for (int i = 0; i < 1000; i++) {
            var res = PulseSimulate("broadcaster", false, null);
            lowRes += res.first;
            highRes += res.second;
        }
        System.out.println("Solution 1: " + lowRes * highRes);
    }

    boolean SystemReseted(String InModule, String OutModule) {
        Queue<String> q = new LinkedList<>();
        HashSet<String> visitedModule = new HashSet<>();
        q.add(InModule);
        visitedModule.add(InModule);
        while (!q.isEmpty()) {
            String curModuleName = q.poll();
            var curModule = Modules.get(curModuleName);
            if (curModule.first.equals(ModuleType.FlipFlop)) {
                if (curModule.second.get().compareTo(true) == 0) {
                    return false;
                }
            } else {
                for (var InputStatus : CjctModuleInputStatus.get(curModuleName).entrySet()) {
                    if (InputStatus.getValue().compareTo(true) == 0) {
                        return false;
                    }
                }
            }
            if (!curModuleName.equals(OutModule)) {
                for (String outModuleName : OutputLists.get(curModuleName)) {
                    if (!visitedModule.contains(outModuleName)) {
                        q.add(outModuleName);
                        visitedModule.add(outModuleName);
                    }
                }
            }
        }
        return true;
    }

    public void Solution2() {
        List<String> BranchNameList = OutputLists.get("broadcaster");
        HashMap<String, String> BranchOutputName = new HashMap<>();
        for (String branchName : BranchNameList) {
            for (String output : OutputLists.get(branchName)) {
                if (Modules.get(output).first.equals(ModuleType.Conjunction)) {
                    BranchOutputName.put(branchName, output);
                    break;
                }
            }
        }
        // List<Pair<List<Boolean>, Integer>> signals = new ArrayList<>();
        HashMap<String, Integer> ChaosIndexList = new HashMap<>();
        for (String branchName : BranchNameList) {
            String To = BranchOutputName.get(branchName);
            int push = 0;
            do {
                Modules.get(branchName).second = Optional.of(!Modules.get(branchName).second.get());
                PulseSimulate(branchName, Modules.get(branchName).second.get(), To);
                push++;
                // signals.add(new Pair<>(pulseList, push));
            } while (!SystemReseted(branchName, To));
            // signals.forEach(p -> {
            // for (int at = 0; at < p.first.size(); at++) {
            // if (!p.first.get(at)) {
            // lowCnt++;
            // LowPulseAt.first = p.second;
            // LowPulseAt.second = at;
            // } else {
            // hightCnt++;
            // }
            // }
            // });
            // signals.clear();
            // System.out.println("Low " + lowCnt + " High " + hightCnt + " Push loop at " + push);
            // System.out.println("\t\tLow pulse at " + LowPulseAt.first + " Push " + LowPulseAt.second + "th Pulse\n");
            // lowCnt = 0;
            // hightCnt = 0;
            ChaosIndexList.put(branchName, 1000 + push);
        }
        System.out.println("Solution 2: " + ChaosIndexList.values().stream().mapToLong(l -> (long) l).reduce(1L, (init, l) -> lcm(init, l)));
    }

    int lowCnt = 0, hightCnt = 0;

    Pair<Integer, Integer> LowPulseAt = new Pair<>(0, 0);

    long gcd(long a, long b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    long lcm(long a, long b) {
        return a / gcd(a, b) * b;
    }

    public static void main(String[] args) throws IOException {
        PulsePropagation Day20 = new PulsePropagation();
        Day20.Solution1();
        Day20.Solution2();
    }

}