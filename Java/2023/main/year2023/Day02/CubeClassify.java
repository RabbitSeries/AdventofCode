package year2023.Day02;

import java.io.*;
import java.util.*;

import JavaDataModel.*;

@AoCSolution()
public class CubeClassify implements SolutionBase {

    Integer res = 0;

    Integer power = 0;

    HashMap<String, Integer> cubeCnt;

    HashMap<String, Integer> cubeTarget;

    void InitDataModel() {
        cubeCnt = new HashMap<>(Map.of("red", 0, "green", 0, "blue", 0));
        cubeTarget = new HashMap<>(Map.of("red", 12, "green", 13, "blue", 14));
    }

    void readFile(BufferedReader input, boolean stopOnFail) throws IOException {
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

    public void Solution1(BufferedReader input) throws IOException {
        res = 0;
        power = 0;
        readFile(input, true);
        System.out.println("Solution 1: " + res);
    }

    public void Solution2(BufferedReader input) throws IOException {
        res = 0;
        power = 0;
        readFile(input, false);
        System.out.println("Solution 2: " + power);
    }
}
