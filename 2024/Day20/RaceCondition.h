#include <bits/stdc++.h>
using namespace std;
#include "../../utils/SolutionBase.h"

namespace RaceConditionData {
using pos = pair<int, int>;
}

template <>
struct std::hash<RaceConditionData::pos> {
    size_t operator()( RaceConditionData::pos const& p ) const {
        return p.first * 200 + p.second;
    }
};

class RaceCondition : public SolutionBase {
    using pos = RaceConditionData::pos;
    friend struct std::hash<pos>;
    enum cellStatus {
        WALL,
        EMPTY
    };

    int dx[4]{ 0, 0, 1, -1 };
    int dy[4]{ 1, -1, 0, 0 };

    bool isValid( int rows, int cols, pos const& curPos, vector<vector<cellStatus>> const& roadMap ) {
        return curPos.first < rows && curPos.second < cols && curPos.first >= 0 && curPos.second >= 0 && roadMap[curPos.first][curPos.second] == EMPTY;
    }
    bool isWall( int rows, int cols, pos const& curPos, vector<vector<cellStatus>> const& roadMap ) {
        return curPos.first < rows && curPos.second < cols && curPos.first >= 0 && curPos.second >= 0 && roadMap[curPos.first][curPos.second] == WALL;
    }
    pos getNextPos( pos const& curPos, int id ) {
        return { curPos.first + dx[id], curPos.second + dy[id] };
    }

    int Dijkstra( pos const& start, pos const& end, vector<vector<cellStatus>> const& roadMap ) {
        int rows = roadMap.size(), cols = roadMap[0].size();
        vector<vector<int>> cost( rows, vector<int>( cols, INT_MAX ) );
        // vector<vector<int>> optimized( rows, vector<int>( cols, false ) );
        using pqElem = pair<int, pos>;
        priority_queue<pqElem, vector<pqElem>, greater<>> pq;
        pq.push( { 0, start } );
        cost[start.first][start.second] = 0;
        path.emplace( start, 0 );
        while ( !pq.empty() ) {
            auto [curCost, curPos] = move( const_cast<pqElem&>( pq.top() ) );
            pq.pop();
            // // TODO how to filter curCost == cost[start.first][start.second]
            if ( /* optimized[curPos.first][curPos.second] || */ curCost > cost[curPos.first][curPos.second] ) {
                continue;
            }
            // optimized[curPos.first][curPos.second] = true;
            if ( curPos == end ) {
                return curCost;
            }
            for ( int i = 0; i < 4; i++ ) {
                pos nextPos( curPos.first + dx[i], curPos.second + dy[i] );
                if ( isValid( rows, cols, nextPos, roadMap ) && curCost + 1 < cost[nextPos.first][nextPos.second] ) {
                    cost[nextPos.first][nextPos.second] = curCost + 1;
                    pq.emplace( curCost + 1, move( nextPos ) );
                    path.emplace( nextPos, curCost + 1 );
                }
            }
        }
        cout << "Path not found" << endl;
        return 0;
    }

    void readMap() {
        ifstream input( "Day20/input.txt" );
        // FILE* input( fopen( "example.txt", "r" ) );
        string strBuf;
        while ( getline( input, strBuf ) ) {
            vector<cellStatus> row;
            for ( auto c : strBuf ) {
                switch ( c ) {
                    case '#':
                        row.push_back( WALL );
                        break;
                    case '.':
                        row.push_back( EMPTY );
                        break;
                    case 'S':
                        start = { roadMap.size(), row.size() };
                        row.push_back( EMPTY );
                        break;
                    case 'E':
                        end = { roadMap.size(), row.size() };
                        row.push_back( EMPTY );
                        break;

                    default:
                        break;
                }
            }
            roadMap.emplace_back( move( row ) );
        }
        input.close();
        return;
    }

    int cheat() {
        // Benchmark satisfied.
        // vector<int> targets{ 2,4,6,8,10,12,20,36,38,40,64 };
        // vector<pair<pair<pos, pos>, int>> cheatPos;
        // for( int target : targets ) {
        int res = 0;
        int rows = roadMap.size(), cols = roadMap[0].size();
        for ( auto const& [curPos, curCost] : path ) {
            if ( isValid( rows, cols, curPos, roadMap ) ) {
                for ( int k = 0; k < 4; k++ ) {
                    pos wallPos = getNextPos( curPos, k );
                    pos throughPos = getNextPos( wallPos, k );
                    if ( isWall( rows, cols, wallPos, roadMap ) && isValid( rows, cols, throughPos, roadMap ) ) {
                        int cheatCost = 2 + path.size() - path.at( throughPos );
                        int savedTime = path.size() - curCost - cheatCost;
                        if ( savedTime > 0 ) {
                            // cheatPos.push_back( { {curPos,throughPos},savedTime } );
                            // if( savedTime == target ) {
                            if ( savedTime >= 100 ) {
                                res++;
                            }
                        }
                    }
                }
            }
        }
        return res;
    }
    // /*
    //      *
    //     * *
    //    *   *
    //   *     *
    //  *   +   *
    //   *     *
    //    *   *
    //     * *
    //      *
    //  */
    int getCheatZone( pos const& curPos ) {
        int cnt = 0;
        int rows = roadMap.size(), cols = roadMap[0].size();
        for ( int i = -min( curPos.first, 20 ); i <= min( rows - 1 - curPos.first, 20 ); i++ ) {
            for ( int j = -min( curPos.second, ( 20 - abs( i ) ) ); j <= min( cols - 1 - curPos.second, ( 20 - abs( i ) ) ); j++ ) {
                pos checkPos{ curPos.first + i, curPos.second + j };
                int cheatCost = abs( i ) + abs( j );
                if ( isValid( rows, cols, checkPos, roadMap ) ) {
                    int proceedCost = cheatCost + path.size() - path.at( checkPos );
                    int savedTime = path.size() - path.at( curPos ) - proceedCost;
                    if ( savedTime >= 100 )
                        cnt++;
                }
            }
        }
        return cnt;
    }
    vector<vector<cellStatus>> roadMap;
    pos start, end;
    unordered_map<pos, int> path;

   public:
    void Solution1() {
        readMap();
        Dijkstra( start, end, roadMap );
        printRes( 1, cheat() );
        return;
    }

    void Solution2() {
        int res = 0;
        // int pathCnt = path.size();
        // int processCnt = 0;
        for ( auto const& [startPos, _] : path ) {
            // showProgressBar( ++processCnt, pathCnt );
            res += getCheatZone( startPos );
        }
        printRes( 2, res );
    }
};
