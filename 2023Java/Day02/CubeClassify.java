package Day02;

import java.io.*;
import java.util.*;

public class CubeClassify {

    Integer res = 0;
    Integer power = 0;
    HashMap<String, Integer> cubeCnt;
    HashMap<String, Integer> cubeTarget;

    void InitDataModel() {
        cubeCnt = new HashMap<>();
        cubeTarget = new HashMap<>();

        cubeCnt.put("red", 0);
        cubeCnt.put("green", 0);
        cubeCnt.put("blue", 0);

        cubeTarget.put("red", 12);
        cubeTarget.put("green", 13);
        cubeTarget.put("blue", 14);
    }

    void readFile(boolean stopOnFail) throws IOException {
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        String buf;
        while ((buf = input.readLine()) != null && buf.length() != 0) {
            InitDataModel();
            boolean possible = true;
            String[] game = buf.split(":");
            Integer gameId = Integer.parseInt(game[0].split("\\s")[1]);
            String[] grabs = game[1].split(";");
            for (String grab : grabs) {
                for (String cubeInfo : grab.split(",")) {
                    cubeInfo = cubeInfo.trim();
                    Integer cnt = Integer.parseInt(cubeInfo.split("\\s")[0]);
                    String type = cubeInfo.split("\\s")[1];
                    if (cubeCnt.get(type) < cnt) {
                        if (cnt > cubeTarget.get(type)) {
                            possible = false;
                            if (stopOnFail)
                                break;
                            else {
                                cubeCnt.put(type, cnt);
                            }
                        } else {
                            cubeCnt.put(type, cnt);
                        }
                    }
                }
                if (!possible && stopOnFail) {
                    break;
                }
            }
            if (possible) {
                res += gameId;
            }
            if (!stopOnFail) {
                Integer gamePower = 1;
                for (String type : cubeCnt.keySet()) {
                    gamePower *= cubeCnt.get(type);
                }
                power += gamePower;
            }
        }
        input.close();
    }

    void Solution1() throws IOException {
        res = 0;
        power = 0;
        readFile(true);
        System.out.println("Solution 1: " + res);
    }

    void Solution2() throws IOException {
        res = 0;
        power = 0;
        readFile(false);
        System.out.println("Solution 2: " + power);

    }

    public static void main(String[] args) throws IOException {
        CubeClassify Solution = new CubeClassify();
        Solution.Solution1();
        Solution.Solution2();
    }
}
