package Day20;

import java.io.*;
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
    Pair<Long, Long> PulseSimulate() {
        Queue<Pair<String, Boolean>> q = new LinkedList<>();
        q.add(new Pair<>("broadcaster", false));
        long lowCnt = 1, highCnt = 0;
        while (!q.isEmpty()) {
            var curPulse = q.poll();
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
            var res = PulseSimulate();
            lowRes += res.first;
            highRes += res.second;
        }
        System.out.println("Solution 1: " + lowRes * highRes);
    }

    long rxLowCnt = 0, rxHightCnt = 0;

    void clear() {
        rxLowCnt = 0;
        rxHightCnt = 0;
    }

    public void Solution2() {
        long i = 0;
        for (;; i++) {
            PulseSimulate();
            if (rxLowCnt == 1 && rxHightCnt == 0) {
                break;
            }
            clear();
        }
        System.out.println("Solution 2: " + i);
    }

    public static void main(String[] args) throws IOException {
        PulsePropagation Day20 = new PulsePropagation();
        Day20.Solution1();
        Day20.Solution2();
    }

}