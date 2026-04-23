package year2023.Day20

import java.io.*
import java.util.*

import JavaDataModel.ISolution
import JavaDataModel.AoCSolution

@AoCSolution(day = 20)
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
        val WirePattern = """(?<InWire>.+)->(?<OutWireList>.+)""".toRegex()
        for (buf in input.lineSequence()) {
            val WireMatch = WirePattern.find(buf)?.groups
            if (WireMatch != null) {
                var InWire = WireMatch["InWire"]!!.value.trim()
                val OutWireList = WireMatch["OutWireList"]!!.value.trim().split(",").asSequence()
                    .map { it.trim() }.toList()
                if (InWire[0] == '%') {
                    InWire = InWire.substring(1)
                    // Is on
                    Modules.putIfAbsent(InWire, ModuleType.FlipFlop to false)
                } else if (InWire[0] == '&') {
                    InWire = InWire.substring(1)
                    Modules.putIfAbsent(InWire, ModuleType.Conjunction to null)
                } else {
                    Modules.putIfAbsent(InWire, ModuleType.BroadCaster to null)
                }
                OutputLists.getOrPut(InWire) { ArrayList() } += OutWireList
                for (s in OutWireList) {
                    CjctModuleInputStatus.getOrPut(s) { HashMap() }.putIfAbsent(InWire, false)
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
        var (lowCnt, highCnt) = 1L to 0L
        while (!q.isEmpty()) {
            val curPulse = q.poll()
            for (outWire in OutputLists[curPulse.first] ?: listOf()) {
                if (!curPulse.second) {
                    lowCnt++
                } else {
                    highCnt++
                }
                if (outWire in Modules) {
                    var nextPulse: Boolean? = null
                    if (Modules.getValue(outWire).first == ModuleType.Conjunction) {
                        if (curPulse.first == To) {
                            // pulseList.add(curPulse.second)
                            continue// Forbit the conjunction module's propagation to next conjunction module
                        }
                        CjctModuleInputStatus.getValue(outWire)[curPulse.first] = curPulse.second
                        nextPulse = CjctModuleInputStatus[outWire]!!.values.asSequence().any { !it }
                    } else {// if (Modules.get(outWire).first.equals(ModuleType.FlipFlop)) {
                        if (!curPulse.second) {// low pulse
                            // Flip
                            Modules[outWire] = Pair(Modules[outWire]!!.first, !Modules[outWire]!!.second!!)
                            nextPulse = Modules[outWire]!!.second
                        }
                    }
                    if (nextPulse != null) {
                        q += outWire to nextPulse
                    }
                }

            }
        }
        return lowCnt to highCnt
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
        visitedModule += InModule
        while (!q.isEmpty()) {
            val curModuleName: String = q.poll()
            val curModule = Modules[curModuleName]!!
            if (curModule.first == ModuleType.FlipFlop) {
                if (curModule.second!!) {
                    return false
                }
            } else {
                if (CjctModuleInputStatus[curModuleName]!!.values.any { it }) {
                    return false
                }
            }
            if (curModuleName != OutModule) {
                for (outModuleName in OutputLists[curModuleName]!!) {
                    if (outModuleName !in visitedModule) {
                        q += outModuleName
                        visitedModule += outModuleName
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
                    BranchOutputName[branchName] = output
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

    // var lowCnt = 0
    // var highCnt = 0
    //
    // val LowPulseAt = Pair(0, 0)

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