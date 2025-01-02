#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <tuple>

using namespace std;

enum { CELLWALL = -1, CELLEMPTY = 0 };
enum { NORTH, EAST, SOUTH, WEST };

struct State {
    int x, y, direction, score;
    bool operator>(const State& other) const {
        return score > other.score;
    }
};

const int dx[] = {-1, 0, 1, 0};
const int dy[] = {0, 1, 0, -1};

int solveMaze(const vector<vector<int>>& maze, pair<int, int> start, pair<int, int> end) {
    int rows = maze.size();
    int cols = maze[0].size();

    auto isValid = [&](int x, int y) {
        return x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] == CELLEMPTY;
    };

    priority_queue<State, vector<State>, greater<State>> pq;
    unordered_set<string> visited;

    // Initial state: starting at S, facing EAST, score 0
    pq.push({start.first, start.second, EAST, 0});

    while (!pq.empty()) {
        cout << pq.size() << endl;
        State curr = pq.top();
        pq.pop();

        int x = curr.x, y = curr.y, dir = curr.direction, score = curr.score;

        if (x == end.first && y == end.second) {
            return score;
        }

        string stateKey = to_string(x) + "," + to_string(y) + "," + to_string(dir);
        if (visited.count(stateKey)) continue;
        visited.insert(stateKey);

        // Move forward
        int nx = x + dx[dir], ny = y + dy[dir];
        if (isValid(nx, ny)) {
            pq.push({nx, ny, dir, score + 1});
        }

        // Rotate clockwise
        pq.push({x, y, (dir + 1) % 4, score + 1000});

        // Rotate counterclockwise
        pq.push({x, y, (dir + 3) % 4, score + 1000});
    }

    return -1; // No solution found
}

int main() {
    vector<vector<int>> m;
    pair<int, int> s, e;

    // Replace with the input file reading logic you provided.
    // Populate the `maze`, `start`, and `end` variables.
    FILE* input = fopen( "input.txt", "r" );
    // Always init it, or you will have wrong result.
    char buf[1024] = "\0";
    while( !feof( input ) && fgets( buf, 1024, input ) ) {
        vector<int> row;
        for( char c : buf ) {
            if( c != '\n' && c != '\0' ) {
                if( c == '#' ) {
                    row.push_back( CELLWALL );
                }
                else if( c == '.' ) {
                    row.push_back( CELLEMPTY );
                }
                else if( c == 'S' ) {
                    s = { m.size(), row.size() };
                    row.push_back( CELLEMPTY );
                }
                else if( c == 'E' ) {
                    e = { m.size(), row.size() };
                    row.push_back( CELLEMPTY );
                }
            }
        }
        m.push_back( row );
    }
    int result = solveMaze( m, s, e );
    cout << "Lowest score: " << result << endl;

    return 0;
}