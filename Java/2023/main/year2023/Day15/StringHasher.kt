package year2023.Day15

import JavaDataModel.AoCSolution
import JavaDataModel.ISolution
import JavaDataModel.inputFileName
import java.io.BufferedReader
import java.io.FileReader
import java.util.Collections


fun hash(s: String) = s.chars().reduce(0) { init, c -> (init + c) * 17 % 256 }

@AoCSolution(day = 15)
class StringHasher : ISolution {

    lateinit var stepList: List<String>

    fun readFile(input: BufferedReader) {
        stepList = input.lineSequence().flatMap { it.trim().split(",") }.toList()
    }

    fun extractLabel(s: String) = s.split("[=-]")[0]

    override fun Solution1(input: BufferedReader) {
        readFile(input)
        println("Solution 1: " + stepList.asSequence().sumOf { hash(it) })
    }

    override fun Solution2(input: BufferedReader) {
        val BoxList =
            Collections.nCopies(256, mutableListOf<Pair<String, Int>>()).toList() // Label -> Slot,focalLength
        val LensPosList = mutableMapOf<String, Pair<Int, Int>>()
        for (s in stepList) {
            val label = extractLabel(s)
            val SlotList = BoxList[hash(label)]
            if ("=" in s) {
                val focalLength = s.substring(s.indexOf("=") + 1).toInt()
                if (label in LensPosList) {
                    if (LensPosList[label]!!.second != focalLength) {
                        val whichSlot = LensPosList[label]!!.first
                        SlotList[whichSlot] = Pair(label, focalLength)
                        LensPosList.put(label, Pair(whichSlot, focalLength))
                    }
                } else {
                    LensPosList[label] = Pair(SlotList.size, focalLength)
                    SlotList.add(Pair(label, focalLength))
                }
            } else {
                if (label in LensPosList) {
                    val whichSlot = LensPosList[label]!!.first
                    SlotList.removeAt(whichSlot)
                    LensPosList.remove(label)
                    for (i in 0.rangeUntil(SlotList.size)) {
                        LensPosList[SlotList[i].first] = Pair(i, SlotList[i].second)
                    }
                }
            }
        }
        println(
            "Solution 2: " + LensPosList.entries.sumOf { (k, v) -> (hash(k) + 1) * (v.first + 1) * v.second })
    }

    companion object {
        @JvmStatic
        fun main(args: Array<String>) {
            StringHasher().let { solution ->
                BufferedReader(FileReader(solution.inputFileName!!)).use { it ->
                    solution.Solution1(it)
                }
                BufferedReader(FileReader(solution.inputFileName!!)).use { it ->
                    solution.Solution2(it)
                }
            }
        }
    }
}
