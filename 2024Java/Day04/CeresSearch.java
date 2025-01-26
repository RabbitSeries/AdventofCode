
package Day04;

import java.io.*;
import java.util.*;

class CeresSearch {

    int[][] dx;
    int[][] dy;
    int[][] xdx;
    int[][] xdy;

    void InitDataModel() {

        dx = new int[8][4];
        dy = new int[8][4];
        xdx = new int[4][3];
        xdy = new int[4][3];

        dx[2] = new int[] { 0, 1, 2, 3 };
        dx[3] = new int[] { 0, 1, 2, 3 };
        dx[4] = new int[] { 0, 1, 2, 3 };
        dx[5] = new int[] { 0, -1, -2, -3 };
        dx[6] = new int[] { 0, -1, -2, -3 };
        dx[7] = new int[] { 0, -1, -2, -3 };

        dy[0] = new int[] { 0, 1, 2, 3 };
        dy[1] = new int[] { 0, -1, -2, -3 };
        dy[2] = new int[] { 0, 1, 2, 3 };
        dy[3] = new int[] { 0, 0, 0, 0 };
        dy[4] = new int[] { 0, -1, -2, -3 };
        dy[5] = new int[] { 0, 1, 2, 3 };
        dy[6] = new int[] { 0, 0, 0, 0 };
        dy[7] = new int[] { 0, -1, -2, -3 };

        xdx[0] = new int[] { -1, 0, 1 };
        xdx[1] = new int[] { -1, 0, 1 };
        xdx[2] = new int[] { 1, 0, -1 };
        xdx[3] = new int[] { 1, 0, -1 };

        xdy[0] = new int[] { -1, 0, 1 };
        xdy[1] = new int[] { 1, 0, -1 };
        xdy[2] = new int[] { -1, 0, 1 };
        xdy[3] = new int[] { 1, 0, -1 };
    }

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
        return i + xdx[k][2] >= 0 && i + xdx[k][2] < rows && j + xdy[k][2] >= 0 && j + xdy[k][2] < rols &&
                i + xdx[k][0] >= 0 && i + xdx[k][0] < rows && j + xdy[k][0] >= 0 && j + xdy[k][0] < rols
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
        InitDataModel();
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
        InitDataModel();
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