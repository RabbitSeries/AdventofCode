package year2023.Day12;

import java.util.*;

import org.junit.jupiter.api.Test;

public class CharacterMatcherTest {
    CharacterMatcher matcher = new CharacterMatcher();

    @Test
    void unitTest() {
        assert (matcher.DP("?###????????", List.of(3, 2, 1)) == 10);
        assert (matcher.heuristics("?###????????", List.of(3, 2, 1)) == 10);
        assert (matcher.DP("?#??#????.????#", List.of(8, 1, 1)) == 6);
        assert (matcher.heuristics("?#??#????.????#", List.of(8, 1, 1)) == 6);
    }
}
