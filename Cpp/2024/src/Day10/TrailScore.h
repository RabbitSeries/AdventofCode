#include <algorithm>
#include <cstddef>
#include <fstream>
#include <functional>
#include <map>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "utils/BufferedReader.hpp"
#include "utils/ISolution.hpp"
class TrailScore : public ISolution {
    REGISTER( TrailScore )

    using pos = std::pair<int, int> ;

    const int dx[4]{ -1, 1, 0, 0 };
    const int dy[4]{ 0, 0, -1, 1 };
    int rows, cols;
    bool isValid( const pos& p ) {
        return p.first >= 0 && p.first < rows && p.second >= 0 && p.second < cols;
    }
    int trail( const pos& p, std::vector<std::vector<bool>>& visited, bool isSolution2 ) {
        int x = p.first, y = p.second;
        visited[x][y] = true;
        int score = 0;
        if ( topomap[x][y] == 9 ) {
            // Stay visited
            if ( isSolution2 )
                visited[x][y] = false;
            return 1;
        }
        for ( int i : std::views::iota( 0, 4 ) ) {
            auto nextPos = std::pair{ x + dx[i], y + dy[i] };
            if ( isValid( nextPos ) && !visited[nextPos.first][nextPos.second] && topomap[nextPos.first][nextPos.second] == topomap[x][y] + 1 ) {
                score += trail( nextPos, visited, isSolution2 );
            }
        }
        visited[x][y] = false;
        return score;
    }

    std::vector<pos> headList;
    std::vector<std::vector<int>> topomap;

    void readFile() {
        using namespace std;
        for ( const string& line : BufferedReader( "Day10/input.txt" ).lines().yield() ) {
            auto row = line | views::transform( []( char c ) { return c - '0'; } ) | ranges::to<vector<int>>();
            for ( int i : views::iota( 0, (int)row.size() ) ) {
                if ( row[i] == 0 ) {
                    headList.emplace_back( topomap.size(), i );
                }
            };
            topomap.emplace_back( std::move( row ) );
        }
        rows = (int)topomap.size();
        cols = (int)topomap[0].size();
    }

    void interaction( bool isSolution2 ) {
        using namespace std;
        printRes( !isSolution2 ? 1 : 2, ranges::fold_left( headList | views::transform( [isSolution2, this]( const pos& head ) {
                                                               vector visited( topomap.size(), vector( topomap[0].size(), false ) );
                                                               return trail( head, visited, isSolution2 );
                                                           } ),
                                                           0, plus<>{} ) );
    }

   public:
    void Solution1() {
        readFile();
        interaction( false );
    }
    void Solution2() {
        interaction( true );
    }
};