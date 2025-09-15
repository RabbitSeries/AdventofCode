package Launcher;

import static org.junit.jupiter.api.Assumptions.assumeTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.when;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;
import java.nio.file.Path;
import java.util.List;
import java.util.Map;
import java.util.jar.JarEntry;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.Mock;
import org.mockito.MockedStatic;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;

import JavaDataModel.ISolution;

public class TestAoCSolutionLoader {
    @Mock
    AoCSolutionLoader solutionLoader;

    @Mock
    ISolution solution;

    @BeforeEach
    public void TestArgs() throws Exception {
        MockitoAnnotations.openMocks(this);
        when(solutionLoader.loadSolutions()).thenAnswer(invocation -> List.of(Map.entry(solution, new JarEntry("FOO"))));
        doAnswer(invocation -> {
            System.out.println("Solution 1: test result1");
            return null;
        }).when(solution).Solution1(any(BufferedReader.class));
        doAnswer(invocation -> {
            System.out.println("Solution 2: test result2");
            return null;
        }).when(solution).Solution2(any(BufferedReader.class));
    }

    @Test
    public void testRun() throws Exception {
        try (MockedStatic<Main> mockedStatic = Mockito.mockStatic(Main.class)) {
            mockedStatic.when(() -> Main.RunSolution(any(URL.class))).then(invocation -> {
                System.out.println("Test: Loading solutions");
                assumeTrue(solutionLoader.loadSolutions().size() == 1);
                return null;
            });
            Main.RunSolution(Path.of("FOO").toUri().toURL());
            solution.Solution1(new BufferedReader(new InputStreamReader(System.in)));
            solution.Solution2(new BufferedReader(new InputStreamReader(System.in)));
            System.out.println("ALL PASS");
        }
    }
}
