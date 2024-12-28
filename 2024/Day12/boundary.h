#include "bits/stdc++.h"
#define BUFFER_SIZE 1024
using namespace std;
typedef pair<int, int> pos;
typedef long long ll;

bool isUpBoundary( const pos s, const  vector<vector<char>> garden ) {
    int x = s.first, y = s.second;
    if( ( x - 1 >= 0 && garden[x - 1][y] != garden[x][y] ) || x - 1 < 0 ) {
        return true;
    }
    return false;
}
bool isDownBoundary( const pos s, const  vector<vector<char>> garden ) {
    int x = s.first, y = s.second;
    if( x + 1 < garden.size() && garden[x + 1][y] != garden[x][y] || x + 1 >= garden.size() ) {
        return true;
    }
    return false;
}
bool isRightBoundary( const pos s, const  vector<vector<char>> garden ) {
    int x = s.first, y = s.second;
    if( ( y + 1 < garden[x].size() && garden[x][y + 1] != garden[x][y] ) || y + 1 >= garden[x].size() ) {
        return true;
    }
    return false;
}
bool isLeftBoundary( const pos s, const  vector<vector<char>> garden ) {
    int x = s.first, y = s.second;
    if( ( y - 1 >= 0 && garden[x][y - 1] != garden[x][y] ) || y - 1 < 0 ) {
        return true;
    }
    return false;
}


int getBoundarys( pos s, vector<vector<char>> garden, vector<vector<bool>>& visited, vector<pos>& boundary ) {
    int x = s.first, y = s.second;
    visited[x][y] = true;
    int area = 1;
    if( isUpBoundary( s, garden ) || isDownBoundary( s, garden ) || isLeftBoundary( s, garden ) || isRightBoundary( s, garden ) ) {
        boundary.push_back( pos( x, y ) );
    }
    // Up fence
    if( x - 1 >= 0 && !visited[x - 1][y] && garden[x - 1][y] == garden[x][y] ) {
        area += getBoundarys( pos( x - 1, y ), garden, visited, boundary );
    }
    // Down
    if( x + 1 < garden.size() && !visited[x + 1][y] && garden[x + 1][y] == garden[x][y] ) {
        area += getBoundarys( pos( x + 1, y ), garden, visited, boundary );
    }
    // Left
    if( y - 1 >= 0 && !visited[x][y - 1] && garden[x][y - 1] == garden[x][y] ) {
        area += getBoundarys( pos( x, y - 1 ), garden, visited, boundary );
    }
    // Right
    if( y + 1 < garden[x].size() && !visited[x][y + 1] && garden[x][y + 1] == garden[x][y] ) {
        area += getBoundarys( pos( x, y + 1 ), garden, visited, boundary );
    }
    return area;
}

void printGarden( vector<vector<char>> const garden ) {
    for( int i = 0; i < garden.size(); i++ ) {
        for( int j = 0; j < garden.size(); j++ ) {
            cout << garden[i][j];
        }
        cout << endl;
    }
}
