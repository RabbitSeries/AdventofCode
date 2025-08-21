#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

#include "utils/SolutionBase.hpp"
#include "utils/Stream/RegexStream.hpp"
class CeresSearch : public SolutionBase {
    REGISTER( CeresSearch )

    std::vector<std::string> m;

    const int dx[8]{ 0, 0, 1, -1, 1, 1, -1, -1 };
    const int dy[8]{ 1, -1, 0, 0, 1, -1, 1, -1 };

    const int xdx[2][3]{
        { -1, 0, 1 },
        { -1, 0, 1 } };

    const int xdy[2][3]{
        { -1, 0, 1 },
        { 1, 0, -1 } };
    size_t rows, cols;

    bool isValid( const std::pair<size_t, size_t>& pos ) {
        return pos.first >= 0 && pos.first < rows && pos.second >= 0 && pos.second < cols;
    }

    size_t isXMAS( const std::pair<size_t, size_t>& pos ) {
        return std::ranges::count_if( std::views::iota( 0, 8 ), [&]( int dir ) -> bool {
            return std::ranges::all_of( std::views::iota( 0, 4 ), [&]( int dis ) -> bool {
                std::pair<size_t, size_t> nextPos{ pos.first + dx[dir] * dis, pos.second + dy[dir] * dis };
                return isValid( nextPos ) && m[nextPos.first][nextPos.second] == dis["XMAS"];
            } );
        } );
    }

    size_t isCrossMAS( const std::pair<size_t, size_t>& pos ) {
        return std::ranges::all_of( std::views::iota( 0, 2 ), [&]( int line ) -> bool {
            return std::ranges::all_of( std::views::iota( 0, 3 ), [&]( int dis ) -> bool {
                       std::pair nextPos{ pos.first + xdx[line][dis], pos.second + xdy[line][dis] };
                       return isValid( nextPos ) && m[nextPos.first][nextPos.second] == dis["MAS"];
                   } ) ||
                   std::ranges::all_of( std::views::iota( 0, 3 ), [&]( int dis ) -> bool {
                       std::pair nextPos{ pos.first + xdx[line][dis], pos.second + xdy[line][dis] };
                       return isValid( nextPos ) && m[nextPos.first][nextPos.second] == dis["SAM"];
                   } );
        } );
    }

    void readFile() {
        m = BufferedReader( "Day04/input.txt" ).lines().toList();
        rows = m.size();
        cols = m[0].length();
    }

    int sumBy( size_t ( CeresSearch::*f )( const std::pair<size_t, size_t>& ) ) {
        int res = 0;
        for ( size_t i = 0; i < rows; i++ ) {
            for ( size_t j = 0; j < cols; j++ ) {
                res += ( this->*f )( { i, j } );
            }
        }
        return res;
    }

   public:
    void Solution1() {
        readFile();
        printRes( 1, sumBy( &CeresSearch::isXMAS ) );
    }

    void Solution2() {
        printRes( 2, sumBy( &CeresSearch::isCrossMAS ) );
    }
};