import java.io.File;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.List;

import JavaDataModel.SolutionBase;

public class Main {
    public static void main(String[] args) throws Exception {
        File baseDir = new File("../compileRuntime/bin");
        List<String> matchedClasses = new ArrayList<>();

        findClassFiles(baseDir, baseDir, matchedClasses);

        try (URLClassLoader classLoader = new URLClassLoader(
                new URL[] {
                        baseDir.toURI().toURL()
                },
                Thread.currentThread().getContextClassLoader())) {

            for (String binaryName : matchedClasses) {
                System.out.println();
                try {
                    Class<?> clazz = classLoader.loadClass(binaryName);
                    boolean hasSolutionBase = false;
                    for (var itf : clazz.getInterfaces()) {
                        hasSolutionBase = List.of(itf.getInterfaces()).stream().reduce(false, (init, v) -> init || v.equals(SolutionBase.class), (init, res) -> init || res);
                    }
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
        } catch (Exception e) {
            System.out.println(e.toString());
        }
    }

    private static void findClassFiles(File root, File current, List<String> classList) {
        for (File file : current.listFiles()) {
            if (file.isDirectory()) {
                findClassFiles(root, file, classList);
            } else if (file.getName().endsWith(".class")) {
                String path = file.getPath()
                        .replace(root.getPath() + File.separator, "")
                        .replace(File.separator, ".")
                        .replaceAll("\\.class$", "");
                // var matcher = Pattern.compile("Day[0-9]+\\.(?<ClassName>[^\\.]+)").matcher(path);
                if (path.matches("Day[0-9]+\\.(?<ClassName>[^\\.]+)")) {
                    classList.add(path);
                }
            }
        }
    }
}
