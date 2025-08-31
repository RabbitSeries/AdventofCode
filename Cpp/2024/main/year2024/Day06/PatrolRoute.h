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

    using pos = std::pair<size_t, size_t>;
    const char face[4]{ '^', '>', 'v', '<' };
    const std::unordered_map<char, int> faceId{
        { '^', 0 },
        { '>', 1 },
        { 'v', 2 },
        { '<', 3 } };
    const int dx[4]{ -1, 0, 1, 0 };
    const int dy[4]{ 0, 1, 0, -1 };

    // Inclass init, use {}
    // atomic<int> progress( 0 );
    // char face[] = { '^','>','v','<' };//an initializer cannot be specified for a flexible array member
    inline bool isValid( pos const& curPos ) {
        return curPos.first >= 0 && curPos.first < rows && curPos.second >= 0 && curPos.second < cols;
    }

    size_t rows, cols;

    void readFile() {
        for ( std::string line : BufferedReader( "Day06/input.txt" ).lines().yield() ) {
            for ( size_t i : std::views::iota( 0ull, line.size() ) ) {
                if ( line[i] == '<' || line[i] == '>' || line[i] == '^' || line[i] == 'v' ) {
                    guardPos.first = routeMap.size();
                    guardPos.second = i;
                }
            }
            routeMap.emplace_back( std::move( line ) );
        }
        rows = routeMap.size();
        cols = routeMap[0].size();
    }

    std::vector<std::vector<std::array<bool, 4>>> passed;
    std::vector<std::vector<bool>> visited;

    std::pair<bool, int> patrol( bool enablePlacementCnt = true ) {
        using namespace std;
        pos curPos = guardPos;
        int distinctPosCnt = 0;
        if ( enablePlacementCnt ) {
            visited = vector( routeMap.size(), vector( routeMap.at( 0 ).size(), false ) );
        }
        passed = vector( routeMap.size(), vector( routeMap[0].size(), array<bool, 4>{ false } ) );
        int curFace = faceId.at( routeMap[curPos.first][curPos.second] );
        while ( isValid( curPos ) ) {
            if ( enablePlacementCnt && !visited[curPos.first][curPos.second] ) {
                visited[curPos.first][curPos.second] = true;
                distinctPosCnt++;
            }
            if ( passed[curPos.first][curPos.second][curFace] ) {
                return { true, distinctPosCnt };
            } else {
                passed[curPos.first][curPos.second][curFace] = true;
            }
            pos nextPos{ dx[curFace] + curPos.first, dy[curFace] + curPos.second };
            if ( isValid( nextPos ) ) {
                //     // There is no need to update before turnning directions, cause this location's direction must not be updated if this location is a loop's end and it will be updated if this location is not loop's end.
                //     // However, in general mark every status on this block can be another useful approach (implemented)
                if ( routeMap[nextPos.first][nextPos.second] == '#' ) {
                    curFace = ( curFace + 1 ) % 4;
                } else {
                    curPos = nextPos;
                }
            } else {
                break;
            }
        }
        return { false, distinctPosCnt };
    }
    std::vector<std::string> routeMap;
    pos guardPos{ 0, 0 };

   public:
    void Solution1() {
        readFile();
        using namespace std;
        printRes( 1, patrol().second );
    }

    void Solution2() {
        int distinctPlacement = 0;
        for ( size_t i = 0; i < routeMap.size(); i++ ) {
            for ( size_t j = 0; j < routeMap[0].size(); j++ ) {
                if ( ( guardPos == std::pair{ i, j } ) || routeMap[i][j] == '#' ) {
                    continue;
                }
                char curCell = routeMap[i][j];
                routeMap[i][j] = '#';
                if ( patrol( false ).first ) {
                    distinctPlacement++;
                }
                routeMap[i][j] = curCell;
            }
        }
        printRes( 2, distinctPlacement );
    }
};
