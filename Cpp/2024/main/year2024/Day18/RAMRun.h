#include <fstream>
#include <functional>
#include <map>
#include <queue>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "utils/ISolution.hpp"
class RAMRun : public ISolution {
    REGISTER( RAMRun )

#define BUF_SIZE 100
#define SPACE 71

    enum cellStatus {
        good,
        bad
    };

    using pos = std::pair<int, int>;
    using Stamp = std::vector<std::vector<cellStatus>>;

    const int dx[4]{ 0, 0, 1, -1 };
    const int dy[4]{ 1, -1, 0, 0 };

    void readSpaceStamp( int time, std::ifstream &input ) {
        std::string buf;
        std::regex re( "[0-9]+" );
        for ( int i = 0; i < time && getline( input, buf ) && !buf.empty(); i++ ) {
            std::string bufStr( buf );
            std::sregex_iterator it( bufStr.begin(), bufStr.end(), re ), end_it;
            int x, y;
            if ( distance( it, end_it ) == 2 ) {
                // ---------> X
                // |
                // |
                // |
                // |
                // V y
                x = stoi( ( *( it++ ) ).str() );
                y = stoi( ( *( it++ ) ).str() );
                bytePos.emplace_back( x, y );
            }
        }
    }

    bool isValid( pos const &coordinate, Stamp const &stamp ) {
        int x = coordinate.first, y = coordinate.second;
        return x >= 0 && x < SPACE && y >= 0 && y < SPACE && stamp[y][x] == good;
    }

    int dijkstra( Stamp const &stamp ) {
        std::vector<std::vector<int>> step( SPACE, std::vector<int>( SPACE, INT_MAX ) );
        std::priority_queue<pair<int, pos>, std::vector<std::pair<int, pos>>, std::greater<>> pq;
        step[0][0] = 0;
        pq.push( { 0, { 0, 0 } } );
        while ( !pq.empty() ) {
            auto [curCost, curPos] = pq.top();
            pq.pop();
            if ( curCost > step[curPos.second][curPos.first] ) {
                continue;
            }
            if ( curPos == pos( SPACE - 1, SPACE - 1 ) ) {
                return curCost;
            }
            for ( int i = 0; i < 4; i++ ) {
                pos nextPos = { curPos.first + dx[i], curPos.second + dy[i] };
                if ( isValid( nextPos, stamp ) && curCost + 1 < step[nextPos.second][nextPos.first] ) {
                    step[nextPos.second][nextPos.first] = curCost + 1;
                    pq.push( { curCost + 1, nextPos } );
                }
            }
        }
        return 0;
    }

    int countCorrupted( Stamp const &stamp ) {
        int cnt = 0;
        for ( auto const &row : stamp ) {
            for ( auto &col : row ) {
                if ( col == bad ) {
                    cnt++;
                }
            }
        }
        return cnt;
    }
    std::vector<pos> bytePos;
    std::ifstream input{ "Day18/input.txt" };

   public:
    void Solution1() {
        Stamp stamp( SPACE, std::vector<cellStatus>( SPACE, good ) );
        readSpaceStamp( 1024, input );
        for_each( bytePos.begin(), bytePos.end(), [&]( pos cur ) {
            stamp[cur.second][cur.first] = bad;
        } );
        int res = dijkstra( stamp );
        if ( res )
            printRes( 1, res );
        else {
            cout << "Solution 1 failed." << endl;
            exit( -1 );
        }
    }

    void Solution2() {
        Stamp stamp( SPACE, std::vector<cellStatus>( SPACE, good ) );
        readSpaceStamp( INT_MAX, input );
        int left = 0, right = bytePos.size() - 1;
        while ( left < right ) {
            Stamp curStamp = stamp;
            int mid = ( left + right ) / 2;
            for_each( bytePos.begin(), bytePos.begin() + mid + 1, [&]( pos cur ) {
                curStamp[cur.second][cur.first] = bad;
            } );
            if ( dijkstra( curStamp ) ) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        std::ostringstream ss;
        ss << bytePos[left].first << "," << bytePos[left].second;
        printRes( 2, ss.str() );
    }
};
