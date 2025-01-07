#include "bits/stdc++.h"
#define BUFFER_SIZE 1024
using namespace std;
typedef pair<int, int> pos;

inline void turnFace( char& curFace ) {
    switch( curFace ) {
    case '^':
        curFace = '>';
        break;
    case '>':
        curFace = 'v';
        break;
    case 'v':
        curFace = '<';
        break;
    case '<':
        curFace = '^';
        break;
    default:
        cerr << "Error facing." << endl;
    }
    return;
}

const map<char, pos> NEXTPOS{
    {'^',{-1,0}},
    {'>',{0, 1}},
    {'v', {1,0}},
    {'<',{0,-1}}
};

inline bool isValid( pos curPos, int rows, int cols ) {
    int x = curPos.first, y = curPos.second;
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

vector<vector<char>> readFile( pair<int, int>& guardPos ) {
    vector<vector<char>> routeMap;
    char lineBuf[BUFSIZ] = { '\0' };
    FILE* input = fopen( "input.txt", "r" );
    while( fgets( lineBuf, BUFSIZ, input ) ) {
        vector<char> row;
        int i = 0;
        while( lineBuf[i] != '\n' && lineBuf[i] != '\0' ) {
            if( lineBuf[i] == '<' || lineBuf[i] == '>' || lineBuf[i] == '^' || lineBuf[i] == 'v' ) {
                guardPos.first = routeMap.size();
                guardPos.second = i;
            }
            row.push_back( lineBuf[i] );
            i++;
        }
        if( !row.empty() ) {
            routeMap.push_back( row );
        }
    }
    fclose( input );
    return move( routeMap );
}

void patrol( pos curPos, vector<vector<char>>& routeMap ) {
    int rows = routeMap.size(), cols = routeMap[0].size();
    char curFace;
    while( isValid( curPos, rows, cols ) ) {
        // Determine the face
        curFace = routeMap[curPos.first][curPos.second];
        pos nextPos{ NEXTPOS.at( curFace ).first + curPos.first,NEXTPOS.at( curFace ).second + curPos.second };
        if( isValid( nextPos, rows, cols ) ) {
            char& nextBlock = routeMap[nextPos.first][nextPos.second];
            if( nextBlock == '#' ) {
                turnFace( curFace );
                routeMap[curPos.first][curPos.second] = curFace;
            }
            else {
                routeMap[curPos.first][curPos.second] = 'X';
                curPos = nextPos;
                nextBlock = curFace;
            }
        }
        else {
            routeMap[curPos.first][curPos.second] = 'X';
            return;
        }
    }
}

void Solution1() {
    pair<int, int> guardPos( 0, 0 );
    vector<vector<char>> routeMap = readFile( guardPos );
    patrol( guardPos, routeMap );
    int distinctPosCnt = 0;
    for( vector<char> row : routeMap ) {
        for( char c : row ) {
            // cout << c;
            if( c == 'X' ) {
                distinctPosCnt++;
            }
        }
        // cout << endl;
    }
    cout << "Solution 1: " << distinctPosCnt << endl;

}

// Thus reduced the copy time consumption.
bool isPatrolCircle( pos curPos, vector<vector<char>> const& routeMap, int rows, int cols ) {
    bool isCircle = false;
    vector<vector<pair<bool, char>>> passed( rows, vector<pair<bool, char>>( cols, pair<bool, char>( false, '.' ) ) );
    int times = 0;
    char curFace = routeMap[curPos.first][curPos.second];;
    pair<bool, char> curStatus;
    while( isValid( curPos, rows, cols ) ) {
        times++;
        curStatus = passed[curPos.first][curPos.second];
        if( curStatus.first && curStatus.second == curFace ) {
            isCircle = true;
            return isCircle;
        }
        // Determine the face
        pos nextPos{ NEXTPOS.at( curFace ).first + curPos.first,NEXTPOS.at( curFace ).second + curPos.second };
        if( isValid( nextPos, rows, cols ) ) {
            if( routeMap[nextPos.first][nextPos.second] == '#' ) {
                turnFace( curFace );
            }
            else {
                passed[curPos.first][curPos.second].first = true;
                passed[curPos.first][curPos.second].second = curFace;
                // routeMap[i--][j] = '|';
                curPos = nextPos;
            }
        }
        else {
            return false;
        }
    }
    return isCircle;
}

// Learned from <https://github.com/TrueBurn/advent-of-code/blob/main/2024/day-6/solution.py>
atomic<int> distinctPlacement( 0 );
mutex mtx;

/**
 * @brief
 * @param startRow
 * @param endRow
 * @param guardPos
 * @param routeMap Directly copy the map, this will only happen for the times of number of CPU
 */
void threadTask( int startRow, int endRow, pair<int, int> guardPos, vector<vector<char>> routeMap ) {
    for( int i = startRow; i < endRow; i++ ) {
        for( int j = 0; j < routeMap[i].size(); j++ ) {
            if( i == guardPos.first && j == guardPos.second || routeMap[i][j] == '#' ) {
                continue;
            }
            char curCell = routeMap[i][j];
            routeMap[i][j] = '#';
            if( isPatrolCircle( guardPos, routeMap, routeMap.size(), routeMap[0].size() ) ) {
                lock_guard<mutex> resultLock( mtx );
                distinctPlacement++;
            }
            routeMap[i][j] = curCell;
        }
    }
}

void Solution2() {
    pair<int, int> guardPos( 0, 0 );
    vector<vector<char>> routeMap = readFile( guardPos );

    int numThreads = std::thread::hardware_concurrency();
    vector<thread> threads;

    int rowsPerThread = routeMap.size() / numThreads;

    for( int t = 0; t < numThreads; t++ ) {
        int startRow = t * rowsPerThread;
        int endRow = ( t == numThreads - 1 ) ? routeMap.size() : ( t + 1 ) * rowsPerThread;
        threads.push_back( thread( threadTask, startRow, endRow, guardPos, ref( routeMap ) ) );
    }

    for( auto& t : threads ) {
        t.join();
    }

    cout << "The distinct placement that makes the patrol in a circle counts: " << distinctPlacement.load() << endl;
}

int main() {
    auto now = chrono::high_resolution_clock::now();
    Solution1();
    Solution2();
    auto end = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>( end - now ).count() / 1000.0 << " seconds" << endl;
    return 0;
}