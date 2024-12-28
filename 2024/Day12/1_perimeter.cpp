#include "bits/stdc++.h"
#define BUFFER_SIZE 1024
using namespace std;
typedef pair<int, int> pos;
typedef long long ll;
int getPerimeter( pos s, vector<vector<char>> map ) {
    int x = s.first, y = s.second;
    int fence = 0;
    // Up
    if( ( x - 1 >= 0 && map[x - 1][y] != map[x][y] ) || x - 1 < 0 ) {
        fence++;
    }
    // Down
    if( x + 1 < map.size() && map[x + 1][y] != map[x][y] || x + 1 >= map.size() ) {
        fence++;
    }
    // Right
    if( ( y + 1 < map[x].size() && map[x][y + 1] != map[x][y] ) || y + 1 >= map[x].size() ) {
        fence++;
    }
    // Left
    if( ( y - 1 >= 0 && map[x][y - 1] != map[x][y] ) || y - 1 < 0 ) {
        fence++;
    }
    return fence;
}

pair<int, int> getPrice( pos s, vector<vector<char>> map, vector<vector<bool>>& counted ) {
    int x = s.first, y = s.second;
    counted[x][y] = true;
    int area = 1, perimeter = getPerimeter( s, map );
    if( x - 1 >= 0 && !counted[x - 1][y] && map[x - 1][y] == map[x][y] ) {
        auto [_area, _perimeter] = getPrice( pos( x - 1, y ), map, counted );
        area += _area;
        perimeter += _perimeter;
    }
    // Down
    if( x + 1 < map.size() && !counted[x + 1][y] && map[x + 1][y] == map[x][y] ) {
        auto [_area, _perimeter] = getPrice( pos( x + 1, y ), map, counted );
        area += _area;
        perimeter += _perimeter;
    }
    // Right
    if( y + 1 < map[x].size() && !counted[x][y + 1] && map[x][y + 1] == map[x][y] ) {
        auto [_area, _perimeter] = getPrice( pos( x, y + 1 ), map, counted );
        area += _area;
        perimeter += _perimeter;
    }
    // Left
    if( y - 1 >= 0 && !counted[x][y - 1] && map[x][y - 1] == map[x][y] ) {
        auto [_area, _perimeter] = getPrice( pos( x, y - 1 ), map, counted );
        area += _area;
        perimeter += _perimeter;
    }
    return pair<int, int>( area, perimeter );
}

int main() {
    vector<vector<char>> garden;
    FILE* input = fopen( "input.txt", "r" );
    char buf[BUFFER_SIZE + 1] = "\0";
    int lineCnt = 0;
    while( !feof( input ) && fgets( buf, BUFFER_SIZE + 1, input ) ) {
        vector<char> row;
        for( char c : buf ) {
            row.push_back( c );
        }
        garden.push_back( row );
        lineCnt++;
    }

    ll res = 0;
    vector<vector<bool>> counted( lineCnt, vector<bool>( garden[0].size(), false ) );
    for( int i = 0; i < garden.size(); i++ ) {
        cout << "Processing line " << i << "." << endl;
        for( int j = 0; j < garden.size(); j++ ) {
            if( !counted[i][j] ) {
                auto [area, perimeter] = getPrice( pos( i, j ), garden, counted );
                res += area * perimeter;
            }
        }
    }
    cout << res << endl;
}