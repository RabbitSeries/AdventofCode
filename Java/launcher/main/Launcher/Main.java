package Launcher;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.lang.reflect.Method;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;


public class Main {
    public static void main(String[] args) throws Exception {
        var start_time = System.nanoTime();
        if (args.length == 0) {
            System.err.println("Please specify JAR URL/PATH");
        }
        for (String arg : args) {
            List<Class<?>> Sources = AoCSolutionLoader.loadCodeSource(Paths.get(arg).normalize().toUri().toURL());
            if (Sources == null) {
                throw new RuntimeException("Please launch from a Jar bundle");
            }
            for (Class<?> clazz : Sources) {
                try {
                    System.out.println();
                    System.out.println(clazz.getName());
                    Object instance = clazz.getConstructor().newInstance();
                    for (String methodName : new String[] {
                            "Solution1", "Solution2"
                    }) {
                        try {
                            Method method = clazz.getMethod(methodName);
                            System.out.print("\t");
                            method.invoke(instance);
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
        }
        var end_time = System.nanoTime();
        BufferedWriter cost = null;
        try {
            cost = new BufferedWriter(new FileWriter("exception.log"));
            cost.write("Cost: " + (end_time - start_time) / 1000 / 1_000_000.0);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (cost != null) {
                cost.close();
            }
        }
    }

    @SuppressWarnings("unused")
    @Deprecated(forRemoval = true)
    private static List<String> findClassFiles() {
        File root = new File(".");
        String rootPath = root.getAbsoluteFile().getParentFile().getParent();
        Queue<File> q = new LinkedList<>(List.of(root));
        List<String> classList = new ArrayList<>();
        while (!q.isEmpty()) {
            File current = q.poll();
            for (File file : current.listFiles()) {
                if (file.isDirectory()) {
                    q.add(file);
                } else if (file.getName().endsWith(".java")) {
                    String path = Paths.get(file.getAbsolutePath()).toAbsolutePath().normalize().toString()
                            .replace(rootPath + File.separator, "")
                            .replace(File.separator, ".")
                            .replaceAll("\\.java$", "");
                    // var matcher =
                    // Pattern.compile("Day[0-9]+\\.(?<ClassName>[^\\.]+)").matcher(path);
                    if (path.matches("year[0-9]+\\.Day[0-9]+\\.(?<ClassName>[^\\.]+)$")) {
                        classList.add(path);
                    }
                }

            }
        }
        return classList;
    }
}
