package year2023.Day15

import org.mockito.Mock
import org.mockito.Mockito
import org.testng.annotations.Listeners
import org.testng.annotations.Test
import org.mockito.testng.MockitoTestNGListener

@Listeners(MockitoTestNGListener::class)
class StringHasherTest {
    @Mock
    lateinit var lensPosList: Map<String, Pair<Int, Int>>

    // @InjectMocks
    // private lateinit var someType: SomeType

    @Test
    fun PrintRes() {
        Mockito.`when`(lensPosList.entries).thenReturn(mapOf("foo" to Pair(1, 2)).entries)
        val output = lensPosList.entries.groupBy { hash(it.key) }.entries.sortedBy { it.key }.map { it.value }
        for (curBox in 0.rangeUntil(output.size)) {
            val curPrintBox = output[curBox]
            println("Box " + hash(curPrintBox.first().key) + ": ")
            curPrintBox.sortedBy { it.value.first }
            curPrintBox.forEach { it -> print("${it.value.first}: [${it.key} ${it.value.second}]") }
            println()
        }
    }

}
