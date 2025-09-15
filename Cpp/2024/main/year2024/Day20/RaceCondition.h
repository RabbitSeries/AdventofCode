#include <fstream>
#include <functional>
#include <map>
#include <queue>
#include <ranges>
#include <string>
#include <vector>

#include "utils/ISolution.hpp"

// namespace RaceConditionData {
// using pos = pair<int, int>;
// }

// template <>
// struct std::hash<RaceConditionData::pos> {
//     size_t operator()( RaceConditionData::pos const& p ) const {
//         return p.first * 200 + p.second;
//     }
// };

class RaceCondition : public ISolution {
    REGISTER( RaceCondition )
    using pos = std::pair<int, int>;
    // using pos = RaceConditionData::pos;
    // friend struct std::hash<pos>; // This is not neccessary
    struct PairHasher {
        size_t operator()( pos const& p ) const {
            return p.first * 200 + p.second;
        }
    };

    enum cellStatus {
        WALL,
        EMPTY
    };
    using RoadMap = std::vector<std::vector<cellStatus>>;
    int dx[4]{ 0, 0, 1, -1 };
    int dy[4]{ 1, -1, 0, 0 };

    bool isValid( int rows, int cols, pos const& curPos, RoadMap const& roadMap ) {
        return curPos.first < rows && curPos.second < cols && curPos.first >= 0 && curPos.second >= 0 && roadMap[curPos.first][curPos.second] == EMPTY;
    }
    bool isWall( int rows, int cols, pos const& curPos, RoadMap const& roadMap ) {
        return curPos.first < rows && curPos.second < cols && curPos.first >= 0 && curPos.second >= 0 && roadMap[curPos.first][curPos.second] == WALL;
    }
    pos getNextPos( pos const& curPos, int id ) {
        return { curPos.first + dx[id], curPos.second + dy[id] };
    }
    int Dijkstra( pos const& start, pos const& end, RoadMap const& roadMap ) {
        int rows = roadMap.size(), cols = roadMap[0].size();
        std::vector cost( rows, std::vector<int>( cols, INT_MAX ) );
        // vector<vector<int>> optimized( rows, vector<int>( cols, false ) );
        using pqElem = std::pair<int, pos>;
        std::priority_queue<pqElem, std::vector<pqElem>, std::greater<>> pq;
        pq.push( { 0, start } );
        cost[start.first][start.second] = 0;
        path.emplace( start, 0 );
        while ( !pq.empty() ) {
            auto [curCost, curPos] = std::move( const_cast<pqElem&>( pq.top() ) );
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
                    pq.emplace( curCost + 1, std::move( nextPos ) );
                    path.emplace( nextPos, curCost + 1 );
                }
            }
        }
        // std::cout << "Path not found" << std::endl;
        return 0;
    }

    void readMap() {
        std::ifstream input( "Day20/input.txt" );
        // FILE* input( fopen( "example.txt", "r" ) );
        std::string strBuf;
        while ( getline( input, strBuf ) ) {
            std::vector<cellStatus> row;
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
            roadMap.emplace_back( std::move( row ) );
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
        for ( int i : std::views::iota( -std::min( curPos.first, 20 ), std::min( rows - 1 - curPos.first, 20 ) + 1 ) ) {
            for ( int j : std::views::iota( -std::min( curPos.second, 20 - abs( i ) ), std::min( cols - 1 - curPos.second, 20 - abs( i ) ) + 1 ) ) {
                pos checkPos{ curPos.first + i, curPos.second + j };
                int cheatCost = std::abs( i ) + std::abs( j );
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
    RoadMap roadMap;
    pos start, end;
    std::unordered_map<pos, int, PairHasher> path;

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
