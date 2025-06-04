import java.io.File;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

import JavaDataModel.SolutionBase;

public class Main {
    public static void main(String[] args) throws Exception {
        var start_time = System.nanoTime();
        File baseDir = new File(".");
        List<String> matchedClasses = findClassFiles(baseDir);
        matchedClasses.sort(Comparator.<String> naturalOrder());
        for (String binaryName : matchedClasses) {
            try {
                Class<?> clazz = Class.forName(binaryName);
                boolean hasSolutionBase = Arrays.stream(clazz.getInterfaces())
                        .flatMap(itf -> Arrays.stream(itf.getInterfaces()))
                        .reduce(false, (init, v) -> init || v.equals(SolutionBase.class), (init, res) -> init || res);
                if (hasSolutionBase) {
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
                            System.out.println("Error calling " + binaryName);
                            e.printStackTrace();
                        }
                    }
                }
            } catch (Exception e) {
                System.out.println("Error running " + binaryName);
                e.printStackTrace();
            }
        }
        var end_time = System.nanoTime();
        System.out.println("Cost: " + (end_time - start_time) / 1000 / 1_000_000.0);
    }

    private static List<String> findClassFiles(File root) {
        String rootPath = root.getName();
        Queue<File> q = new LinkedList<>(List.of(root));
        List<String> classList = new ArrayList<>();
        while (!q.isEmpty()) {
            File current = q.poll();
            for (File file : current.listFiles()) {
                if (file.isDirectory()) {
                    q.add(file);
                } else if (file.getName().endsWith(".java")) {
                    String path = file.getPath()
                            .replace(rootPath + File.separator, "")
                            .replace(File.separator, ".")
                            .replaceAll("\\.java$", "");
                    // var matcher = Pattern.compile("Day[0-9]+\\.(?<ClassName>[^\\.]+)").matcher(path);
                    if (path.matches("Day[0-9]+\\.(?<ClassName>[^\\.]+)")) {
                        classList.add(path);
                    }
                }

            }
        }
        return classList;
    }
}
