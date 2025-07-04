#include "bits/stdc++.h"
using namespace std;
#include <utils/SolutionBase.hpp>
class PatrolRoute : public SolutionBase {
    using pos = pair<int, int>;
    const char face[4]{ '^', '>', 'v', '<' };
    const unordered_map<char, int> faceId{
        { '^', 0 },
        { '>', 1 },
        { 'v', 2 },
        { '<', 3 } };
    const int dx[4]{ -1, 0, 1, 0 };
    const int dy[4]{ 0, 1, 0, -1 };

    // Inclass init, use {}
    // atomic<int> progress( 0 );
    // char face[] = { '^','>','v','<' };//an initializer cannot be specified for a flexible array member
    inline bool isValid( pos const& curPos, int rows, int cols ) {
        int x = curPos.first, y = curPos.second;
        return x >= 0 && x < rows && y >= 0 && y < cols;
    }

    void readFile() {
        ifstream input( "Day06/input.txt" );
        vector<char> row;
        for ( string lineBuf; getline( input, lineBuf ); ) {
            for ( int i = 0; lineBuf[i] != '\n' && lineBuf[i] != '\0'; i++ ) {
                if ( lineBuf[i] == '<' || lineBuf[i] == '>' || lineBuf[i] == '^' || lineBuf[i] == 'v' ) {
                    guardPos.first = routeMap.size();
                    guardPos.second = i;
                }
                row.push_back( lineBuf[i] );
            }
            if ( !row.empty() ) {
                routeMap.emplace_back( move( row ) );
                row.clear();
            }
        }
        input.close();
    }
    vector<vector<array<bool, 4>>> passed;
    vector<vector<bool>> visited;

    pair<bool, int> patrol( vector<vector<char>> const& routeMap, bool enablePlacementCnt = true ) {
        pos curPos = guardPos;
        int rows = routeMap.size(), cols = routeMap[0].size();
        int distinctPosCnt = 0;
        if ( enablePlacementCnt ) {
            for ( auto& row : visited ) {
                fill( row.begin(), row.end(), false );
            }
        }
        for ( auto& row : passed ) {
            for ( auto& cell : row ) {
                fill( cell.begin(), cell.end(), false );
            }
        }
        int curFace = faceId.at( routeMap[curPos.first][curPos.second] );
        while ( isValid( curPos, rows, cols ) ) {
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
            if ( isValid( nextPos, rows, cols ) ) {
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
    vector<vector<char>> routeMap;
    pos guardPos{ 0, 0 };

   public:
    void Solution1() {
        readFile();
        visited = vector( routeMap.size(), vector( routeMap.at( 0 ).size(), false ) );
        passed = vector( routeMap.size(), vector( routeMap[0].size(), array<bool, 4>{ false } ) );
        printRes( 1, patrol( routeMap ).second );
    }

    void Solution2() {
        int distinctPlacement = 0;
        for ( size_t i = 0; i < routeMap.size(); i++ ) {
            for ( size_t j = 0; j < routeMap[0].size(); j++ ) {
                if ( ( i == (size_t)guardPos.first && j == (size_t)guardPos.second ) || routeMap[i][j] == '#' ) {
                    continue;
                }
                char curCell = routeMap[i][j];
                routeMap[i][j] = '#';
                if ( patrol( routeMap, false ).first ) {
                    distinctPlacement++;
                }
                routeMap[i][j] = curCell;
            }
        }
        printRes( 2, distinctPlacement );
    }
};
