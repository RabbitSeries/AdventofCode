#include "bits/stdc++.h"
using namespace std;

typedef pair<int, int> pos;

#include "../../utils/SolutionBase.h"
class PatrolRoute : public SolutionBase {
    static atomic<int> distinctPlacement;
    static mutex mtx;
    static atomic<int> progress;
    static mutex progressMtx;
    static void progressIncrement() {
        lock_guard<mutex> resultLock{ mtx };
        progress++;
    }
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

    vector<vector<char>> readFile( pair<int, int>& guardPos ) {
        vector<vector<char>> routeMap;
        char lineBuf[BUFSIZ] = { '\0' };
        FILE* input = fopen( "Day06/input.txt", "r" );
        while ( fgets( lineBuf, BUFSIZ, input ) ) {
            vector<char> row;
            int i = 0;
            while ( lineBuf[i] != '\n' && lineBuf[i] != '\0' ) {
                if ( lineBuf[i] == '<' || lineBuf[i] == '>' || lineBuf[i] == '^' || lineBuf[i] == 'v' ) {
                    guardPos.first = routeMap.size();
                    guardPos.second = i;
                }
                row.push_back( lineBuf[i] );
                i++;
            }
            if ( !row.empty() ) {
                routeMap.push_back( row );
            }
        }
        fclose( input );
        return routeMap;
    }

    vector<vector<bool>> patrol( pos curPos, vector<vector<char>> const& routeMap ) {
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
                return visited;
            }
        }
        return visited;
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
        for ( size_t i = startRow; i < endRow; i++ ) {
            for ( size_t j = 0; j < routeMap[i].size(); j++ ) {
                progressIncrement();
                if ( ( i == guardPos.first && j == guardPos.second ) || routeMap[i][j] == '#' ) {
                    continue;
                }
                char curCell = routeMap[i][j];
                routeMap[i][j] = '#';
                if ( isPatrolCircle( guardPos, routeMap, routeMap.size(), routeMap[0].size() ) ) {
                    lock_guard<mutex> resultLock( mtx );
                    distinctPlacement++;
                }
                routeMap[i][j] = curCell;
            }
        }
    }

    int distinctPosCnt = 0;

   public:
    void Solution1() {
        pos guardPos( 0, 0 );
        vector<vector<char>> routeMap = readFile( guardPos );
        patrol( guardPos, routeMap );
    }

    void Solution2() {
        pos guardPos( 0, 0 );
        vector<vector<char>> routeMap = readFile( guardPos );

        int numThreads = std::thread::hardware_concurrency();
        vector<thread> threads;

        int rowsPerThread = routeMap.size() / numThreads;

        for ( int t = 0; t < numThreads; t++ ) {
            int startRow = t * rowsPerThread;
            int endRow = ( t == numThreads - 1 ) ? routeMap.size() : ( t + 1 ) * rowsPerThread;

            threads.push_back( thread( threadTask, startRow, endRow, guardPos, routeMap ) );
        }

        // int lastChecked = 0, curChecked = 0, total = routeMap.size() * routeMap[0].size();
        // while( lastChecked < total ) {
        //     curChecked = progress.load();
        //     if( curChecked - lastChecked > 500 || curChecked == total ) {
        //         lastChecked = curChecked;
        //         std::cout << "\033[2J\033[1;1H";
        //         cout << "Progress - Part 2" << endl;
        //         cout << "Positions checked: " << lastChecked << "/" << total << " (" << ( lastChecked * 100.0 / total ) << "%)" << endl;
        //         cout << "Loop positions found: " << distinctPlacement.load() << endl;
        //     }
        // }

        for ( auto& t : threads ) {
            t.join();
        }

        cout << "Solution 1: " << distinctPosCnt << endl;
        cout << "Solution 2: " << distinctPlacement.load() << endl;
    }
};
