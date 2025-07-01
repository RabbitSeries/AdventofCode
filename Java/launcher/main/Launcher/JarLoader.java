package Launcher;

import java.io.File;
import java.io.IOException;
import java.security.CodeSource;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.List;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

import JavaDataModel.AoCSolution;
import JavaDataModel.SolutionBase;

public class JarLoader {
    public static List<Class<?>> loadCodeSource() throws IOException, ClassNotFoundException {
        // Locate current jar file
        CodeSource codeSource = JarLoader.class.getProtectionDomain().getCodeSource();
        if (codeSource == null || !codeSource.getLocation().getPath().endsWith(".jar")) {
            System.err.println("Can't locate jar file.");
            return null;
        }
        List<Class<?>> sources = new ArrayList<>();
        File jarFile = new File(codeSource.getLocation().getPath());
        try (JarFile jar = new JarFile(jarFile)) {
            Enumeration<JarEntry> entries = jar.entries();
            while (entries.hasMoreElements()) {
                JarEntry entry = entries.nextElement();
                String name = entry.getName();
                if (name.endsWith(".class")) {
                    String className = name
                            .replace('/', '.')
                            .replace(".class", "");
                    Class<?> source = Class.forName(className);
                    if (source.isAnnotationPresent(AoCSolution.class) && new HashSet<>(List.of(source.getInterfaces())).contains(SolutionBase.class)) {
                        sources.add(source);
                    }
                }
            }
        }
        sources.sort(Comparator.comparing(Class::getName));
        return sources;
    }

}
