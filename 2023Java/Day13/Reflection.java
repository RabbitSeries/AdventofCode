package Day13;

import java.io.*;
import java.util.*;

public class Reflection {
    List<List<Character>> IslandMap;

    void readFile() throws IOException {
        IslandMap = new ArrayList<>();
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        String buf;
        while ((buf = input.readLine()) != null) {
            IslandMap.add(buf.trim().chars().mapToObj(c -> (char) c).toList());
        }
    }
}
