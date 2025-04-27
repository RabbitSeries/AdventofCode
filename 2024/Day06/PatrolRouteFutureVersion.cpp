#include "bits/stdc++.h"
#define BUFFER_SIZE 1024
using namespace std;
struct Point2D : public pair<int, int> {
    Point2D() {};
    Point2D( int f, int s ) {
        first = f, second = s;
    }
    inline bool operator==( Point2D const& p ) const {
        return first == p.first && second == p.second;
    }
};

template<>
struct std::hash<Point2D> {
    inline size_t operator()( const Point2D& p ) const {
        return hash<int>{}( p.first ) ^ ( hash<int>{}( p.first ) << 10 );
    }
};

char face[]{ '^','>','v','<' };
unordered_map<char, int> faceId{
    {'^',0},
    {'>',1},
    {'v',2},
    {'<',3}
};
int dx[]{ -1,0,1,0 };
int dy[]{ 0,1,0,-1 };

inline bool isValid( Point2D curPos, int rows, int cols ) {
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
    return routeMap;
}

void patrol( Point2D curPos, vector<vector<char>>& routeMap ) {
    int rows = routeMap.size(), cols = routeMap[0].size();
    int curFace;
    while( isValid( curPos, rows, cols ) ) {
        // Determine the face
        curFace = faceId.at( routeMap[curPos.first][curPos.second] );
        Point2D nextPos{ dx[curFace] + curPos.first,dy[curFace] + curPos.second };
        if( isValid( nextPos, rows, cols ) ) {
            char& nextBlock = routeMap[nextPos.first][nextPos.second];
            if( nextBlock == '#' ) {
                curFace = ( curFace + 1 ) % 4;
                routeMap[curPos.first][curPos.second] = face[curFace];
            }
            else {
                routeMap[curPos.first][curPos.second] = 'X';
                curPos = nextPos;
                nextBlock = face[curFace];
            }
        }
        else {
            routeMap[curPos.first][curPos.second] = 'X';
            return;
        }
    }
}

void Solution1() {
    Point2D guardPos;
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

void PrintMap( unordered_map<Point2D, unordered_map<char, bool >> passed, vector<vector<char>> const& routeMap ) {
    auto canvas = routeMap;
    for( auto [pos, directitons] : passed ) {
        for( auto [d, _] : directitons ) {
            switch( d ) {
            case '^':
            case 'v':
                canvas[pos.first][pos.second] = '|';
                break;
            case '<':
            case '>':
                canvas[pos.first][pos.second] = '-';
            default:
                break;
            }
        }
    }
    for( auto line : canvas ) {
        for( auto cell : line ) {
            cout << cell;
        }
        cout << endl;
    }
}

// Thus reduced the copy time consumption.
bool isPatrolCircle( Point2D curPos, vector<vector<char>> const& routeMap, int rows, int cols ) {
    bool isCircle = false;
    // vector<vector<pair<bool, char>>> passed( rows, vector<pair<bool, char>>( cols, pair<bool, char>( false, '.' ) ) );
    unordered_map<Point2D, unordered_map<char, bool>> passed;
    int curFace = faceId[routeMap[curPos.first][curPos.second]];
    bool curStatus;
    while( isValid( curPos, rows, cols ) ) {
        // PrintMap( passed, routeMap );
        curStatus = ( passed.count( curPos ) != 0 && passed[curPos].count( face[curFace] ) != 0 ) ? true : false;
        if( curStatus ) {
            return true;
        }
        Point2D nextPos{ dx[curFace] + curPos.first,dy[curFace] + curPos.second };
        if( isValid( nextPos, rows, cols ) ) {
            if( routeMap[nextPos.first][nextPos.second] == '#' ) {
                passed[curPos][face[curFace]] = true;
                curFace = ( curFace + 1 ) % 4;
            }
            else {
                passed[curPos][face[curFace]] = true;
                curPos = nextPos;
            }
        }
        else {
            return false;
        }
    }
    return isCircle;
}

int threadTask( int startRow, int endRow, Point2D guardPos, vector<vector<char>> routeMap, atomic<int>& count ) {
    int distinctPlacementLocal = 0;
    for( int i = startRow; i < endRow; i++ ) {
        for( size_t j = 0; j < routeMap[i].size(); j++ ) {
            // count++;
            if( i == guardPos.first && j == guardPos.second || routeMap[i][j] == '#' ) {
                continue;
            }
            char curCell = routeMap[i][j];
            routeMap[i][j] = '#';
            if( isPatrolCircle( guardPos, routeMap, routeMap.size(), routeMap[0].size() ) ) {
                distinctPlacementLocal++;
            }
            routeMap[i][j] = curCell;
        }
    }
    return distinctPlacementLocal;
}

void Solution2() {
    Point2D guardPos;
    vector<vector<char>> routeMap = readFile( guardPos );

    int numThreads = std::thread::hardware_concurrency();
    vector<future<int>> futures;

    atomic<int> count = 0;
    bool enableMultiThread = true;
    int totalDistinctPlacement = 0;
    if( enableMultiThread ) {
        int rowsPerThread = routeMap.size() / numThreads;

        for( int t = 0; t < numThreads; t++ ) {
            int startRow = t * rowsPerThread;
            int endRow = ( t == numThreads - 1 ) ? routeMap.size() : ( t + 1 ) * rowsPerThread;
            // User async to start mission.
            futures.push_back( std::async( std::launch::async, threadTask, startRow, endRow, guardPos, routeMap, ref( count ) ) );
        }
        // int lastChecked = 0, total = routeMap.size() * routeMap[0].size();
        // while( lastChecked < total ) {
        //     lastChecked = count.load();

        //     cout << "Progress - Part 2" << endl;
        //     cout << "Positions checked: " << lastChecked << "/" << total << " (" << ( lastChecked * 100.0 / total ) << "%)" << endl;
        //     cout << "Loop positions found: " << count.load() << endl;
        //     this_thread::sleep_for( chrono::milliseconds( 50 ) );

        // }
        // Accumulate all res.
        for( auto& fut : futures ) {
            totalDistinctPlacement += fut.get();  // Wait and get.
        }

    }
    else {
        totalDistinctPlacement = threadTask( 0, routeMap.size(), guardPos, routeMap, ref( count ) );
    }

    cout << "Solution 2: " << totalDistinctPlacement << endl;

}