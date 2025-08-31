package year2023.Day24;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.stream.IntStream;

import org.hipparchus.util.*;

import JavaDataModel.AoCSolution;
import JavaDataModel.ISolution;
import year2023.Legacy.Day24.HailIntersection_LinearAlgebra;

@AoCSolution()
public class HailIntersection implements ISolution {

    Hail ParseHail(String s) {
        var InputSplit = s.split("@");
        return new Hail(
                Arrays.stream(InputSplit[0].trim().split("\\s*,\\s*")).map(Long::parseLong).toList(),
                Arrays.stream(InputSplit[1].trim().split("\\s*,\\s*")).map(Long::parseLong).toList());
    }

    List<Hail> HailList = null;

    void readFile(BufferedReader input) throws Exception {
        HailList = input.lines().map(this::ParseHail).toList();
    }

    final long L = 200000000000000L, R = 400000000000000L;

    boolean inBoundary(Optional<Pair<Double, Double>> candidate) {
        if (candidate.isPresent()) {
            var p = candidate.get();
            return p.getFirst() >= L && p.getFirst() <= R && p.getSecond() >= L && p.getSecond() <= R;
        }
        return false;
    }

    public void Solution1(BufferedReader input) throws Exception {
        readFile(input);
        System.out.println("Solution 1: " + IntStream.range(0, HailList.size()).mapToLong(i -> IntStream.range(i + 1, HailList.size())
                .mapToObj(j -> HailList.get(i).Intersection(HailList.get(j)))
                .filter(this::inBoundary)
                .count()).sum());
    }

    /**
     * Currently using hipparchus MultivariateJacobianFunction as a solution
     * @see HailIntersection_LinearAlgebra#Solution2() Legacy.Day24.HailIntersection_LinearAlgbra
     */
    public void Solution2(BufferedReader input) throws Exception {
        HailIntersection_LinearAlgebra legacy = new HailIntersection_LinearAlgebra();
        legacy.Solution2(HailList);
    }

    public static void main(String[] args) throws Exception {
        HailIntersection Day24 = new HailIntersection();
        Day24.Solution1(new BufferedReader(new FileReader("Day24/input.txt")));
        Day24.Solution2(new BufferedReader(new FileReader("Day24/input.txt")));
    }
}
