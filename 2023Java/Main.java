import java.io.File;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import JavaDataModel.SolutionBase;

public class Main {
    public static void main(String[] args) throws Exception {
        File baseDir = new File(".");
        List<String> matchedClasses = findClassFiles(baseDir, baseDir);
        for (String binaryName : matchedClasses) {
            System.out.println();
            try {
                Class<?> clazz = Class.forName(binaryName);
                boolean hasSolutionBase = Arrays.stream(clazz.getInterfaces())
                        .flatMap(itf -> Arrays.stream(itf.getInterfaces()))
                        .reduce(false, (init, v) -> init || v.equals(SolutionBase.class), (init, res) -> init || res);
                if (hasSolutionBase) {
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
    }

    private static List<String> findClassFiles(File root, File current) {
        List<String> classList = new ArrayList<>();
        for (File file : current.listFiles()) {
            if (file.isDirectory()) {
                classList.addAll(findClassFiles(root, file));
            } else if (file.getName().endsWith(".java")) {
                String path = file.getPath()
                        .replace(root.getPath() + File.separator, "")
                        .replace(File.separator, ".")
                        .replaceAll("\\.java$", "");
                // var matcher = Pattern.compile("Day[0-9]+\\.(?<ClassName>[^\\.]+)").matcher(path);
                if (path.matches("Day[0-9]+\\.(?<ClassName>[^\\.]+)")) {
                    classList.add(path);
                }
            }
        }
        return classList;
    }
}
