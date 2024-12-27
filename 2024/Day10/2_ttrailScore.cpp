#include "bits/stdc++.h"
#define BUFFER_SIZE 1024

#define SOLUTION_2

using namespace std;
typedef pair<int, int> pos;
typedef pair<pos, int> posInfo;
int trail( int x, int y, const vector<vector<int>> topomap, vector<vector<bool>>& visited ) {
    visited[x][y] = true;
    int score = 0;
    if( topomap[x][y] == 9 ) {
#ifdef SOLUTION_2
        visited[x][y] = false;
#endif
        return 1;
    }
    // Up
    if( x - 1 >= 0 && !visited[x - 1][y] && topomap[x - 1][y] == topomap[x][y] + 1 ) {
        score += trail( x - 1, y, topomap, visited );
    }
    // Down
    if( x + 1 < topomap.size() && !visited[x + 1][y] && topomap[x + 1][y] == topomap[x][y] + 1 ) {
        score += trail( x + 1, y, topomap, visited );
    }
    // Left
    if( y - 1 >= 0 && !visited[x][y - 1] && topomap[x][y - 1] == topomap[x][y] + 1 ) {
        score += trail( x, y - 1, topomap, visited );
    }
    // Right
    if( y + 1 < topomap[x].size() && !visited[x][y + 1] && topomap[x][y + 1] == topomap[x][y] + 1 ) {
        score += trail( x, y + 1, topomap, visited );
    }
    visited[x][y] = false;
    return score;
}
// TODO figure out how to simulate this process using stack
int main() {
    FILE* input = fopen( "input.txt", "r" );
    char linebuf[BUFFER_SIZE + 1] = { '\0' };
    vector<pair<int, int>> headList;
    vector<vector<int>> topomap;
    int lineCnt = 0;
    while( !feof( input ) && fgets( linebuf, BUFFER_SIZE, input ) ) {
        string line( linebuf );
        vector<int> row;
        for( int i = 0; i < line.size(); i++ ) {
            char c = line[i];
            if( c != '\n' && c != '\0' ) {
                row.push_back( c - '0' );
                if( c == '0' ) {
                    headList.push_back( pair<int, int>( lineCnt, i ) );
                }
            }
        }
        if( !row.empty() ) {
            topomap.push_back( row );
            lineCnt++;
        }
    }
    int res = 0;
    for( auto head : headList ) {
        vector<vector<bool>> visited( topomap.size(), vector<bool>( topomap[0].size(), false ) );
        // visited[head.first][head.second] = true;
        res += trail( head.first, head.second, topomap, visited );
    }
    cout << res << endl;
}