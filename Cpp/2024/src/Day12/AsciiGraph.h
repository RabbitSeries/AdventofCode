#include <algorithm>
#include <functional>
#include <map>
#include <queue>
#include <ranges>
#include <string>
#include <vector>

#include "utils/BufferedReader.hpp"
#include "utils/ISolution.hpp"
class AsciiGraph : public ISolution {
    REGISTER( AsciiGraph )

    const int dx[4]{ -1, 1, 0, 0 };
    const int dy[4]{ 0, 0, 1, -1 };

    bool isValid( int x, int y ) {
        // Address 0x4e3f8cc is 0 bytes after a block of size 140 alloc'd
        // return ... && (size_t)y <= garden[0].size();
        return x >= 0 && x < rows && y >= 0 && y < cols;
    }

    using pos = std::pair<int, int>;
    struct posInfo {
        int side;
        pos p;
        posInfo( int s, int i, int j ) : side{ s }, p{ i, j } {};
    };
    using ll = long long;

    int flood( const pos& s, std::vector<std::vector<bool>>& visited, std::vector<posInfo>& boundary ) {
        int area = 0;
        visited[s.first][s.second] = true;
        std::queue<pos> q( { s } );
        while ( !q.empty() ) {
            auto [x, y] = q.front();
            q.pop();
            area++;
            for ( int i = 0; i < 4; i++ ) {
                int nextX = x + dx[i], nextY = y + dy[i];
                if ( isValid( nextX, nextY ) && garden[nextX][nextY] == garden[x][y] ) {
                    if ( !visited[nextX][nextY] ) {
                        visited[nextX][nextY] = true;
                        q.emplace( nextX, nextY );
                    }
                } else {
                    boundary.emplace_back( i, nextX, nextY );
                }
            }
        }
        return area;
    }

    using mapper = std::function<int( const posInfo& )>;
    mapper byrow = []( const posInfo& b ) { return b.p.first; };
    mapper bycol = []( const posInfo& b ) { return b.p.second; };

    size_t collectBy( std::vector<posInfo> const& bounds, int side ) {
        mapper grouper = side < 2 ? byrow : bycol;
        mapper projecter = side < 2 ? bycol : byrow;
        std::map<int, std::vector<posInfo>> collect;
        for ( const auto& b : bounds ) {
            collect[grouper( b )].emplace_back( b );
        }
        size_t consecutives = 0;
        for ( auto& [_, vec] : collect ) {
            std::ranges::sort( vec, std::less<>{}, projecter );
            consecutives++;
            for ( size_t t = 1; t < vec.size(); t++ ) {
                posInfo &p = vec[t - 1], q = vec[t];
                if ( projecter( p ) != projecter( q ) - 1 ) {
                    consecutives++;
                }
            }
        }
        return consecutives;
    }

    size_t getBoundaryEdges( std::vector<posInfo>& bounds ) {
        size_t cnt = 0;
        for ( int side = 0; side < 4; side++ ) {
            std::vector<posInfo> heading = bounds |
                                           std::views::filter( [side]( const posInfo& b ) {
                                               return b.side == side;
                                           } ) |
                                           std::ranges::to<std::vector<posInfo>>();
            cnt += collectBy( heading, side );
        }
        return cnt;
    }

    std::vector<std::string> garden;
    int rows, cols;
    ll res1 = 0, res2 = 0;

   public:
    void Solution1() {
        garden = BufferedReader( "Day12/input.txt" ).lines().toList();
        rows = (int)garden.size();
        cols = (int)garden[0].size();
        std::vector visited( garden.size(), std::vector( garden[0].size(), false ) );
        for ( int i = 0; i < rows; i++ ) {
            for ( int j = 0; j < cols; j++ ) {
                if ( !visited[i][j] ) {
                    std::vector<posInfo> bounds;
                    int area = flood( pos( i, j ), visited, bounds );
                    res1 += area * bounds.size();
                    res2 += area * getBoundaryEdges( bounds );
                }
            }
        }
        printRes( 1, res1 );
    }
    void Solution2() {
        printRes( 2, res2 );
    }
};