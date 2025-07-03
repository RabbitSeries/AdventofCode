package Launcher;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.lang.reflect.Method;
import java.nio.file.Paths;
import java.util.List;
import java.util.Map.Entry;
import java.util.jar.JarEntry;

public class Main {
    public static void main(String[] args) throws Exception {
        if (args.length == 0) {
            System.err.println("Please specify JAR URL/PATH");
            return;
        }
        for (String arg : args) {
            var start_time = System.nanoTime();
            List<Entry<Class<?>, JarEntry>> Solutions = null;
            AoCSolutionLoader SolutionLoader = null;
            try {
                SolutionLoader = new AoCSolutionLoader(Paths.get(arg).normalize().toUri().toURL());
                Solutions = SolutionLoader.loadSolutions();
            } catch (Exception e) {
                e.printStackTrace();
                System.out.println("Error loading Solution: " + arg);
                continue;
            }
            for (Entry<Class<?>, JarEntry> solution : Solutions) {
                Class<?> clazz = solution.getKey();
                JarEntry inputResource = solution.getValue();
                try {
                    System.out.println();
                    System.out.println(clazz.getName());
                    Object instance = clazz.getConstructor().newInstance();
                    for (String methodName : new String[] {
                            "Solution1", "Solution2"
                    }) {
                        try {
                            BufferedReader input = new BufferedReader(new InputStreamReader(SolutionLoader.getJarFile().getInputStream(inputResource)));
                            Method method = clazz.getMethod(methodName, input.getClass());
                            System.out.print("\t");
                            method.invoke(instance, input);
                        } catch (Exception e) {
                            System.out.println("Error calling " + clazz.getName() + "." + methodName);
                            e.printStackTrace();
                            break;
                        }
                    }
                } catch (Exception e) {
                    System.err.println("Error running " + clazz.getName());
                    e.printStackTrace();
                }
            }
            var end_time = System.nanoTime();
            BufferedWriter cost = new BufferedWriter(new FileWriter("exception.log", true));
            cost.write("Cost: " + (end_time - start_time) / 1000 / 1_000_000.0);
            cost.newLine();
            cost.close();
        }
    }
}
