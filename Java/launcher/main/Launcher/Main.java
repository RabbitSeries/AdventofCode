package Launcher;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.reflect.Method;
import java.net.URL;
import java.nio.file.Paths;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.PriorityQueue;
import java.util.Map.Entry;
import java.util.concurrent.TimeUnit;
import java.util.jar.JarEntry;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {
    public static void main(String[] args) throws Exception {
        for (String arg : args) {
            RunSolution(Paths.get(arg).normalize().toUri().toURL());
        }
    }

    public static void RunSolution(URL url) throws IOException {
        PriorityQueue<Entry<String, Long>> pq = new PriorityQueue<>(Comparator.<Entry<String, Long>, Long> comparing(Entry::getValue).reversed());
        var start_time = System.nanoTime();
        List<Entry<Class<?>, JarEntry>> Solutions = null;
        AoCSolutionLoader SolutionLoader = null;
        try {
            SolutionLoader = new AoCSolutionLoader(url);
            Solutions = SolutionLoader.loadSolutions();
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Error loading Solution: " + url);
            return;
        }
        for (Entry<Class<?>, JarEntry> solution : Solutions) {
            Class<?> clazz = solution.getKey();
            JarEntry inputResource = solution.getValue();
            long begin = System.nanoTime();
            try {
                String clazzName = clazz.getName();
                System.out.println(clazzName);
                Object instance = clazz.getConstructor().newInstance();
                for (String methodName : List.of("Solution1", "Solution2")) {
                    // BufferedReader is AutoCloseable, and "Closing a previously closed stream has no effect." -- Java8
                    try (BufferedReader input = new BufferedReader(new InputStreamReader(SolutionLoader.getJarFile().getInputStream(inputResource)))) {
                        Method method = clazz.getMethod(methodName, input.getClass());
                        System.out.print("\t");
                        method.invoke(instance, input);
                    } catch (Exception e) {
                        System.err.println("Error calling " + clazz.getName() + "." + methodName);
                        e.printStackTrace();
                        break;
                    }
                }
                System.out.println();
                long end = System.nanoTime();
                pq.add(Map.entry(clazzName, TimeUnit.NANOSECONDS.toMillis(end - begin)));
            } catch (Exception e) {
                System.err.println("Error running " + clazz.getName());
                e.printStackTrace();
            }
        }
        var end_time = System.nanoTime();
        try (BufferedWriter cost = new BufferedWriter(new FileWriter("exception.log", true))) {
            cost.write("Done solving year package " + url.toString());
            cost.newLine();
            cost.write(Stream.generate(() -> {
                return pq.poll();
            }).takeWhile(Objects::nonNull).map(K_V -> K_V.getKey() + ":\n" + "\t" + K_V.getValue() + " ms").collect(Collectors.joining("\n")));
            cost.newLine();
            cost.write("Totoal Cost: " + (end_time - start_time) / 1000 / 1_000_000.0);
            cost.newLine();
            cost.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
