package JavaDataModel;

import java.io.File;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.List;

public class ReflectiveLauncher {
    public static void main(String[] args) throws Exception {
        File baseDir = new File("target/classes");
        List<String> matchedClasses = new ArrayList<>();

        findClassFiles(baseDir, baseDir, matchedClasses);

        try (URLClassLoader classLoader = new URLClassLoader(
                new URL[] {
                        baseDir.toURI().toURL()
                },
                Thread.currentThread().getContextClassLoader())) {

            for (String className : matchedClasses) {
                System.out.println("Running class: " + className);

                try {
                    Class<?> clazz = classLoader.loadClass(className);
                    Object instance = clazz.getConstructor().newInstance();

                    for (String methodName : new String[] {
                            "solution1", "solution2"
                    }) {
                        try {
                            Method method = clazz.getMethod(methodName);
                            System.out.println("==> " + methodName + ":");
                            method.invoke(instance);
                        } catch (NoSuchMethodException e) {
                            System.out.println("No method " + methodName + " in " + className);
                        }
                    }
                } catch (Exception e) {
                    System.out.println("Error running " + className);
                    e.printStackTrace();
                }
            }
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

                if (path.matches("[0-9]+Java\\.Day[0-9]+\\.[^\\.]+")) {
                    classList.add(path);
                }
            }
        }
    }
}
