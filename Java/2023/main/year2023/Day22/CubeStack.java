package year2023.Day22;

import java.io.BufferedReader;
// import java.io.BufferedWriter;
import java.io.FileReader;
// import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Optional;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Set;
import java.util.stream.IntStream;

import JavaDataModel.*;

@AoCSolution(day = 22)
public class CubeStack implements ISolution {
    final int EMPTY = -1;

    class Point3D extends TernaryTuple<Integer, Integer, Integer> {
        public Point3D(TernaryTuple<Integer, Integer, Integer> rhs) {
            super(rhs.getE1(), rhs.getE2(), rhs.getE3());
        }

        public Point3D(Integer e1, Integer e2, Integer e3) {
            super(e1, e2, e3);
        }

        public Point3D minus(Point3D rhs) {
            return new Point3D(getE1() - rhs.getE1(), getE2() - rhs.getE2(), getE3() - rhs.getE3());
        }
    }

    class Cube {
        public Cube(Point3D start, Point3D end) {
            Start = start;
            End = end;
            Delta = End.minus(Start);
        }

        int id = EMPTY;

        public int getId() {
            return this.id;
        }

        public void setId(int id) {
            this.id = id;
        }

        public int getHeight() {
            return Math.min(Start.getE3(), End.getE3());
        }

        public void Fall() {
            Start.setE3(Start.getE3() - 1);
            End.setE3(End.getE3() - 1);
        }

        Point3D Start, End, Delta;

        boolean supported = false;

        Set<Integer> supportedByCubes = new HashSet<>(), supportingCubes = new HashSet<>();

        public Set<Integer> getSupportingCubes() {
            return this.supportingCubes;
        }

        public void setSupportingCubes(Set<Integer> supportingCube) {
            this.supportingCubes = supportingCube;
        }

        public Set<Integer> getSupportedByCubes() {
            return this.supportedByCubes;
        }

        public void setSupportedByCubes(Set<Integer> supportedCubes) {
            this.supportedByCubes = supportedCubes;
        }

        public boolean isSupported() {
            return this.supported;
        }

        public void setSupported(boolean supported) {
            this.supported = supported;
        }
    }

    public Cube ParseCube(String buf) {
        var parse = Arrays.stream(buf.split("~")).map(s -> {// Lazy computation
            var coordinates = Arrays.stream(s.trim().split(",")).map(Integer::parseInt).iterator();// Lazy iterator
            return new Point3D(coordinates.next(), coordinates.next(), coordinates.next());
        }).toList();// Real computation
        Cube cube = new Cube(parse.get(0), parse.get(1));
        return cube;
    }

    List<Cube> CubeList = new ArrayList<>();

    List<List<List<Integer>>> Space = new ArrayList<>();

    Point3D Volume = new Point3D(0, 0, 0);

    void readFile(BufferedReader input) throws Exception {
        CubeList = input.lines().map(this::ParseCube).toList();
        IntStream.range(0, CubeList.size()).forEach(i -> {
            Cube cube = CubeList.get(i);
            cube.setId(i);
            Volume.setE1(Math.max(Volume.getE1(), cube.End.getE1()));
            Volume.setE1(Math.max(Volume.getE1(), cube.Start.getE1()));
            Volume.setE2(Math.max(Volume.getE2(), cube.Start.getE2()));
            Volume.setE2(Math.max(Volume.getE2(), cube.End.getE2()));
            Volume.setE3(Math.max(Volume.getE3(), cube.Start.getE3()));
            Volume.setE3(Math.max(Volume.getE3(), cube.End.getE3()));
        });
        IntStream.rangeClosed(0, Volume.getE1()).forEach(x -> {
            Space.add(new ArrayList<>());
            IntStream.rangeClosed(0, Volume.getE2()).forEach(y -> {
                Space.get(x).add(new ArrayList<>());
                // Space.get(x).set(y, new ArrayList<>(Volume.getE3() + 1)); The initcapacity list is still empty.
                IntStream.rangeClosed(0, Volume.getE3()).forEach(l -> Space.get(x).get(y).add(EMPTY));
            });
        });
        CubeList.forEach(cube -> {
            if (!cube.Delta.getE1().equals(0)) {
                getRange(cube.Start.getE1(), cube.End.getE1()).forEach(x -> {
                    Space.get(x).get(cube.Start.getE2()).set(cube.Start.getE3(), cube.getId());
                });
            } else if (!cube.Delta.getE2().equals(0)) {
                getRange(cube.Start.getE2(), cube.End.getE2()).forEach(y -> {
                    Space.get(cube.Start.getE1()).get(y).set(cube.Start.getE3(), cube.getId());
                });
            } else {// Z Align or single cube
                getRange(cube.Start.getE3(), cube.End.getE3()).forEach(z -> {
                    Space.get(cube.Start.getE1()).get(cube.Start.getE2()).set(z, cube.getId());
                });
            }
        });
    }

