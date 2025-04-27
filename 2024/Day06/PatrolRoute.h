#include "bits/stdc++.h"
using namespace std;

typedef pair<int, int> pos;

#include "../../utils/SolutionBase.h"
class PatrolRoute : public SolutionBase {
    static atomic<int> distinctPlacement;
    static mutex mtx;

    static const char face[];
    static const unordered_map<char, int> faceId;
    static const int dx[];
    static const int dy[];

    // Inclass init, use {}
    // atomic<int> progress( 0 );
    // char face[] = { '^','>','v','<' };//an initializer cannot be specified for a flexible array member
    static inline bool isValid( pos const& curPos, int rows, int cols ) {
        int x = curPos.first, y = curPos.second;
        return x >= 0 && x < rows && y >= 0 && y < cols;
    }

    void readFile() {
        ifstream input( "Day06/input.txt" );
        vector<char> row;
        for ( string lineBuf; getline( input, lineBuf ); ) {
            for ( int i = 0; lineBuf[i] != '\n' && lineBuf[i] != '\0'; i++ ) {
                if ( lineBuf[i] == '<' || lineBuf[i] == '>' || lineBuf[i] == '^' || lineBuf[i] == 'v' ) {
                    guardPos.first = routeMap.size();
                    guardPos.second = i;
                }
                row.push_back( lineBuf[i] );
            }
            if ( !row.empty() ) {
                routeMap.emplace_back( move( row ) );
                row.clear();
            }
        }
        input.close();
    }

    int patrol( vector<vector<char>> const& routeMap ) {
        pos curPos = guardPos;
        int distinctPosCnt = 0;
        vector<vector<bool>> visited( routeMap.size(), vector<bool>( routeMap.at( 0 ).size(), false ) );
        int rows = routeMap.size(), cols = routeMap[0].size();
        int curFace = faceId.at( routeMap[curPos.first][curPos.second] );
        while ( isValid( curPos, rows, cols ) ) {
            pos nextPos{ dx[curFace] + curPos.first, dy[curFace] + curPos.second };
            if ( isValid( nextPos, rows, cols ) ) {
                if ( !visited[curPos.first][curPos.second] ) {
                    visited[curPos.first][curPos.second] = true;
                    distinctPosCnt++;
                }
                if ( routeMap[nextPos.first][nextPos.second] == '#' ) {
                    curFace = ( curFace + 1 ) % 4;
                } else {
                    curPos = nextPos;
                }
            } else {
                return distinctPosCnt;
            }
        }
        return distinctPosCnt;
    }

    // Thus reduced the copy time consumption.
    static bool isPatrolCircle( pos curPos, vector<vector<char>> const& routeMap, int rows, int cols ) {
        bool isCircle = false;
        vector<vector<array<bool, 4>>> passed( rows, vector<array<bool, 4>>( cols, array<bool, 4>() ) );
        int curFace = faceId.at( routeMap[curPos.first][curPos.second] );
        bool curStatus;
        while ( isValid( curPos, rows, cols ) ) {
            curStatus = passed[curPos.first][curPos.second][curFace];
            if ( curStatus ) {
                return true;
            }
            pos nextPos{ dx[curFace] + curPos.first, dy[curFace] + curPos.second };
            if ( isValid( nextPos, rows, cols ) ) {
                // There is no need to update before turnning directions, cause this location's direction must not be updated if this location is a loop's end and it will be updated if this location is not loop's end.
                // However, in general mark every status on this block can be another useful approach
                passed[curPos.first][curPos.second][curFace] = true;
                if ( routeMap[nextPos.first][nextPos.second] == '#' ) {
                    curFace = ( curFace + 1 ) % 4;
                } else {
                    curPos = nextPos;
                }
            } else {
                return false;
            }
        }
        return isCircle;
    }

    static void threadTask( int startRow, size_t endRow, pair<size_t, size_t> guardPos, vector<vector<char>> routeMap ) {
        int zoneRes = 0;
        for ( size_t i = startRow; i < endRow; i++ ) {
            for ( size_t j = 0; j < routeMap[i].size(); j++ ) {
                if ( ( i == guardPos.first && j == guardPos.second ) || routeMap[i][j] == '#' ) {
                    continue;
                }
                char curCell = routeMap[i][j];
                routeMap[i][j] = '#';
                if ( isPatrolCircle( guardPos, routeMap, routeMap.size(), routeMap[0].size() ) ) {
                    zoneRes++;
                }
                routeMap[i][j] = curCell;
            }
        }
        lock_guard<mutex> resultLock( mtx );
        distinctPlacement += zoneRes;
    }

    vector<vector<char>> routeMap;
    pos guardPos{ 0, 0 };

   public:
    void Solution1() {
        readFile();
        printRes( 1, patrol( routeMap ) );
    }

    void Solution2() {
        int numThreads = std::thread::hardware_concurrency();
        vector<thread> threads;
        int rowsPerThread = routeMap.size() / numThreads;
        for ( int t = 0; t < numThreads; t++ ) {
            int startRow = t * rowsPerThread;
            int endRow = ( t == numThreads - 1 ) ? routeMap.size() : ( t + 1 ) * rowsPerThread;
            threads.push_back( thread( threadTask, startRow, endRow, guardPos, routeMap ) );
        }
        for ( auto& t : threads ) {
            t.join();
        }
        printRes( 2, distinctPlacement.load() );
    }
};
