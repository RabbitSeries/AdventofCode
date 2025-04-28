package Day20;

import java.io.*;
import java.lang.classfile.instruction.BranchInstruction;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import JavaDataModel.Pair;

public class PulsePropagation {
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
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
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
                OutputLists.putIfAbsent(InWire, new ArrayList<>());
                OutputLists.get(InWire).addAll(OutWireList);
                for (String s : OutWireList) {
                    CjctModuleInputStatus.putIfAbsent(s, new HashMap<>());
                    CjctModuleInputStatus.get(s).putIfAbsent(InWire, false);
                }
            }
        }
        input.close();
    }

    // TODO Add chaos index for System reset flag.
    Pair<Long, Long> PulseSimulate(String From, boolean stat, String To) {
        Queue<Pair<String, Boolean>> q = new LinkedList<>();
        q.add(new Pair<>(From, stat));
        long lowCnt = 1, highCnt = 0;
        while (!q.isEmpty()) {
            var curPulse = q.poll();
            if (To != null && curPulse.first.equals(To)) {
                continue;
            }
            for (String outWire : OutputLists.get(curPulse.first)) {
                if (!curPulse.second) {
                    lowCnt++;
                    if (outWire.equals("rx")) {
                        rxLowCnt++;
                    }
                } else {
                    highCnt++;
                    if (outWire.equals("rx")) {
                        rxHightCnt++;
                    }
                }
                if (Modules.containsKey(outWire)) {
                    Optional<Boolean> nextPulse = Optional.empty();
                    if (Modules.get(outWire).first.equals(ModuleType.Conjunction)) {
                        CjctModuleInputStatus.get(outWire).put(curPulse.first, curPulse.second);
                        nextPulse = Optional.of(!CjctModuleInputStatus.get(outWire).values().stream().reduce(true, (init, e) -> init && e));
                    } else {// if (Modules.get(outWire).first.equals(ModuleType.FlipFlop)) {
                        if (curPulse.second == false) {// low pulse
                            if (Modules.get(outWire).second.get().equals(false)) { // off
                                Modules.get(outWire).second = Optional.of(true);
                                nextPulse = Optional.of(true);
                            } else { // on
                                Modules.get(outWire).second = Optional.of(false);
                                nextPulse = Optional.of(false);
                            }
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

    int SystemChaos(String InModule, String OutModule) {
        int chaos = 0;
        return chaos;
    }

    long rxLowCnt = 0, rxHightCnt = 0;

    void clear() {
        rxLowCnt = 0;
        rxHightCnt = 0;
    }

    public void Solution2() {
        List<String> branchNameList = OutputLists.get("broadcaster");
        HashMap<String, String> branchOutputName = new HashMap<>();
        for (String branchName : branchNameList) {
            Queue<String> q = new LinkedList<>();
            q.add(branchName);
            HashSet<String> visited = new HashSet<>();
            visited.add(branchName);
            while (!q.isEmpty()) {
                String curModule = q.poll();
                if (Modules.get(curModule).first.equals(ModuleType.Conjunction)) {
                    branchOutputName.put(branchName, curModule);
                    break;
                }
                for (String output : OutputLists.get(branchName)) {
                    if (!visited.contains(output)) {
                        q.add(output);
                    }
                }
            }
        }
        HashMap<String, Integer> ChaosIndexList = new HashMap<>();
        for (int i = 0; i < branchNameList.size(); i++) {
            String branchName = branchNameList.get(i);
            Modules.get(branchName).second = Optional.of(!Modules.get(branchName).second.get());
            for (int push = 0;; push++) {
                String To = branchOutputName.get(branchName);
                PulseSimulate(branchName, Modules.get(branchName).second.get(), To);
                if (SystemChaos(branchName, To) == 0) {
                    ChaosIndexList.put(branchName, push);
                    break;
                }
            }
        }
        System.out.println("Solution 2: ");
    }

    public static void main(String[] args) throws IOException {
        PulsePropagation Day20 = new PulsePropagation();
        Day20.Solution1();
        Day20.Solution2();
    }

}