    // void ProjectY() throws Exception {
    //     try (BufferedWriter output = new BufferedWriter(new FileWriter("ProjectY.txt"))) {
    //         List<List<Character>> Projection = new ArrayList<>(); // X,Z
    //         IntStream.rangeClosed(0, Volume.getE1()).forEach(x -> {
    //             Projection.add(new ArrayList<>());
    //             IntStream.rangeClosed(0, Volume.getE3()).forEach(l -> {
    //                 Projection.get(x).add('.');
    //             });
    //         });
    //         for (int x : IntStream.rangeClosed(0, Volume.getE1()).toArray()) {
    //             for (int z : IntStream.rangeClosed(0, Volume.getE3()).toArray()) {
    //                 for (int y : IntStream.rangeClosed(0, Volume.getE2()).toArray()) {
    //                     if (Space.get(x).get(y).get(z) != 0) {
    //                         Projection.get(x).set(z, (char) (Space.get(x).get(y).get(z) + 'A'));
    //                         break;
    //                     }
    //                 }
    //             }
    //         }
    //         for (int z : IntStream.rangeClosed(0, Volume.getE3()).boxed().sorted(Comparator.reverseOrder()).mapToInt(Integer::intValue).toArray()) {
    //             for (int x : IntStream.rangeClosed(0, Volume.getE1()).toArray()) {
    //                 output.append(Projection.get(x).get(z));
    //             }
    //             output.newLine();
    //         }
    //         output.close();
    //     }
    // }

    // void ProjectX() throws Exception {
    //     try (BufferedWriter output = new BufferedWriter(new FileWriter("ProjectX.txt"))) {
    //         List<List<Character>> Projection = new ArrayList<>(); // Y,Z
    //         IntStream.rangeClosed(0, Volume.getE2()).forEach(y -> {
    //             Projection.add(new ArrayList<>());
    //             IntStream.rangeClosed(0, Volume.getE3()).forEach(l -> {
    //                 Projection.get(y).add('.');
    //             });
    //         });
    //         for (int y : IntStream.rangeClosed(0, Volume.getE2()).toArray()) {
    //             for (int z : IntStream.rangeClosed(0, Volume.getE3()).toArray()) {
    //                 for (int x : IntStream.rangeClosed(0, Volume.getE1()).boxed().sorted(Comparator.comparing(Integer::intValue).reversed()).toList()) {
    //                     if (Space.get(x).get(y).get(z) != 0) {
    //                         Projection.get(y).set(z, (char) (Space.get(x).get(y).get(z) + 'A'));
    //                         break;
    //                     }
    //                 }
    //             }
    //         }
    //         for (int z : IntStream.rangeClosed(0, Volume.getE3()).boxed().sorted(Comparator.reverseOrder()).mapToInt(Integer::intValue).toArray()) {
    //             for (int y : IntStream.rangeClosed(0, Volume.getE2()).toArray()) {
    //                 output.append(Projection.get(y).get(z));
    //             }
    //             output.newLine();
    //         }
    //         output.close();
    //     }
    // }

    List<Integer> getRange(int a, int b) {
        return IntStream.rangeClosed(Math.min(a, b), Math.max(a, b)).collect(ArrayList::new, ArrayList::add, ArrayList::addAll);
    }

    Cube getCube(int cubeId) {
        return CubeList.get(cubeId);
    }

