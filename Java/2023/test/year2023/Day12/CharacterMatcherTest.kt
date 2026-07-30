package year2023.Day12;

import org.testng.annotations.Test;

class CharacterMatcherTest {

    val matcher = CharacterMatcher();

    @Test
    fun unitTest() {
        assert(matcher.DP("?###????????", listOf(3, 2, 1)) == 10L);
        assert(matcher.heuristics("?###????????", listOf(3, 2, 1)) == 10L);
        assert(matcher.DP("?#??#????.????#", listOf(8, 1, 1)) == 6L);
        assert(matcher.heuristics("?#??#????.????#", listOf(8, 1, 1)) == 6L);
    }
}
