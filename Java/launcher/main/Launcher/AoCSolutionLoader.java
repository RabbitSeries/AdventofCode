package Launcher;

import java.io.IOException;
import java.net.URL;
import java.net.URLClassLoader;
import java.nio.file.Path;
import java.util.*;
import java.util.Map.Entry;
import java.util.AbstractMap.SimpleEntry;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

import JavaDataModel.AoCSolution;
import JavaDataModel.ISolution;

public class AoCSolutionLoader {
    private URL jarSource = null;

    private JarFile jarFile = null;

    public AoCSolutionLoader(URL JarSource) throws IOException {
        jarSource = JarSource;
        if (jarSource != null) {
            jarFile = new JarFile(jarSource.getPath());
        }
    }

    public JarFile getJarFile() {
        return jarFile;
    }

    private class PeekableIterator<T> implements Iterator<T> {
        private T current = null;

        private Iterator<T> inner_itr;

        public PeekableIterator(Iterator<T> itr) {
            inner_itr = itr;
            next();
        }

        @Override
        public boolean hasNext() {
            return current != null;
        }

        @Override
        public T next() {
            current = inner_itr.hasNext() ? inner_itr.next() : null;
            return current;
        }

        public T peek() {
            return current;
        }
    }

    public List<Entry<Class<?>, JarEntry>> matchResources(List<Entry<JarEntry, Class<?>>> classSources, List<JarEntry> resourceEntries) {
        classSources.sort(Comparator.comparing(entry -> entry.getValue().getName()));
        resourceEntries.sort(Comparator.comparing(entry -> entry.getName()));
        PeekableIterator<Entry<JarEntry, Class<?>>> classItr = new PeekableIterator<>(classSources.iterator());
        PeekableIterator<JarEntry> resourceItr = new PeekableIterator<>(resourceEntries.iterator());
        List<Entry<Class<?>, JarEntry>> results = new ArrayList<>();
        while (classItr.hasNext() && resourceItr.hasNext()) {
            String classModuleName = Path.of(classItr.peek().getKey().getName()).getParent().toString();
            String resourceModuleName = Path.of(resourceItr.peek().getName()).getParent().toString();
            if (classModuleName.endsWith(resourceModuleName)) {
                results.add(new SimpleEntry<>(classItr.peek().getValue(), resourceItr.peek()));
            } else {
                System.err.println("Class " + classModuleName + " is not matching resource " + resourceModuleName);
            }
            classItr.next();
            resourceItr.next();
        }
        return results;
    }

    public List<Entry<Class<?>, JarEntry>> loadSolutions() throws IOException, ClassNotFoundException {
        // Locate current jar file
        List<Entry<JarEntry, Class<?>>> classSources = new ArrayList<>();
        List<JarEntry> resourceEntries = new ArrayList<>();
        URLClassLoader classLoader = new URLClassLoader(new URL[] {
                jarSource
        });
        for (JarEntry entry : jarFile.stream().toList()) {
            String name = entry.getName();
            if (name.endsWith(".class")) {
                String className = name
                        .replace("/", ".") // regardless platform, paths inside the plarform are always seperated by /
                        .replace(".class", "");
                Class<?> source = classLoader.loadClass(className);
                if (source.isAnnotationPresent(AoCSolution.class) && List.of(source.getInterfaces()).contains(ISolution.class)) {
                    classSources.add(new AbstractMap.SimpleEntry<>(entry, source));
                }
            } else if (!entry.isDirectory() && name.endsWith("input.txt")) {
                resourceEntries.add(entry);
            }
        }
        List<Entry<Class<?>, JarEntry>> matchedSolutions = matchResources(classSources, resourceEntries);
        matchedSolutions.sort(Comparator.comparing(entry -> entry.getKey().getName()));
        classLoader.close();
        return matchedSolutions;
    }
}
