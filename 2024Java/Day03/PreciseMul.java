package Day03;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class PreciseMul {
    // String is byVal foundamental type.

    Integer getNum(StringBuilder str, int nextPos) {
        int i = nextPos, num = 0;
        while (i < str.length() && Character.isDigit(str.charAt(i))) {
            num = num * 10 + str.charAt(i) - '0';
            i++;
        }
        return num;
    }

    Integer getNum(StringBuilder str) {
        return getNum(str, 0);
    }

    Integer getLineResult(StringBuilder str) {
        Integer res = 0;
        int nextPos = 0;
        while (nextPos != -1 && nextPos < str.length()) {
            nextPos = str.indexOf("mul(", nextPos);
            if (nextPos != -1) {
                nextPos = nextPos + 4;
            } else {
                break;
            }

            Integer a = 0, b = 0;

            if (nextPos < str.length() && Character.isDigit(str.charAt(nextPos))) {
                a = getNum(str, nextPos);
                nextPos = nextPos + a.toString().length();
            } else {
                nextPos++;
                continue;
            }

            if (nextPos + 1 < str.length() && str.charAt(nextPos) == ','
                    && Character.isDigit(str.charAt(nextPos + 1))) {
                // str.replace(0, 1, "");
                nextPos++;
                b = getNum(str, nextPos);
                nextPos = nextPos + b.toString().length();

            } else {
                nextPos++;
                continue;
            }

            if (nextPos < str.length() && str.charAt(nextPos) == ')') {
                res += a * b;
                if (nextPos + 1 < str.length()) {
                    str.replace(0, str.length(), str.substring(nextPos + 1));
                    nextPos = 0;
                } else {
                    str.replace(0, str.length(), "");
                    nextPos = -1;
                }
            } else {
                nextPos++;
                continue;
            }
        }
        return res;
    }

    Integer getLineResult(StringBuilder str, booleanRef enabled) {
        Integer zoneAdd = 0;
        if (enabled.isEnabled()) {
            Integer dontPos = str.indexOf("don't()");
            if (dontPos != -1) {
                String process = str.substring(0, dontPos);
                zoneAdd += getLineResult(new StringBuilder(process));
                enabled.setEnabled(false);
                if (dontPos + "don't()".length() < str.length()) {
                    str.replace(0, str.length(), str.substring(dontPos + "don't()".length()));
                    zoneAdd += getLineResult(str, enabled);
                    return zoneAdd;
                } else {
                    str.replace(0, str.length(), "");
                    return zoneAdd;
                }
            } else {
                zoneAdd += getLineResult(str);
                return zoneAdd;
            }
        } else {
            Integer doPos = str.indexOf("do()");
            if (doPos != -1) {
                enabled.setEnabled(true);
                if (doPos + "do()".length() < str.length()) {
                    str.replace(0, str.length(), str.substring(doPos + "do()".length()));
                    zoneAdd += getLineResult(str, enabled);
                    return zoneAdd;
                } else {
                    str.replace(0, str.length(), "");
                    return zoneAdd;
                }
            } else {
                return zoneAdd;
            }
        }
    }

    public PreciseMul Solution1() throws IOException {
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        String buf;
        Integer addUp = 0;
        while ((buf = input.readLine()) != null) {
            addUp += getLineResult(new StringBuilder(buf));
        }
        System.out.println("Solution 1: " + addUp);
        return this;
        // input.close();
    }

    public PreciseMul Solution2() throws IOException {
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        String buf;
        StringBuilder currentLine, leftOver = new StringBuilder("");
        Integer addUp = 0;
        booleanRef enabled = new booleanRef();
        while ((buf = input.readLine()) != null) {
            if (!buf.isEmpty()) {
                currentLine = new StringBuilder(buf + leftOver.substring(0));
                addUp += getLineResult(currentLine, enabled);
                leftOver = new StringBuilder(currentLine.substring(0));
            }
        }
        System.out.println("Solution 2: " + addUp);
        return this;
    }

    public static void main(String[] args) throws IOException {
        PreciseMul solution = new PreciseMul();
        solution.Solution1().Solution2();

    }

    static class booleanRef {
        private boolean enabled = true;

        public boolean isEnabled() {
            return this.enabled;
        }

        public void setEnabled(boolean enabled) {
            this.enabled = enabled;
        }

    }
}