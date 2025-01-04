#include <bits/stdc++.h>
using namespace std;
#define BUF_SIZE 1024
typedef pair<int, int> pos;
enum cellStatus {
    WALL,
    EMPTY
};

int dx[4]{ 0,0,1,-1 };
int dy[4]{ 1,-1,0,0 };

bool isValid( int rows, int cols, pos curPos, vector<vector<cellStatus>> roadMap ) {
    return curPos.first < rows && curPos.second < cols && curPos.first >= 0 && curPos.second >= 0 && roadMap[curPos.first][curPos.second] == EMPTY;
}
bool isWall( int rows, int cols, pos curPos, vector<vector<cellStatus>> roadMap ) {
    return curPos.first < rows && curPos.second < cols && curPos.first >= 0 && curPos.second >= 0 && roadMap[curPos.first][curPos.second] == WALL;
}
pos getNextPos( pos curPos, int id ) {
    return pos{ curPos.first + dx[id],curPos.second + dy[id] };
}

int Dijkstra( pos const start, pos const end, vector<vector<cellStatus>> const roadMap, map<pos, int>& path ) {
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

vector<pair<pair<pos, pos>, int>> cheat( pos end, vector<vector<cellStatus>> const roadMap, map<pos, int> const path ) {
    int res = 0;
    vector<pair<pair<pos, pos>, int>> cheatPos;
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
                        cheatPos.push_back( { {curPos,throughPos},savedTime } );
                        if( savedTime >= 100 ) {
                            res++;
                        }
                    }
                }
            }
        }


    }
    cout << "The cheat positions that can save at least 100 picoseconds count: " << res << endl;
    return cheatPos;
}

void Solution1() {
    FILE* input( fopen( "input.txt", "r" ) );
    char buf[BUF_SIZE];
    vector<vector<cellStatus>> roadMap;
    pos start, end;
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
    map<pos, int> path;
    Dijkstra( start, end, roadMap, path );

    cheat( end, roadMap, path );
    fclose( input );
}

int main() {
    Solution1();
    return 0;
}