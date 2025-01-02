#include "bits/stdc++.h"
using namespace std;

#define CELLWALL -1
#define CELLEMPTY -2

typedef struct point {
    point() {}
    point( int a, int b ) : x( a ), y( b ) {}
    int x, y;
    bool operator==( point a ) {
        return this->x == a.x && this->y == a.y;
    }
}point;

typedef vector<int> path;

// typedef vector<vector<int>> maze;

typedef long long ll;
typedef enum face {
    EAST,
    SOUTH,
    WEST,
    NORTH
}face;

const int dx[4] = { 0,+1,0,-1 };
const int dy[4] = { +1,0,-1,0 };

inline int getTurn( face f1, face f2 ) {
    return ( 4 + f2 - f1 ) % 4;
}

void findWay( point s, point e, face curface, path& curPath, vector<vector<int>> m, vector<vector<bool>>& visited, priority_queue<ll, vector<ll>, greater<>>& pathPoints ) {
    if( s.x == e.x && s.y == e.y ) {
        pathPoints.push( accumulate( curPath.begin(), curPath.end(), 0ll ) );
        return;
    }
    if( s.x<0 || s.x>m.size() || s.y<0 || s.y>m[s.x].size() ) {
        cerr << "Error";
    }
    // NORTH
    if( s.x - 1 >= 0 && m[s.x - 1][s.y] != CELLWALL && !visited[s.x - 1][s.y] ) {
        point next( s.x - 1, s.y );
        curPath.push_back( getTurn( curface, NORTH ) * 1000 + 1 );
        visited[next.x][next.y] = true;
        findWay( next, e, NORTH, curPath, m, visited, pathPoints );
        visited[next.x][next.y] = false;
        curPath.pop_back();
    }
    // EAST
    if( s.y + 1 < m[s.x].size() && m[s.x][s.y + 1] != CELLWALL && !visited[s.x][s.y + 1] ) {
        point next( s.x, s.y + 1 );
        curPath.push_back( getTurn( curface, EAST ) * 1000 + 1 );
        visited[next.x][next.y] = true;
        findWay( next, e, EAST, curPath, m, visited, pathPoints );
        curPath.pop_back();
        visited[next.x][next.y] = false;
    }
    // WEST
    if( s.y - 1 >= 0 && m[s.x][s.y - 1] != CELLWALL && !visited[s.x][s.y - 1] ) {
        point next( s.x, s.y - 1 );
        curPath.push_back( getTurn( curface, WEST ) * 1000 + 1 );
        visited[next.x][next.y] = true;
        findWay( next, e, WEST, curPath, m, visited, pathPoints );
        curPath.pop_back();
        visited[next.x][next.y] = false;
    }
    // SOUTH
    if( s.x + 1 < m.size() && m[s.x + 1][s.y] != CELLWALL && !visited[s.x + 1][s.y] ) {
        point next( s.x + 1, s.y );
        curPath.push_back( getTurn( curface, SOUTH ) * 1000 + 1 );
        visited[next.x][next.y] = true;
        findWay( next, e, SOUTH, curPath, m, visited, pathPoints );
        curPath.pop_back();
        visited[next.x][next.y] = false;
    }
}
void printMaze( point s, point e, vector<vector<int>> m ) {
    m[s.x][s.y] = 'S';
    m[e.x][e.y] = 'E';
    for( auto line : m ) {
        for( auto cell : line ) {
            if( cell == CELLWALL ) {
                cout << '#';
            }
            else if( cell == CELLEMPTY ) {
                cout << '.';
            }
            else if( cell == 'S' ) {
                cout << 'S';
            }
            else {
                cout << 'E';
            }
        }
        cout << endl;
    }
}
int main() {
    vector<vector<int>> m;
    point s, e;
    face f = EAST;
    FILE* input = fopen( "input.txt", "r" );
    // Always init it, or you will have wrong result.
    char buf[1024] = "\0";
    while( !feof( input ) && fgets( buf, 1024, input ) ) {
        vector<int> row;
        for( char c : buf ) {
            if( c != '\n' && c != '\0' ) {
                if( c == '#' ) {
                    row.push_back( CELLWALL );
                }
                else if( c == '.' ) {
                    row.push_back( CELLEMPTY );
                }
                else if( c == 'S' ) {
                    s = point( m.size(), row.size() );
                    row.push_back( CELLEMPTY );
                }
                else if( c == 'E' ) {
                    e = point( m.size(), row.size() );
                    row.push_back( CELLEMPTY );
                }
            }
        }
        m.push_back( row );
    }
    printMaze( s, e, m );
    path curPath;
    priority_queue<ll, vector<ll>, greater<>> resQueue;
    vector<vector<bool>> visited( m.size(), vector<bool>( m[0].size(), false ) );
    visited[s.x][s.y] = true;
    findWay( s, e, f, curPath, m, visited, resQueue );
    cout << resQueue.top() << endl;
    return 0;
}