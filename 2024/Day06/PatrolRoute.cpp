
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
int distinctPosCnt = 0;
void Solution1() {
    pair<int, int> guardPos( 0, 0 );
    vector<vector<char>> routeMap = readFile( guardPos );
    patrol( guardPos, routeMap );

    for( vector<char> row : routeMap ) {
        for( char c : row ) {
            // cout << c;
            if( c == 'X' ) {
                distinctPosCnt++;
            }
        }
        // cout << endl;
    }
}
bool operator==( pos& p1, pos& p2 ) {
    return p1.first == p2.first && p2.second == p2.second;
}
template<>
struct std::hash<pos> {
    size_t operator()( const pos& p )const {
        return std::hash<int>{}( p.first ) ^ ( std::hash<int>{}( p.second ) << 1 );
    }
};
// Thus reduced the copy time consumption.
bool isPatrolCircle( pos curPos, vector<vector<char>> const& routeMap, int rows, int cols ) {
    bool isCircle = false;
    // vector<vector<pair<bool, char>>> passed( rows, vector<pair<bool, char>>( cols, pair<bool, char>( false, '.' ) ) );
    unordered_map<pos, unordered_map<char, bool>>* passed = new unordered_map<pos, unordered_map<char, bool>>();
    char curFace = routeMap[curPos.first][curPos.second];;
    bool curStatus;
    // pair<bool, char>  curStatus;
    while( isValid( curPos, rows, cols ) ) {
        curStatus = ( passed->count( curPos ) != 0 && ( *passed )[curPos].count( curFace ) != 0 ) ? true : false;
        // curStatus = passed[curPos.first][curPos.second];
        if( curStatus ) {
            // if( curStatus.first && curStatus.second == curFace ) {
            return true;
        }
        // Determine the face
        pos nextPos{ NEXTPOS.at( curFace ).first + curPos.first,NEXTPOS.at( curFace ).second + curPos.second };
        if( isValid( nextPos, rows, cols ) ) {
            ( *passed )[curPos][curFace] = true;
            if( routeMap[nextPos.first][nextPos.second] == '#' ) {
                // There is no need to update before turnning directions, cause this location's direction must not be updated if this location is a loop's end and it will be updated if this location is not loop's end.
                turnFace( curFace );
            }
            else {
                // routeMap[i--][j] = '|';
                // passed[curPos.first][curPos.second].first = true;
                // passed[curPos.first][curPos.second].second = curFace;
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

atomic<int> progress( 0 );
mutex progressMtx;

void progressIncrement() {
    lock_guard<mutex> resultLock( mtx );
    progress++;
}

void threadTask( int startRow, int endRow, pair<int, int> guardPos, vector<vector<char>> routeMap ) {
    for( int i = startRow; i < endRow; i++ ) {
        for( int j = 0; j < routeMap[i].size(); j++ ) {
            progressIncrement();
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

    int lastChecked = 0, total = routeMap.size() * routeMap[0].size();
    while( lastChecked < total ) {
        lastChecked = progress.load();
        std::cout << "\033[2J\033[1;1H";
        cout << "Progress - Part 2" << endl;
        cout << "Positions checked: " << lastChecked << "/" << total << " (" << ( lastChecked * 100.0 / total ) << "%)" << endl;
        cout << "Loop positions found: " << distinctPlacement.load() << endl;
        this_thread::sleep_for( chrono::milliseconds( 100 ) );
    }

    for( auto& t : threads ) {
        t.join();
    }

    cout << "Solution 1: " << distinctPosCnt << endl;
    cout << "Solution 2: " << distinctPlacement.load() << endl;
}

int main() {
    auto now = chrono::high_resolution_clock::now();
    Solution1();
    Solution2();
    auto end = chrono::high_resolution_clock::now();
    // 258 ms, 0.2 s
    cout << setbase( ios_base::fixed ) << setprecision( 10 ) << setfill( '0' ) << chrono::duration_cast<chrono::microseconds>( end - now ).count() / 1000000.0 << " seconds" << endl;
    return 0;
}