    Optional<Cube> FallCube(Cube cube) {
        int h = cube.getHeight();
        Optional<Cube> support = Optional.empty();
        if (!cube.Delta.getE1().equals(0)) {
            int y = cube.Start.getE2();
            for (int x : getRange(cube.Start.getE1(), cube.End.getE1())) {
                int supportSpace = Space.get(x).get(y).get(h - 1);
                if (supportSpace >= 0) {
                    support = Optional.of(getCube(supportSpace));
                    BuildConnection(cube, support.get());
                }
            }
            if (support.isPresent()) {
                return Optional.empty();
            }
            for (int x : getRange(cube.Start.getE1(), cube.End.getE1())) {
                Space.get(x).get(y).set(h, EMPTY);
                Space.get(x).get(y).set(h - 1, cube.getId());
            }
        } else if (!cube.Delta.getE2().equals(0)) {
            int x = cube.Start.getE1();
            for (int y : getRange(cube.Start.getE2(), cube.End.getE2())) {
                int supportSpace = Space.get(x).get(y).get(h - 1);
                if (supportSpace >= 0) {
                    support = Optional.of(getCube(supportSpace));
                    BuildConnection(cube, support.get());
                }
            }
            if (support.isPresent()) {
                return Optional.empty();
            }
            for (int y : getRange(cube.Start.getE2(), cube.End.getE2())) {
                Space.get(x).get(y).set(h, EMPTY);
                Space.get(x).get(y).set(h - 1, cube.getId());
            }
        } else {
            int x = cube.Start.getE1();
            int y = cube.Start.getE2();
            int supportSpace = Space.get(x).get(y).get(h - 1);
            if (supportSpace >= 0) {
                BuildConnection(cube, getCube(supportSpace));
                return Optional.empty();
            }
            Space.get(x).get(y).set(Math.max(cube.Start.getE3(), cube.End.getE3()), EMPTY);
            Space.get(x).get(y).set(h - 1, cube.getId());
        }
        cube.Fall();
        return Optional.of(cube);
    }

    void BuildConnection(Cube supported, Cube supporting) {
        supporting.getSupportingCubes().add(supported.getId());
        supported.getSupportedByCubes().add(supporting.getId());
        supported.setSupported(true);
    }

    void SetGravity() {
        PriorityQueue<Cube> pq = new PriorityQueue<>(Comparator.comparing(Cube::getHeight));
        pq.addAll(CubeList);
        while (!pq.isEmpty()) {
            Cube cube = pq.poll();
            if (cube.getHeight() == 1) {
                cube.setSupported(true);
                continue;
            }
            Optional<Cube> nextFall = FallCube(cube);
            if (nextFall.isPresent())
                pq.add(nextFall.get());
        }
    }

    public void Solution1(BufferedReader input) throws Exception {
        readFile(input);
        // ProjectY();
        // ProjectX();
        SetGravity();
        // ProjectY();
        // ProjectX();
        System.out.println("Solution 1: " + CubeList.stream().filter(cube -> {
            for (int i : cube.getSupportingCubes()) {
                if (CubeList.get(i).getSupportedByCubes().size() <= 1) {
                    return false;
                }
            }
            // System.out.println((char) (cube.getId() + 'A'));
            return true;
        }).count());
    }

    HashMap<Integer, Integer> NodeCnt = new HashMap<>();

    public void Solution2(BufferedReader input) throws Exception {
        List<Integer> InDegree = new ArrayList<>();
        Queue<Cube> q = new LinkedList<>();
        q.addAll(CubeList.stream().filter(cube -> cube.getSupportingCubes().size() > 0).toList());
        int res = 0;
        while (!q.isEmpty()) {
            Cube disintergrated = q.poll();
            Queue<Cube> topoQ = new LinkedList<>();
            topoQ.add(disintergrated);
            InDegree.clear();
            InDegree.addAll(CubeList.stream().map(cube -> cube.getSupportedByCubes().size()).toList());
            while (!topoQ.isEmpty()) {
                Cube cube = topoQ.poll();
                for (Integer supportingId : cube.getSupportingCubes().stream().toList()) {
                    InDegree.set(supportingId, InDegree.get(supportingId) - 1);
                    if (InDegree.get(supportingId) == 0) {
                        topoQ.add(getCube(supportingId));
                        res++;
                    }
                }
            }
        }
        System.out.println("Solution 2: " + res);
    }

    public static void main(String[] args) throws Exception {
        CubeStack Day22 = new CubeStack();
        Day22.Solution1(new BufferedReader(new FileReader("Day22/input.txt")));
        Day22.Solution2(new BufferedReader(new FileReader("Day22/input.txt")));
    }
}
