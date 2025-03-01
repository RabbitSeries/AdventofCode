
package Day04;

import java.io.*;
import java.util.*;

class CeresSearch {

    static int[][] dx = new int[][] {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 2, 3}, {0, 1, 2, 3},
            {0, 1, 2, 3}, {0, -1, -2, -3}, {0, -1, -2, -3}, {0, -1, -2, -3},};
    static int[][] dy = new int[][] {{0, 1, 2, 3}, {0, -1, -2, -3}, {0, 1, 2, 3}, {0, 0, 0, 0},
            {0, -1, -2, -3}, {0, 1, 2, 3}, {0, 0, 0, 0}, {0, -1, -2, -3}

    };
    static int[][] xdx = new int[][] {{-1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {1, 0, -1}};
    static int[][] xdy = new int[][] {{-1, 0, 1}, {1, 0, -1}, {-1, 0, 1}, {1, 0, -1},};

    boolean isXMAS(int i, int j, int k, ArrayList<ArrayList<Character>> m) {
        int rows = m.size(), rols = m.get(0).size();
        return i + dx[k][3] >= 0 && i + dx[k][3] < rows && j + dy[k][3] >= 0 && j + dy[k][3] < rols
                && m.get(i + dx[k][0]).get(j + dy[k][0]) == 'X'
                && m.get(i + dx[k][1]).get(j + dy[k][1]) == 'M'
                && m.get(i + dx[k][2]).get(j + dy[k][2]) == 'A'
                && m.get(i + dx[k][3]).get(j + dy[k][3]) == 'S';
    }

    boolean isCrossMAS(int i, int j, int k, ArrayList<ArrayList<Character>> m) {
        int rows = m.size(), rols = m.get(0).size();
        return i + xdx[k][2] >= 0 && i + xdx[k][2] < rows && j + xdy[k][2] >= 0
                && j + xdy[k][2] < rols && i + xdx[k][0] >= 0 && i + xdx[k][0] < rows
                && j + xdy[k][0] >= 0 && j + xdy[k][0] < rols
                && m.get(i + xdx[k][0]).get(j + xdy[k][0]) == 'M'
                && m.get(i + xdx[k][1]).get(j + xdy[k][1]) == 'A'
                && m.get(i + xdx[k][2]).get(j + xdy[k][2]) == 'S';
    }

    ArrayList<ArrayList<Character>> readFile() throws IOException {
        ArrayList<ArrayList<Character>> m = new ArrayList<>();
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        String linebuffer;
        while ((linebuffer = input.readLine()) != null) {
            ArrayList<Character> row = new ArrayList<>();
            String line = linebuffer;
            for (int i = 0; i < line.length(); i++) {
                if (line.charAt(i) == '\n')
                    continue;
                row.add(line.charAt(i));
            }
            m.add(row);
        }
        input.close();
        return m;
    }

    CeresSearch Solution1() throws IOException {
        ArrayList<ArrayList<Character>> m = readFile();
        int res = 0;
        for (int i = 0; i < m.size(); i++) {
            for (int j = 0; j < m.get(i).size(); j++) {
                for (int k = 0; k < 8; k++) {
                    if (isXMAS(i, j, k, m)) {
                        res++;
                    }
                }
            }
        }
        System.out.println("Solution 1: " + res);
        return this;
    }

    void Solution2() throws IOException {
        ArrayList<ArrayList<Character>> m = readFile();
        Integer res = 0;
        for (int i = 0; i < m.size(); i++) {
            for (int j = 0; j < m.get(i).size(); j++) {
                int crossTimes = 0;
                for (int k = 0; k < 4; k++) {
                    if (isCrossMAS(i, j, k, m)) {
                        crossTimes++;
                    }
                }
                if (crossTimes >= 2) {
                    res++;
                }
            }
        }
        System.out.println("Solution 2: " + res);
    }

    public static void main(String[] args) throws IOException {
        CeresSearch solution = new CeresSearch();
        solution.Solution1().Solution2();
    }
}
