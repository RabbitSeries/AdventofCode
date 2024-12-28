#include "boundary.h"
// #include <./boundary.h>
int getBoundaryEdges( vector<pos> const boundary, vector<vector<char>> const garden ) {
    vector<vector<bool>> visited( garden.size(), vector<bool>( garden[0].size(), false ) );
    // TODO print the boundary with direction out.
    vector<vector<char>> gardenCanvas = garden;
    // Up
    int upedge = 0;
    for( int i = 0; i < boundary.size(); i++ ) {
        pos s = boundary[i];
        int x = s.first, y = s.second;
        if( !visited[x][y] ) {
            if( isUpBoundary( s, garden ) ) {
                // Has up fence
                // Draw horizontal line
                upedge++;
                while( y - 1 >= 0 && garden[x][y - 1] == garden[x][y] && !visited[x][y - 1] && isUpBoundary( pos( x, y - 1 ), garden ) ) {
                    cout << "Draw up edge From " << s.first << " " << s.second << " to " << x << " " << y - 1 << "." << endl;
                    y--;
                }
                visited[x][y] = true;
                gardenCanvas[x][y] = '-';
                while( y + 1 < garden[x].size() && garden[x][y + 1] == garden[x][y] && !visited[x][y + 1] && isUpBoundary( pos( x, y + 1 ), garden ) ) {
                    visited[x][++y] = true;
                    gardenCanvas[x][y] = '-';
                }
            }
        }

    }
    fill( visited.begin(), visited.end(), vector<bool>( garden[0].size(), false ) );
    // Down
    int downEdge = 0;
    for( int i = 0; i < boundary.size(); i++ ) {
        pos s = boundary[i];
        int x = boundary[i].first, y = boundary[i].second;
        if( !visited[x][y] ) {
            if( isDownBoundary( s, garden ) ) {
                // Draw horizontal line
                downEdge++;
                while( y - 1 >= 0 && garden[x][y - 1] == garden[x][y] && !visited[x][y - 1] && isDownBoundary( pos( x, y - 1 ), garden ) ) {
                    cout << "Draw down edge From " << s.first << " " << s.second << " to " << x << " " << y - 1 << "." << endl;
                    y--;
                }
                visited[x][y] = true;
                gardenCanvas[x][y] = '-';
                while( y + 1 < garden[x].size() && garden[x][y + 1] == garden[x][y] && !visited[x][y + 1] && isDownBoundary( pos( x, y + 1 ), garden ) ) {
                    visited[x][++y] = true;
                    gardenCanvas[x][y] = '-';
                }
            }
        }

    }
    fill( visited.begin(), visited.end(), vector<bool>( garden[0].size(), false ) );
    // Left
    int leftEdge = 0;
    for( int i = 0; i < boundary.size(); i++ ) {
        pos s = boundary[i];
        int x = boundary[i].first, y = boundary[i].second;
        if( !visited[x][y] ) {
            if( isLeftBoundary( s, garden ) ) {
                // Draw vertical line
                leftEdge++;
                while( x - 1 >= 0 && garden[x - 1][y] == garden[x][y] && !visited[x - 1][y] && isLeftBoundary( pos( x - 1, y ), garden ) ) {
                    cout << "Draw left edge From " << s.first << " " << s.second << " to " << x - 1 << " " << y << "." << endl;
                    x--;
                }
                gardenCanvas[x][y] = '|';
                visited[x][y] = true;
                while( x + 1 < garden.size() && garden[x + 1][y] == garden[x][y] && !visited[x + 1][y] && isLeftBoundary( pos( x + 1, y ), garden ) ) {
                    visited[++x][y] = true;
                    gardenCanvas[x][y] = '|';
                }
            }
        }

    }
    fill( visited.begin(), visited.end(), vector<bool>( garden[0].size(), false ) );
    // Right
    int rightEdge = 0;
    for( int i = 0; i < boundary.size(); i++ ) {
        pos s = boundary[i];
        int x = boundary[i].first, y = boundary[i].second;
        if( !visited[x][y] ) {
            if( isRightBoundary( s, garden ) ) {
                // Draw vertical line
                rightEdge++;
                while( x - 1 >= 0 && garden[x - 1][y] == garden[x][y] && !visited[x - 1][y] && isRightBoundary( pos( x - 1, y ), garden ) ) {
                    cout << "Draw right edge From " << s.first << " " << s.second << " to " << x - 1 << " " << y << "." << endl;
                    x--;
                }
                gardenCanvas[x][y] = '|';
                visited[x][y] = true;
                while( x + 1 < garden.size() && garden[x + 1][y] == garden[x][y] && !visited[x + 1][y] && isRightBoundary( pos( x + 1, y ), garden ) ) {
                    visited[++x][y] = true;
                    gardenCanvas[x][y] = '|';
                }
            }
        }

    }
    printGarden( gardenCanvas );
    return upedge + downEdge + leftEdge + rightEdge;
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
                vector<pos> boundary;
                int area = getBoundarys( pos( i, j ), garden, counted, boundary );
                res += area * getBoundaryEdges( boundary, garden );
            }
        }
    }
    cout << res << endl;
}