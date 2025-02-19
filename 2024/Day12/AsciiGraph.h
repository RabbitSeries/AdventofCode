#include "bits/stdc++.h"
using namespace std;
class AsciiGraph {

    typedef pair<int, int> pos;
    typedef long long ll;
    int getFence( pos s, vector<vector<char>> map ) {
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

    pair<int, int> getBoundarys( pos s, vector<vector<char>> map, vector<vector<bool>>& counted ) {
        int x = s.first, y = s.second;
        counted[x][y] = true;
        int area = 1, perimeter = getFence( s, map );
        if( x - 1 >= 0 && !counted[x - 1][y] && map[x - 1][y] == map[x][y] ) {
            auto [_area, _perimeter] = getBoundarys( pos( x - 1, y ), map, counted );
            area += _area;
            perimeter += _perimeter;
        }
        // Down
        if( x + 1 < map.size() && !counted[x + 1][y] && map[x + 1][y] == map[x][y] ) {
            auto [_area, _perimeter] = getBoundarys( pos( x + 1, y ), map, counted );
            area += _area;
            perimeter += _perimeter;
        }
        // Right
        if( y + 1 < map[x].size() && !counted[x][y + 1] && map[x][y + 1] == map[x][y] ) {
            auto [_area, _perimeter] = getBoundarys( pos( x, y + 1 ), map, counted );
            area += _area;
            perimeter += _perimeter;
        }
        // Left
        if( y - 1 >= 0 && !counted[x][y - 1] && map[x][y - 1] == map[x][y] ) {
            auto [_area, _perimeter] = getBoundarys( pos( x, y - 1 ), map, counted );
            area += _area;
            perimeter += _perimeter;
        }
        return pair<int, int>( area, perimeter );
    }


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
                        // cout << "Draw up edge From " << s.first << " " << s.second << " to " << x << " " << y - 1 << "." << endl;
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
                        // cout << "Draw down edge From " << s.first << " " << s.second << " to " << x << " " << y - 1 << "." << endl;
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
                        // cout << "Draw left edge From " << s.first << " " << s.second << " to " << x - 1 << " " << y << "." << endl;
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
                        // cout << "Draw right edge From " << s.first << " " << s.second << " to " << x - 1 << " " << y << "." << endl;
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
        // printGarden( gardenCanvas );
        return upedge + downEdge + leftEdge + rightEdge;
    }
public:
    void Solution1() {
        vector<vector<char>> garden;
        ifstream input( "Day12/input.txt" );
        string buf;
        int lineCnt = 0;
        while( getline( input, buf ) ) {
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
                    auto [area, perimeter] = getBoundarys( pos( i, j ), garden, counted );
                    res += area * perimeter;
                }
            }
        }
        cout << "Solution 1: " << res << endl;
    }
    void Solution2() {
        vector<vector<char>> garden;
        ifstream input( "Day12/input.txt" );
        string buf;
        int lineCnt = 0;
        while( getline( input, buf ) ) {
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
        cout << "Solution2 : " << res << endl;
    }
};