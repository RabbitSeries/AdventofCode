#include <algorithm>
#include <array>
#include <cstddef>
#include <map>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include "utils/BufferedReader.hpp"
#include "utils/ISolution.hpp"
class PatrolRoute : public ISolution {
    REGISTER( PatrolRoute )

    using pos = std::pair<int, int>;
    const char face[4]{ '^', '>', 'v', '<' };
    const std::map<char, unsigned int> faceId{
        { '^', 0 },
        { '>', 1 },
        { 'v', 2 },
        { '<', 3 } };
    const int dx[4]{ -1, 0, 1, 0 };
    const int dy[4]{ 0, 1, 0, -1 };

    bool isValid( pos const& curPos ) {
        return curPos.first >= 0 && curPos.first < rows && curPos.second >= 0 && curPos.second < cols;
    }

    void readFile() {
        for ( const std::string& line : BufferedReader( "Day06/input.txt" ).lines().yield() ) {
            if ( guardPos.first == -1 ) {
                for ( size_t i : std::views::iota( 0ull, line.size() ) ) {
                    if ( faceId.contains( line[i] ) ) {
                        guardPos.first = routeMap.size();
                        guardPos.second = i;
                        guardDirection = faceId.at( line[i] );
                    }
                }
            }
            routeMap.emplace_back( std::move( const_cast<std::string&>( line ) ) );
        }
        rows = routeMap.size();
        cols = routeMap[0].size();
    }

    bool patrol( std::optional<std::function<void( const pos& )>> visitor = std::nullopt ) {
        using namespace std;
        pos curPos = guardPos;
        std::vector passed = vector( routeMap.size(), vector( routeMap[0].size(), 0 ) );
        unsigned int curFace = faceId.at( routeMap[curPos.first][curPos.second] );
        while ( isValid( curPos ) ) {
            if ( visitor ) {
                visitor.value()( curPos );
            }
            if ( passed[curPos.first][curPos.second] & ( 1 << curFace ) ) {
                return true;
            }
            passed[curPos.first][curPos.second] |= ( 1 << curFace );
            pos nextPos{ dx[curFace] + curPos.first, dy[curFace] + curPos.second };
            if ( isValid( nextPos ) ) {
                if ( routeMap[nextPos.first][nextPos.second] == '#' ) {
                    curFace = ( curFace + 1 ) % 4;
                } else {
                    curPos = nextPos;
                }
            } else {
                break;
            }
        }
        return false;
    }

    std::vector<std::string> routeMap;
    int rows, cols;
    pos guardPos{ -1, 0 };
    int guardDirection;

   public:
    void Solution1() {
        readFile();
        std::vector visited = std::vector( rows, std::vector( cols, false ) );
        int count = 0;
        patrol( [&]( const pos& p ) {
            if ( !visited[p.first][p.second] ) {
                visited[p.first][p.second] = ++count;
            }
        } );
        printRes( 1, count );
    }
    void Solution2() {
        int distinctPlacement = 0;
        for ( int i = 0; i < rows; i++ ) {
            for ( int j = 0; j < cols; j++ ) {
                if ( ( guardPos.first == i && guardPos.second == j ) || routeMap[i][j] == '#' ) {
                    continue;
                }
                char curCell = routeMap[i][j];
                routeMap[i][j] = '#';
                if ( patrol() ) {
                    distinctPlacement++;
                }
                routeMap[i][j] = curCell;
            }
        }
        printRes( 2, distinctPlacement );
    }
};
