#include "bits/stdc++.h"
using namespace std;

#include <utils/SolutionBase.hpp>
class TrailScore : public SolutionBase {
    typedef pair<int, int> pos;
    typedef pair<pos, int> posInfo;

    int trail( size_t x, size_t y, const vector<vector<int>> topomap, vector<vector<bool>>& visited, bool isSolution2 ) {
        visited[x][y] = true;
        int score = 0;
        if ( topomap[x][y] == 9 ) {
            // Stay visited
            if ( isSolution2 )
                visited[x][y] = false;
            return 1;
        }
        // Up
        if ( x >= 1 && !visited[x - 1][y] && topomap[x - 1][y] == topomap[x][y] + 1 ) {
            score += trail( x - 1, y, topomap, visited, isSolution2 );
        }
        // Down
        if ( x + 1 < topomap.size() && !visited[x + 1][y] && topomap[x + 1][y] == topomap[x][y] + 1 ) {
            score += trail( x + 1, y, topomap, visited, isSolution2 );
        }
        // Left
        if ( y >= 1 && !visited[x][y - 1] && topomap[x][y - 1] == topomap[x][y] + 1 ) {
            score += trail( x, y - 1, topomap, visited, isSolution2 );
        }
        // Right
        if ( y + 1 < topomap[x].size() && !visited[x][y + 1] && topomap[x][y + 1] == topomap[x][y] + 1 ) {
            score += trail( x, y + 1, topomap, visited, isSolution2 );
        }
        visited[x][y] = false;
        return score;
    }

    void interaction( bool isSolution2 ) {
        ifstream input( "Day10/input.txt" );
        string linebuf;
        vector<pair<int, int>> headList;
        vector<vector<int>> topomap;
        int lineCnt = 0;
        while ( getline( input, linebuf ) ) {
            vector<int> row;
            for ( size_t i = 0; i < linebuf.size(); i++ ) {
                char c = linebuf[i];
                if ( c != '\n' && c != '\0' ) {
                    row.push_back( c - '0' );
                    if ( c == '0' ) {
                        headList.emplace_back( lineCnt, i );
                    }
                }
            }
            if ( !row.empty() ) {
                topomap.push_back( row );
                lineCnt++;
            }
        }
        int res = 0;
        for ( auto& head : headList ) {
            vector<vector<bool>> visited( topomap.size(), vector<bool>( topomap[0].size(), false ) );
            res += trail( head.first, head.second, topomap, visited, isSolution2 );
        }
        !isSolution2 ? printRes( 1, res ) : printRes( 2, res );
    }

   public:
    void Solution1() {
        interaction( false );
    }
    void Solution2() {
        interaction( true );
    }
};