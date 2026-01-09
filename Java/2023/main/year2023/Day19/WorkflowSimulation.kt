package year2023.Day19

import JavaDataModel.AoCSolution
import JavaDataModel.ISolution
import java.io.BufferedReader
import java.io.FileReader
import java.util.LinkedList
import java.util.Queue
import kotlin.collections.asSequence
import kotlin.contracts.contract
import kotlin.math.max
import kotlin.math.min

@AoCSolution(day = 19)
class WorkflowSimulation : ISolution {

    class Condition(val dest: String, val property: Int, val cmp: Comparator<Int>, val threshold: Int)

    class Part(val prop: List<Int>) {
        val rating get() = prop.asSequence().sum()
    }

    lateinit var ruleLists: Map<String, Pair<List<Condition>, String>>
    lateinit var PartList: List<Part>

    val Comparators = mapOf(
        Pair('<', Comparator.comparing { it }),
        Pair('>', Comparator.comparing<Int, Int> { it }.reversed())
    )

    fun readFile(input: BufferedReader) {
        // Consider the compatability for \r\n and \n
        val (rules, parts) = input.readText().split("""\R{2}""".toRegex()).let { it[0] to it[1] }
        val workflowPattern = """^(\w+)\{(.+)\}$""".toRegex()
        val rulesPattern = """(\w)([<>])(\d+):(\w+)""".toRegex()
        ruleLists =
            rules.lineSequence().mapNotNull { workflowPattern.find(it)?.groupValues }.map { it[1] to it[2].split(',') }
                .map { (workflowName, rules) ->
                    workflowName to (rules.dropLast(1).asSequence().mapNotNull { it ->
                        rulesPattern.find(it)?.groupValues
                    }.map { leadingRules ->
                        val property = PropIndex.getValue(leadingRules[1][0])
                        val cmp = leadingRules[2][0]
                        val threshold = leadingRules[3].toInt()
                        val dest = leadingRules[4]
                        Condition(dest, property, Comparators.getValue(cmp), threshold)
                    }.toMutableList() to rules.last())
                }.toMap()
        """\d+""".toRegex().apply {
            PartList = parts.lineSequence()
                .map { findAll(it).map { res -> res.value.toInt() }.toList() }.filter { !it.isEmpty() }
                .map { Part(it) }.toList()
        }
    }

    tailrec fun Simulate(p: Part, curDest: String): Boolean = when (curDest) {
        "R" -> false
        "A" -> true
        else -> ruleLists.getValue(curDest).let { (conditions, default) ->
            (conditions.firstOrNull { it.cmp.compare(p.prop[it.property], it.threshold) < 0 }?.dest ?: default)
                .let { next -> return Simulate(p, next) }
        }
    }

    class Constraint private constructor(val constraints: List<Pair<Int, Int>>) {
        // this@Constraint.constraints = mutableListOf(*_constraints)
        constructor(vararg ranges: Pair<Int, Int>) : this(ranges.toList())

        // Extension
        private fun Condition.range() = when {
            cmp.compare(0, threshold) < 0 -> 1 to threshold - 1
            else -> threshold + 1 to 4000
        }

        fun ReplacedAt(prop: Int, itv: Pair<Int, Int>): Constraint {
            return Constraint(constraints.mapIndexed { i, original -> if (prop == i) itv else original }.toList())
        }

        fun intersect(a: Pair<Int, Int>, b: Pair<Int, Int>) = max(a.first, b.first) to min(a.second, b.second)

        fun joined(cond: Condition): Constraint? {
            val (l, r) = intersect(constraints[cond.property], cond.range())
            return if (l > r) null else ReplacedAt(cond.property, l to r)
        }

        fun excluded(cond: Condition): Constraint? {
            val (l, r) = intersect(
                constraints[cond.property],
                if (cond.range().first == 1) cond.threshold to 4000 else 1 to cond.threshold
            )
            return if (l > r) null else ReplacedAt(cond.property, l to r)
        }

        val count get() = constraints.asSequence().fold(1L) { init, (l, r) -> init * (r - l + 1) }
    }

    fun BFSPathList(): Sequence<Constraint> = sequence {
        val q: Queue<Pair<String, Constraint>> = LinkedList()
        q += "in" to Constraint((1 to 4000), (1 to 4000), (1 to 4000), (1 to 4000))
        while (q.isNotEmpty()) {
            val (curWkflN, curCstr) = q.poll()
            when {
                curWkflN == "A" -> yield(curCstr)
                else -> ruleLists[curWkflN]?.let { (conditions, default) ->
                    var flowCstr: Constraint? = curCstr
                    for (itr in conditions) {// Kotlin removed for-init-condition-inc loop
                        if (flowCstr == null) {
                            break
                        }
                        flowCstr.joined(itr)?.let { q.add(itr.dest to it) }
                        flowCstr = flowCstr.excluded(itr)
                    }
                    flowCstr?.let { q += (default to flowCstr) }
                }
            }
        }
    }

    override fun Solution1(input: BufferedReader) {
        readFile(input)
        println("Solution 1: ${PartList.asSequence().filter { Simulate(it, "in") }.sumOf { it.rating }}")
    }

    override fun Solution2(input: BufferedReader) {
        println("Solution 2: ${BFSPathList().toList().sumOf { it.count }}")
    }

    companion object {
        val PropIndex = mapOf('x' to 0, 'm' to 1, 'a' to 2, 's' to 3)

        @JvmStatic
        fun main(args: Array<String>) {
            val now = System.nanoTime()
            WorkflowSimulation().apply {
                BufferedReader(FileReader("Day19/input.txt")).use {
                    Solution1(it)
                }
                BufferedReader(FileReader("Day19/input.txt")).use {
                    Solution2(it)
                }
            }
            val end = System.nanoTime()
            println((end - now) / 1000000)
        }
    }
}
