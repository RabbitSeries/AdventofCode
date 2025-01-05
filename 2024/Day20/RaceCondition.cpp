#include <bits/stdc++.h>
#include "ProgressBar.h"
using namespace std;
#define BUF_SIZE 1024

typedef pair<int, int> pos;

enum cellStatus {
    WALL,
    EMPTY
};


int dx[4]{ 0,0,1,-1 };
int dy[4]{ 1,-1,0,0 };

bool isValid( int const& rows, int const& cols, pos const& curPos, vector<vector<cellStatus>> const& roadMap ) {
    return curPos.first < rows && curPos.second < cols && curPos.first >= 0 && curPos.second >= 0 && roadMap[curPos.first][curPos.second] == EMPTY;
}
inline bool isWall( int const& rows, int const& cols, pos const& curPos, vector<vector<cellStatus>>const& roadMap ) {
    return curPos.first < rows && curPos.second < cols && curPos.first >= 0 && curPos.second >= 0 && roadMap[curPos.first][curPos.second] == WALL;
}
inline pos getNextPos( pos curPos, int id ) {
    return pos{ curPos.first + dx[id],curPos.second + dy[id] };
}

int Dijkstra( pos const start, pos const end, vector<vector<cellStatus>> const& roadMap, map<pos, int>& path ) {
    int rows = roadMap.size(), cols = roadMap[0].size();
    vector<vector<int>> cost( rows, vector<int>( cols, INT_MAX ) );
    vector<vector<int>> optimized( rows, vector<int>( cols, false ) );
    priority_queue<pair<int, pos>, vector<pair<int, pos>>, greater<>> pq;

    pq.push( { 0,start } );
    cost[start.first][start.second] = 0;
    path.emplace( start, 0 );
    while( !pq.empty() ) {
        auto [curCost, curPos] = pq.top();
        pq.pop();
        // // TODO how to filter curCost == cost[start.first][start.second]
        if( optimized[curPos.first][curPos.second] || curCost > cost[curPos.first][curPos.second] ) {
            continue;
        }
        optimized[curPos.first][curPos.second] = true;
        if( curPos == end ) {
            return curCost;
        }
        for( int i = 0; i < 4; i++ ) {
            pos nextPos( curPos.first + dx[i], curPos.second + dy[i] );
            if( isValid( rows, cols, nextPos, roadMap ) && curCost + 1 < cost[nextPos.first][nextPos.second] ) {
                cost[nextPos.first][nextPos.second] = curCost + 1;
                pq.push( { curCost + 1,nextPos } );
                path.emplace( nextPos, curCost + 1 );
            }
        }
    }
    cout << "Path not found" << endl;
    return 0;
}


void readMap( pos& start, pos& end, vector<vector<cellStatus>>& roadMap, map<pos, int>& path ) {
    FILE* input( fopen( "input.txt", "r" ) );
    // FILE* input( fopen( "example.txt", "r" ) );
    char buf[BUF_SIZE];
    while( fgets( buf, BUF_SIZE, input ) ) {
        string strBuf( buf );
        vector<cellStatus> row;
        for( auto c : strBuf ) {

            switch( c ) {
            case '#':
                row.push_back( WALL );
                break;
            case '.':
                row.push_back( EMPTY );
                break;
            case 'S':
                start = { roadMap.size(),row.size() };
                row.push_back( EMPTY );
                break;
            case 'E':
                end = { roadMap.size(),row.size() };
                row.push_back( EMPTY );
                break;

            default:
                break;
            }

        }
        roadMap.push_back( row );
    }
    fclose( input );
    Dijkstra( start, end, roadMap, path );
    return;
}

void cheat( pos end, vector<vector<cellStatus>> const& roadMap, map<pos, int> const& path ) {
    // Benchmark satisfied.
    // vector<int> targets{ 2,4,6,8,10,12,20,36,38,40,64 };
    // vector<pair<pair<pos, pos>, int>> cheatPos;
    // for( int target : targets ) {
    int res = 0;
    int rows = roadMap.size(), cols = roadMap[0].size();
    for( auto [curPos, curCost] : path ) {
        if( isValid( rows, cols, curPos, roadMap ) ) {
            for( int k = 0; k < 4; k++ ) {
                pos wallPos = getNextPos( curPos, k );
                pos throughPos = getNextPos( wallPos, k );
                if( isWall( rows, cols, wallPos, roadMap ) && isValid( rows, cols, throughPos, roadMap ) ) {
                    int cheatCost = 2 + path.at( end ) - path.at( throughPos );
                    int savedTime = path.at( end ) - curCost - cheatCost;
                    if( savedTime > 0 ) {
                        // cheatPos.push_back( { {curPos,throughPos},savedTime } );
                        // if( savedTime == target ) {
                        if( savedTime >= 100 ) {
                            res++;
                        }
                    }
                }
            }
        }


    }
    // cout << "There are " << res << " cheats that save " << target << " picoseconds." << endl;
    cout << "Solution 1: there are " << res << " cheats that save " << 100 << " picoseconds." << endl;
    // }
    // return cheatPos;
    return;
}
void Solution1() {
    vector<vector<cellStatus>> roadMap;
    pos start, end;
    map<pos, int> path;
    readMap( start, end, roadMap, path );
    cheat( end, roadMap, path );
    return;
}

/*
     *
    * *
   *   *
  *     *
 *   +   *
  *     *
   *   *
    * *
     *
*/
inline int getCheatZone( pos const& curPos, vector<vector<cellStatus>>  const& roadMap, pos end, map<pos, int> const& path ) {
    // vector<pair<pos, int>> cheatPos;
    int cnt = 0;
    int rows = roadMap.size(), cols = roadMap[0].size();
    for( int i = -min( curPos.first, 20 ); i <= min( rows - 1 - curPos.first, 20 ); i++ ) {
        for( int j = -min( curPos.second, ( 20 - abs( i ) ) ); j <= min( cols - 1 - curPos.second, ( 20 - abs( i ) ) ); j++ ) {
            pos checkPos{ curPos.first + i,curPos.second + j };
            int cheatCost = abs( i ) + abs( j );
            if( isValid( rows, cols, checkPos, roadMap ) ) {
                int proceedCost = cheatCost + path.at( end ) - path.at( checkPos );
                int savedTime = path.at( end ) - path.at( curPos ) - proceedCost;
                if( savedTime >= 100 )
                    cnt++;
                // cheatPos.push_back( { checkPos,abs( i ) + abs( j ) } );
            }
        }
    }
    return cnt;
}

void Solution2() {
    // Benchmark satisfied.
    // vector<int> targets{ 50,52,54,56,58,60,62,64,66,68,70,72,74,76 };
    // for( auto target : targets ) {
    int res = 0;
    vector<vector<cellStatus>> roadMap;
    pos start, end;
    map<pos, int> path;
    readMap( start, end, roadMap, path );
    int pathCnt = path.size();
    int processCnt = 0;
    for( auto [startPos, curCost] : path ) {
        showProgressBar( ++processCnt, pathCnt );
        res += getCheatZone( startPos, roadMap, end, path );
    }
    // cout << "There are " << res << " cheats that save " << target << " picoseconds." << endl;
    cout << "Solution 2: there are " << res << " cheats that save " << 100 << " picoseconds." << endl;
    // }
}

int main() {
    Solution2();
    Solution1();
    return 0;
}