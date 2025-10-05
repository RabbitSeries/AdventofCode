package year2023.Day20

import java.io.*
import java.util.*
import java.util.regex.Matcher
import java.util.regex.Pattern

import JavaDataModel.ISolution
import JavaDataModel.AoCSolution

@AoCSolution()
class PulsePropagation : ISolution {
    enum class ModuleType {
        Conjunction, BroadCaster, FlipFlop
    }

    // A module's ModuleType and Optional On/Off status
    val Modules: HashMap<String, Pair<ModuleType, Boolean?>> = HashMap()

    // A module's output module namelist
    val OutputLists: HashMap<String, MutableList<String>> = HashMap()

    // Conjunction Module's input module status memory
    val CjctModuleInputStatus: HashMap<String, HashMap<String, Boolean>> = HashMap()

    fun readFile(input: BufferedReader) {
        val WirePattern = Pattern.compile("(?<InWire>.+)->(?<OutWireList>.+)")
        for (buf in input.lineSequence()) {
            val WireMatch: Matcher = WirePattern.matcher(buf)
            if (WireMatch.find()) {
                var InWire = WireMatch.group("InWire").trim()
                val OutWireList = WireMatch.group("OutWireList").trim().split(",").asSequence()
                    .map { it.trim() }.toList()
                if (InWire[0] == '%') {
                    InWire = InWire.substring(1)
                    // Is on
                    Modules.putIfAbsent(InWire, Pair(ModuleType.FlipFlop, false))
                } else if (InWire[0] == '&') {
                    InWire = InWire.substring(1)
                    Modules.putIfAbsent(InWire, Pair(ModuleType.Conjunction, null))
                } else {
                    Modules.putIfAbsent(InWire, Pair(ModuleType.BroadCaster, null))
                }
                OutputLists.computeIfAbsent(InWire) { s -> ArrayList() }.addAll(OutWireList)
                for (s in OutWireList) {
                    CjctModuleInputStatus.computeIfAbsent(s) { _ -> HashMap() }.putIfAbsent(InWire, false)
                }
            }
        }
        input.close()
    }

    // List<Boolean> pulseList

    fun PulseSimulate(From: String, stat: Boolean, To: String?): Pair<Long, Long> {
        // pulseList =  ArrayList()
        val q: Queue<Pair<String, Boolean>> = LinkedList()
        q.add(Pair(From, stat))
        var (lowCnt, highCnt) = Pair(1L, 0L)
        while (!q.isEmpty()) {
            val curPulse = q.poll()
            for (outWire in OutputLists[curPulse.first] ?: listOf()) {
                if (!curPulse.second) {
                    lowCnt++
                } else {
                    highCnt++
                }
                if (Modules.containsKey(outWire)) {
                    var nextPulse: Boolean? = null
                    if (Modules[outWire]!!.first == ModuleType.Conjunction) {
                        if (curPulse.first == To) {
                            // pulseList.add(curPulse.second)
                            continue// Forbit the conjunction module's propagation to next conjunction module
                        }
                        CjctModuleInputStatus[outWire]!![curPulse.first] = curPulse.second
                        nextPulse = !CjctModuleInputStatus[outWire]!!.values.asSequence()
                            .fold(true) { init, e -> init && e }
                    } else {// if (Modules.get(outWire).first.equals(ModuleType.FlipFlop)) {
                        if (!curPulse.second) {// low pulse
                            // Flip
                            Modules[outWire] = Pair(Modules[outWire]!!.first, !Modules[outWire]!!.second!!)
                            nextPulse = Modules[outWire]!!.second
                        }
                    }
                    if (nextPulse != null) {
                        q.add(Pair(outWire, nextPulse))
                    }
                }

            }
        }
        return Pair(lowCnt, highCnt)
    }

    override fun Solution1(input: BufferedReader) {
        readFile(input)
        var lowRes = 0L
        var highRes = 0L
        for (i in 0 until 1000) {
            val res = PulseSimulate("broadcaster", false, null)
            lowRes += res.first
            highRes += res.second
        }
        println("Solution 1: ${lowRes * highRes}")
    }

    fun SystemReset(InModule: String, OutModule: String): Boolean {
        val q: Queue<String> = LinkedList()
        val visitedModule: HashSet<String> = HashSet()
        q.add(InModule)
        visitedModule.add(InModule)
        while (!q.isEmpty()) {
            val curModuleName: String = q.poll()
            val curModule = Modules[curModuleName]!!
            if (curModule.first == ModuleType.FlipFlop) {
                if (curModule.second!!) {
                    return false
                }
            } else {
                for ((_, InputStatus) in CjctModuleInputStatus[curModuleName]!!.entries) {
                    if (InputStatus) {
                        return false
                    }
                }
            }
            if (curModuleName != OutModule) {
                for (outModuleName in OutputLists[curModuleName]!!) {
                    if (!visitedModule.contains(outModuleName)) {
                        q.add(outModuleName)
                        visitedModule.add(outModuleName)
                    }
                }
            }
        }
        return true
    }

    override fun Solution2(input: BufferedReader) {
        val BranchNameList = OutputLists["broadcaster"]!!
        val BranchOutputName: HashMap<String, String> = HashMap()
        for (branchName in BranchNameList) {
            for (output in OutputLists[branchName]!!) {
                if (Modules[output]!!.first == ModuleType.Conjunction) {
                    BranchOutputName.put(branchName, output)
                    break
                }
            }
        }
        // List<Pair<List<Boolean>, Integer>> signals =  ArrayList()
        val ChaosIndexList: HashMap<String, Int> = HashMap()
        for (branchName in BranchNameList) {
            val To = BranchOutputName[branchName]!!
            var push = 0
            do {
                Modules[branchName] = Pair(Modules[branchName]!!.first, !Modules[branchName]!!.second!!)
                PulseSimulate(branchName, Modules[branchName]!!.second!!, To)
                push++
                // signals.add( Pair(pulseList, push))
            } while (!SystemReset(branchName, To))
            // signals.forEach(p -> {
            //     for (int at = 0 at < p.first.size() at++) {
            //         if (!p.first.get(at)) {
            //             lowCnt++
            //             LowPulseAt = (p.second, at)
            //         } else {
            //             highCnt++
            //         }
            //     }
            // })
            // signals.clear()
            // System.out.println("Low " + lowCnt + " High " + hightCnt + " Push loop at " + push)
            // System.out.println("\t\tLow pulse at " + LowPulseAt.first + " Push " + LowPulseAt.second + "th Pulse\n")
            // lowCnt = 0
            // highCnt = 0
            ChaosIndexList.put(branchName, 1000 + push)
        }
        println("Solution 2: " + ChaosIndexList.values.asSequence().map { it.toLong() }
            .fold(1L) { init, l -> lcm(init, l) })
    }

    var lowCnt = 0
    var highCnt = 0

    val LowPulseAt = Pair(0, 0)

    fun gcd(a: Long, b: Long): Long {
        return if (b == 0L) {
            a
        } else {
            gcd(b, a % b)
        }
    }

    fun lcm(a: Long, b: Long): Long {
        return a / gcd(a, b) * b
    }

    companion object {
        @JvmStatic
        fun main(args: Array<String>) {
            PulsePropagation().let { Day20 ->
                BufferedReader(FileReader("Day20/input.txt")).use(Day20::Solution1)
                BufferedReader(FileReader("Day20/input.txt")).use(Day20::Solution2)
            }
        }
    }
}