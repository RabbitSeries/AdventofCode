package Launcher;

import java.io.IOException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.*;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

import JavaDataModel.AoCSolution;
import JavaDataModel.SolutionBase;

public class AoCSolutionLoader {
    public static List<Class<?>> loadCodeSource(URL jarSource) throws IOException, ClassNotFoundException {
        // Locate current jar file
        List<Class<?>> sources = new ArrayList<>();
        URLClassLoader classLoader = null;
        try (JarFile jarFile = new JarFile(jarSource.getPath())) {
            classLoader = new URLClassLoader(new URL[] {
                    jarSource
            });
            for (JarEntry entry : jarFile.stream().toList()) {
                String name = entry.getName();
                if (name.endsWith(".class")) {
                    String className = name
                            .replace("/", ".") // regardless platform, paths inside the plarform are always seperated by /
                            .replace(".class", "");
                    Class<?> source = classLoader.loadClass(className);
                    if (source.isAnnotationPresent(AoCSolution.class) && new HashSet<>(List.of(source.getInterfaces())).contains(SolutionBase.class)) {
                        sources.add(source);
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (classLoader != null) {
                classLoader.close();
            }
        }
        sources.sort(Comparator.comparing(Class::getName));
        return sources;
    }

}
