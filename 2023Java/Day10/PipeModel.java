package Day10;

import java.util.*;

public class PipeModel {
    static public HashMap<Character, Map<Integer, Integer>> NextPipeDirection = new HashMap<>(
            Map.of(
                    '|', Map.of(0, 0, 1, 1),
                    '-', Map.of(2, 2, 3, 3),
                    'L', Map.of(1, 3, 2, 0),
                    'J', Map.of(1, 2, 3, 0),
                    '7', Map.of(0, 2, 3, 1),
                    'F', Map.of(0, 3, 2, 1)));

    static public enum ClockOrder {
        ClockWise,
        CounterClockWise
    }

    static public HashMap<Character, Map<Integer, ClockOrder>> ClockOrderQuery = new HashMap<>(
            Map.of(
                    'L', Map.of(1, ClockOrder.CounterClockWise, 2, ClockOrder.ClockWise),
                    'J', Map.of(1, ClockOrder.ClockWise, 3, ClockOrder.CounterClockWise),
                    '7', Map.of(0, ClockOrder.CounterClockWise, 3, ClockOrder.ClockWise),
                    'F', Map.of(1, ClockOrder.ClockWise, 2, ClockOrder.CounterClockWise)));

    // CounterClockWise : left hand
    static public HashMap<Character, Map<Integer, List<Integer>>> CounterclockwiseQuery = new HashMap<>(
            Map.of(
                    '|', Map.of(0, Arrays.asList(3), 1, Arrays.asList(2)),
                    '-', Map.of(2, Arrays.asList(0), 3, Arrays.asList(1)),
                    'L', Map.of(0, Arrays.asList(6), 3, Arrays.asList(1, 2)),
                    'J', Map.of(2, Arrays.asList(4), 0, Arrays.asList(1, 3)),
                    '7', Map.of(1, Arrays.asList(5), 2, Arrays.asList(0, 3)),
                    'F', Map.of(3, Arrays.asList(7), 1, Arrays.asList(0, 2))));

    // Right hand
    static public HashMap<Character, Map<Integer, List<Integer>>> ClockwiseQuery = new HashMap<>(
            Map.of(
                    '|', Map.of(1, Arrays.asList(3), 0, Arrays.asList(2)),
                    '-', Map.of(3, Arrays.asList(0), 2, Arrays.asList(1)),
                    'L', Map.of(3, Arrays.asList(6), 0, Arrays.asList(1, 2)),
                    'J', Map.of(0, Arrays.asList(4), 2, Arrays.asList(1, 3)),
                    '7', Map.of(2, Arrays.asList(5), 1, Arrays.asList(0, 3)),
                    'F', Map.of(1, Arrays.asList(7), 3, Arrays.asList(0, 2))));
}
