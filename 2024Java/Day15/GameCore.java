package Day15;
import java.io.*;
import java.util.*;
import java.util.Map.Entry;

public class GameCore {
    private static final int CELLWALL = -1; // Replace with actual value
    private static final int CELLEMPTY = -2; // Replace with actual value
    private static final int BOT = -3; // Replace with actual value

    public static void main(String[] args) throws IOException {
        System.out.println("Hello World!");
        GameCore gameCore = new GameCore();
        Vector<Vector<Integer>> arcade = new Vector<>();
        Vector<boxPos> id2BoxPos = new Vector<>();
        pos start = new pos(0, 0);
        gameCore.readMap(arcade, id2BoxPos, start);
        gameCore.playMyself(id2BoxPos, arcade, start);
    }

    void playMyself(Vector<boxPos> id2BoxPos, Vector<Vector<Integer>> arcade, pos curPos) throws IOException {
        printGUI(arcade, curPos);
        while (true) {
            if (freshGUI(id2BoxPos, arcade, curPos))
                printGUI(arcade, curPos);
            else
                return;
        }
    }

    public void readMap(Vector<Vector<Integer>> arcade, Vector<boxPos> id2BoxPos, pos start) {
        try (BufferedReader reader = new BufferedReader(new FileReader("./input.txt"))) {
            String line;
            while ((line = reader.readLine()) != null) {
                List<Integer> row = new ArrayList<>();
                for (char c : line.toCharArray()) {
                    switch (c) {
                        case '#':
                            row.add(CELLWALL);
                            row.add(CELLWALL);
                            break;
                        case '.':
                            row.add(CELLEMPTY);
                            row.add(CELLEMPTY);
                            break;
                        case 'O':
                            int boxIndex = id2BoxPos.size();
                            row.add(boxIndex);
                            row.add(boxIndex);
                            pos pos1 = new pos(arcade.size(), row.size() - 2);
                            pos pos2 = new pos(arcade.size(), row.size() - 1);
                            id2BoxPos.add(new boxPos(pos1, pos2));
                            break;
                        case '@':
                            start.x = arcade.size();
                            start.y = row.size();
                            row.add(CELLEMPTY);
                            row.add(CELLEMPTY);
                            break;
                        default:
                            // Ignore other characters
                            break;
                    }
                }
                arcade.add(new Vector<>(row));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void printGUI(Vector<Vector<Integer>> arcade, pos curPos) {
        // Clear the console (works on UNIX-based systems)
        System.out.print("\033[H\033[2J");
        System.out.flush();

        // Create a copy of the arcade for GUI rendering
        Vector<Vector<Integer>> GUI = new Vector<>();
        for (Vector<Integer> row : arcade) {
            GUI.add(new Vector<>(row));
        }

        // Set the bot's position in the GUI
        GUI.get(curPos.x).set(curPos.y, BOT);

        // Render the GUI
        for (Vector<Integer> line : GUI) {
            boolean first = true;
            for (int id : line) {
                if (id >= 0) {
                    if (first) {
                        first = false;
                        System.out.print("[");
                    } else {
                        first = true;
                        System.out.print("]");
                    }
                } else if (id == CELLWALL) {
                    System.out.print("#");
                } else if (id == CELLEMPTY) {
                    System.out.print(" ");
                } else {
                    System.out.print("@");
                }
            }
            System.out.println();
        }
    }

    boolean getNextBoxVertical(Vector<boxPos> id2BoxPos, Vector<Vector<Integer>> arcade, Vector<boxPos> curLevel,
            Vector<boxPos> nextLevel, char control, boolean isBlocked) {
        Map<Integer, Integer> nextLevelID = new HashMap<>();
        // Vector<boxPos> nextLevel = new Vector<>();
        for (int i = 0; i < curLevel.size() && !isBlocked; i++) {
            boxPos curBox = curLevel.elementAt(i);
            {
                pos l = getNextPos(control, curBox.l), r = getNextPos(control, curBox.r);
                int nextIDL = arcade.elementAt(l.x).get(l.y), nextIDR = arcade.elementAt(r.x).get(r.y);
                if (nextIDL == CELLWALL || nextIDR == CELLWALL) {
                    isBlocked = true;
                    nextLevel.clear();
                    // Unable to move.
                    return isBlocked;
                }
                // Avoid dulplicated eppend.
                if (nextIDL != CELLEMPTY)
                    nextLevelID.put(nextIDL, nextLevelID.getOrDefault(nextIDL, 0) + 1);
                if (nextIDR != CELLEMPTY)
                    nextLevelID.put(nextIDR, nextLevelID.getOrDefault(nextIDR, 0) + 1);
            }
        }
        for (Entry<Integer, Integer> m : nextLevelID.entrySet()) {
            nextLevel.add(id2BoxPos.get(m.getKey()));
        }
        return isBlocked;
    }

    boolean getNextBoxHorizental(Vector<boxPos> id2BoxPos, Vector<Vector<Integer>> arcade,
            Vector<boxPos> curLevel, Vector<boxPos> nextLevel, char control, boolean isBlocked) {
        assert (curLevel.size() == 1);
        boxPos curBox = curLevel.elementAt(0);
        pos nextBoxPos;
        if (control == '<') {
            nextBoxPos = getNextPos(control, curBox.l);
        } else {
            nextBoxPos = getNextPos(control, curBox.r);
        }
        if (isBox(arcade, nextBoxPos)) {
            nextLevel.add(getBoxPosAtPos(id2BoxPos, arcade, nextBoxPos));

        } else if (isWALL(arcade, nextBoxPos)) {
            isBlocked = true;

        } else {
            assert (nextLevel.size() == 0);
        }
        return isBlocked;

    }

    void pushBoxVertical(Vector<boxPos> boxList, Vector<boxPos> id2BoxPos, Vector<Vector<Integer>> arcade,
            char control) {
        for (boxPos b : boxList) {
            pos l = b.l, r = b.r;
            int boxId = arcade.elementAt(l.x).get(l.y);
            boxPos curBox = id2BoxPos.get(boxId);
            curBox.l = getNextPos(control, l);
            curBox.r = getNextPos(control, r);
            arcade.get(l.x).set(l.y, CELLEMPTY);
            arcade.get(r.x).set(r.y, CELLEMPTY);
            arcade.get(curBox.l.x).set(curBox.l.y, boxId);
            arcade.get(curBox.r.x).set(curBox.r.y, boxId);
        }
    }

    void pushBoxHorizontal(Vector<boxPos> boxList, Vector<boxPos> id2BoxPos, Vector<Vector<Integer>> arcade,
            char control) {
        assert (boxList.size() == 1);
        for (boxPos b : boxList) {
            pos l = new pos(b.l.x, b.l.y), r = new pos(b.r.x, b.r.y);
            int boxId = arcade.elementAt(l.x).get(l.y);
            boxPos curBox = id2BoxPos.get(boxId);
            curBox.l = getNextPos(control, l);
            curBox.r = getNextPos(control, r);
            if (control == '>')
                arcade.get(l.x).set(l.y, CELLEMPTY);
            else
                arcade.get(r.x).set(r.y, CELLEMPTY);
            arcade.get(curBox.l.x).set(curBox.l.y, boxId);
            arcade.get(curBox.r.x).set(curBox.r.y, boxId);
        }
    }

    void pushBox(Vector<boxPos> id2BoxPos, Vector<Vector<Integer>> arcade, pos curPos, char control) {
        boolean isBlocked = false;
        Stack<Vector<boxPos>> boxPath = new Stack<>();
        Vector<boxPos> curLevel = new Vector<>();
        // NextPos of start
        pos nextPos = getNextPos(control, curPos);
        curLevel.add(getBoxPosAtPos(id2BoxPos, arcade, nextPos));
        boxPath.push(curLevel);
        while (!isBlocked && !curLevel.isEmpty()) {
            Vector<boxPos> nextLevel = new Vector<>();
            if (control == 'v' || control == '^') {
                isBlocked = getNextBoxVertical(id2BoxPos, arcade, curLevel, nextLevel, control, isBlocked);
            } else {
                isBlocked = getNextBoxHorizental(id2BoxPos, arcade, curLevel, nextLevel, control, isBlocked);
            }
            if (!isBlocked) {
                if (!nextLevel.isEmpty())
                    boxPath.push(nextLevel);
                curLevel = nextLevel;
            } else {
                nextLevel.clear();
                curLevel.clear();
            }
        }
        if (!isBlocked) {
            while (!boxPath.empty()) {
                Vector<boxPos> preBoxList = boxPath.pop();
                if (control == '^' || control == 'v') {
                    pushBoxVertical(preBoxList, id2BoxPos, arcade, control);
                } else {
                    pushBoxHorizontal(preBoxList, id2BoxPos, arcade, control);
                }
            }
            curPos.x = nextPos.x;
            curPos.y = nextPos.y;
        }
    }

    boolean freshGUI(Vector<boxPos> id2BoxPos, Vector<Vector<Integer>> arcade, pos curPos) throws IOException {
        char c = getMoveMent();
        if (c == 'q') {
            return false;
        }
        pos nextPos = getNextPos(c, curPos);
        switch (arcade.elementAt(nextPos.x).elementAt(nextPos.y)) {
            case CELLWALL:
                break;
            case CELLEMPTY:
                curPos.x = nextPos.x;
                curPos.y = nextPos.y;
                break;
            default:
                // Push boxes
                pushBox(id2BoxPos, arcade, curPos, c);
                break;
        }
        return true;
    }

    char getMoveMent() throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String move = "A";
        move = String.valueOf(br.readLine());
        // TODO obtain movement from front end;
        move = move.toUpperCase();
        if (move.equals("A") || move.equals("ARROWLEFT")) {
            return '<';
        } else if (move.equals("D") || move.equals("ARROWRIGHT")) {
            return '>';
        } else if (move.equals("S") || move.equals("ARROWDOWN")) {
            return 'v';
        } else if (move.equals("W") || move.equals("ARROWUP")) {
            return '^';
        }
        System.out.println("ERROR: Invalid move: " + move);
        return '?';
    }

    pos getNextPos(char c, pos curPos) {
        pos nextPos = null;
        switch (c) {
            case '^':
                nextPos = new pos(curPos.x - 1, curPos.y);
                break;
            case '>':
                nextPos = new pos(curPos.x, curPos.y + 1);
                break;
            case 'v':
                nextPos = new pos(curPos.x + 1, curPos.y);
                break;
            case '<':
                nextPos = new pos(curPos.x, curPos.y - 1);
                break;
            default:
                System.out.println("Error situation.");
                break;
        }
        if (nextPos != null)
            return nextPos;
        else {
            return new pos(0, 0);
        }
    }

    boxPos getBoxPosAtPos(Vector<boxPos> id2BoxPos, Vector<Vector<Integer>> pos2Id, pos curPos) {
        return new boxPos(id2BoxPos.elementAt(pos2Id.elementAt(curPos.x).elementAt(curPos.y)));
    }

    boolean isBox(Vector<Vector<Integer>> pos2Id, pos point) {
        return pos2Id.elementAt(point.x).elementAt(point.y) != CELLWALL
                && pos2Id.elementAt(point.x).elementAt(point.y) != CELLEMPTY;
    }

    boolean isWALL(Vector<Vector<Integer>> pos2Id, pos point) {
        return pos2Id.elementAt(point.x).elementAt(point.y) == CELLWALL;
    }

    boolean isEmpty(Vector<Vector<Integer>> pos2Id, pos point) {
        return pos2Id.elementAt(point.x).elementAt(point.y) == CELLEMPTY;
    }

}

class pos {
    int x, y;

    public pos(int x, int y) {
        this.x = x;
        this.y = y;
    }
}

class boxPos {
    pos l, r;

    public boxPos(pos pos1, pos pos2) {
        this.l = new pos(pos1.x, pos1.y);
        this.r = new pos(pos2.x, pos2.y);
    }

    public boxPos(boxPos elementAt) {
        this.l = new pos(elementAt.l.x, elementAt.l.y);
        this.r = new pos(elementAt.r.x, elementAt.r.y);
    }
}
