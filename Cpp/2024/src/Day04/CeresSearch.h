#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

#include "utils/ISolution.hpp"
#include "utils/Streams.hpp"

class CeresSearch: public ISolution {
    REGISTER( CeresSearch )

    std::vector<std::string> m;
    using Pos = std::pair<int, int>;

    const int dx[8]{ 0, 0, 1, -1, 1, 1, -1, -1 };
    const int dy[8]{ 1, -1, 0, 0, 1, -1, 1, -1 };

    const int xdx[2][3]{
        {-1, 0, 1},
        {-1, 0, 1}
    };

    const int xdy[2][3]{
        {-1, 0, 1 },
        {1,  0, -1}
    };
    int rows, cols;

    bool isValid( const Pos& pos ) {
        return pos.first >= 0 && pos.first < rows && pos.second >= 0 && pos.second < cols;
    }

    size_t isXMAS( const Pos& pos ) {
        return std::ranges::count_if( std::views::iota( 0, 8 ), [&]( int dir ) -> bool {
            return std::ranges::all_of( std::views::iota( 0, 4 ), [&]( int dis ) -> bool {
                Pos nextPos{ pos.first + dx[dir] * dis, pos.second + dy[dir] * dis };
                return isValid( nextPos ) &&
                       m[nextPos.first][nextPos.second] == dis["XMAS"];
            } );
        } );
    }

    size_t isCrossMAS( const Pos& pos ) {
        return std::ranges::all_of( std::views::iota( 0, 2 ), [&]( int line ) -> bool {
            return std::ranges::all_of( std::views::iota( 0, 3 ),
                                        [&]( int dis ) -> bool {
                                            Pos nextPos{ pos.first + xdx[line][dis],
                                                         pos.second + xdy[line][dis] };
                                            return isValid( nextPos ) &&
                                                   m[nextPos.first][nextPos.second] ==
                                                       dis["MAS"];
                                        } ) ||
                   std::ranges::all_of( std::views::iota( 0, 3 ), [&]( int dis ) -> bool {
                       Pos nextPos{ pos.first + xdx[line][dis],
                                    pos.second + xdy[line][dis] };
                       return isValid( nextPos ) &&
                              m[nextPos.first][nextPos.second] == dis["SAM"];
                   } );
        } );
    }

    void readFile() {
        m = toList( fileLinesStream( "Day04/input.txt" ) );
        rows = static_cast<int>( m.size() );
        cols = static_cast<int>( m[0].length() );
    }

    size_t sumBy( size_t ( CeresSearch::*f )( const Pos& ) ) {
        size_t res = 0;
        for ( int i = 0; i < rows; i++ ) {
            for ( int j = 0; j < cols; j++ ) {
                res += ( this->*f )( Pos{ i, j } );
            }
        }
        return res;
    }

    public:
    void Solution1() {
        readFile();
        printRes( 1, sumBy( &CeresSearch::isXMAS ) );
    }

    void Solution2() { printRes( 2, sumBy( &CeresSearch::isCrossMAS ) ); }
};
