#include "bits/stdc++.h"
using namespace std;
#include <utils/SolutionBase.hpp>
class AsciiGraph : public SolutionBase {
    const int dx[4]{ -1, 1, 0, 0 };
    const int dy[4]{ 0, 0, 1, -1 };

    bool isValid( int x, int y, vector<vector<char>> const& map ) {
        // return x >= 0 && (size_t)x < map.size() && y >= 0 && (size_t)y <= map[0].size();// Address 0x4e3f8cc is 0 bytes after a block of size 140 alloc'd
        return x >= 0 && (size_t)x < map.size() && y >= 0 && (size_t)y < map[0].size();  // Address 0x4e3f8cc is 0 bytes after a block of size 140 alloc'd
    }

    typedef pair<int, int> pos;
    typedef long long ll;
    int getFence( pos const& s, vector<vector<char>> const& map ) {
        int x = s.first, y = s.second;
        int fence = 0;
        for ( int i = 0; i < 4; i++ ) {
            int nextX = x + dx[i], nextY = y + dy[i];
            if ( !( isValid( nextX, nextY, map ) && map[nextX][nextY] == map[x][y] ) ) fence++;
        }
        return fence;
    }

    pair<int, int> getBoundarys( pos s, vector<vector<char>> const& map, vector<vector<bool>>& counted ) {
        int x = s.first, y = s.second;
        counted[x][y] = true;
        int area = 1, perimeter = getFence( s, map );
        for ( int i = 0; i < 4; i++ ) {
            int nextX = x + dx[i], nextY = y + dy[i];
            if ( isValid( nextX, nextY, map ) && !counted[nextX][nextY] && map[nextX][nextY] == map[x][y] ) {
                auto [_area, _perimeter] = getBoundarys( pos( nextX, nextY ), map, counted );
                area += _area;
                perimeter += _perimeter;
            }
        }
        return { area, perimeter };
    }
    bool isBoundary( pos const& s, vector<vector<char>> const& map ) {
        int x = s.first, y = s.second;
        for ( int i = 0; i < 4; i++ ) {
            int nextX = x + dx[i], nextY = y + dy[i];
            if ( !isValid( nextX, nextY, map ) || map[nextX][nextY] != map[x][y] ) {
                return true;
            }
        }
        return false;
    }
    bool isUpBoundary( const pos& s, vector<vector<char>> const& garden ) {
        int x = s.first, y = s.second;
        if ( ( x - 1 >= 0 && garden[x - 1][y] != garden[x][y] ) || x - 1 < 0 ) {
            return true;
        }
        return false;
    }
    bool isDownBoundary( const pos& s, vector<vector<char>> const& garden ) {
        size_t x = s.first, y = s.second;
        if ( ( x + 1 < garden.size() && garden[x + 1][y] != garden[x][y] ) || (size_t)( x + 1 ) >= garden.size() ) {
            return true;
        }
        return false;
    }
    bool isRightBoundary( const pos& s, vector<vector<char>> const& garden ) {
        size_t x = s.first, y = s.second;
        if ( ( y + 1 < garden[x].size() && garden[x][y + 1] != garden[x][y] ) || y + 1 >= garden[x].size() ) {
            return true;
        }
        return false;
    }
    bool isLeftBoundary( const pos& s, vector<vector<char>> const& garden ) {
        int x = s.first, y = s.second;
        if ( ( y >= 1 && garden[x][y - 1] != garden[x][y] ) || y < 1 ) {
            return true;
        }
        return false;
    }

    int getBoundarys( pos s, vector<vector<char>> const& garden, vector<vector<bool>>& visited, vector<pos>& boundary ) {
        int x = s.first, y = s.second;
        visited[x][y] = true;
        int area = 1;
        if ( isBoundary( s, garden ) ) {
            boundary.push_back( pos( x, y ) );
        }
        for ( int i = 0; i < 4; i++ ) {
            int nextX = x + dx[i], nextY = y + dy[i];
            if ( isValid( nextX, nextY, garden ) && !visited[nextX][nextY] && garden[nextX][nextY] == garden[x][y] ) {
                area += getBoundarys( pos( nextX, nextY ), garden, visited, boundary );
            }
        }
        return area;
    }

    void printGarden( vector<vector<char>> const& garden ) {
        for ( size_t i = 0; i < garden.size(); i++ ) {
            for ( size_t j = 0; j < garden.size(); j++ ) {
                cout << garden[i][j];
            }
            cout << endl;
        }
    }

    int getBoundaryEdges( vector<pos> const& boundary, vector<vector<char>> const& garden ) {
        vector<vector<bool>> visited( garden.size(), vector<bool>( garden[0].size(), false ) );
        // TODO print the boundary with direction out.
        vector<vector<char>> gardenCanvas = garden;
        // Up
        int upedge = 0;
        for ( size_t i = 0; i < boundary.size(); i++ ) {
            pos s = boundary[i];
            int x = s.first, y = s.second;
            if ( !visited[x][y] ) {
                if ( isUpBoundary( s, garden ) ) {
                    // Has up fence
                    // Draw horizontal line
                    upedge++;
                    while ( y - 1 >= 0 && garden[x][y - 1] == garden[x][y] && !visited[x][y - 1] && isUpBoundary( pos( x, y - 1 ), garden ) ) {
                        // cout << "Draw up edge From " << s.first << " " << s.second << " to " << x << " " << y - 1 << "." << endl;
                        y--;
                    }
                    visited[x][y] = true;
                    gardenCanvas[x][y] = '-';
                    while ( (size_t)( y + 1 ) < garden[x].size() && garden[x][y + 1] == garden[x][y] && !visited[x][y + 1] && isUpBoundary( pos( x, y + 1 ), garden ) ) {
                        visited[x][++y] = true;
                        gardenCanvas[x][y] = '-';
                    }
                }
            }
        }
        fill( visited.begin(), visited.end(), vector<bool>( garden[0].size(), false ) );
        // Down
        int downEdge = 0;
        for ( size_t i = 0; i < boundary.size(); i++ ) {
            pos s = boundary[i];
            int x = boundary[i].first, y = boundary[i].second;
            if ( !visited[x][y] ) {
                if ( isDownBoundary( s, garden ) ) {
                    // Draw horizontal line
                    downEdge++;
                    while ( y - 1 >= 0 && garden[x][y - 1] == garden[x][y] && !visited[x][y - 1] && isDownBoundary( pos( x, y - 1 ), garden ) ) {
                        // cout << "Draw down edge From " << s.first << " " << s.second << " to " << x << " " << y - 1 << "." << endl;
                        y--;
                    }
                    visited[x][y] = true;
                    gardenCanvas[x][y] = '-';
                    while ( (size_t)( y + 1 ) < garden[x].size() && garden[x][y + 1] == garden[x][y] && !visited[x][y + 1] && isDownBoundary( pos( x, y + 1 ), garden ) ) {
                        visited[x][++y] = true;
                        gardenCanvas[x][y] = '-';
                    }
                }
            }
        }
        fill( visited.begin(), visited.end(), vector<bool>( garden[0].size(), false ) );
        // Left
        int leftEdge = 0;
        for ( size_t i = 0; i < boundary.size(); i++ ) {
            pos s = boundary[i];
            int x = boundary[i].first, y = boundary[i].second;
            if ( !visited[x][y] ) {
                if ( isLeftBoundary( s, garden ) ) {
                    // Draw vertical line
                    leftEdge++;
                    while ( x - 1 >= 0 && garden[x - 1][y] == garden[x][y] && !visited[x - 1][y] && isLeftBoundary( pos( x - 1, y ), garden ) ) {
                        // cout << "Draw left edge From " << s.first << " " << s.second << " to " << x - 1 << " " << y << "." << endl;
                        x--;
                    }
                    gardenCanvas[x][y] = '|';
                    visited[x][y] = true;
                    while ( (size_t)( x + 1 ) < garden.size() && garden[x + 1][y] == garden[x][y] && !visited[x + 1][y] && isLeftBoundary( pos( x + 1, y ), garden ) ) {
                        visited[++x][y] = true;
                        gardenCanvas[x][y] = '|';
                    }
                }
            }
        }
        fill( visited.begin(), visited.end(), vector<bool>( garden[0].size(), false ) );
        // Right
        int rightEdge = 0;
        for ( size_t i = 0; i < boundary.size(); i++ ) {
            pos s = boundary[i];
            int x = boundary[i].first, y = boundary[i].second;
            if ( !visited[x][y] ) {
                if ( isRightBoundary( s, garden ) ) {
                    // Draw vertical line
                    rightEdge++;
                    while ( x - 1 >= 0 && garden[x - 1][y] == garden[x][y] && !visited[x - 1][y] && isRightBoundary( pos( x - 1, y ), garden ) ) {
                        // cout << "Draw right edge From " << s.first << " " << s.second << " to " << x - 1 << " " << y << "." << endl;
                        x--;
                    }
                    gardenCanvas[x][y] = '|';
                    visited[x][y] = true;
                    while ( (size_t)( x + 1 ) < garden.size() && garden[x + 1][y] == garden[x][y] && !visited[x + 1][y] && isRightBoundary( pos( x + 1, y ), garden ) ) {
                        visited[++x][y] = true;
                        gardenCanvas[x][y] = '|';
                    }
                }
            }
        }
        // printGarden( gardenCanvas );
        return upedge + downEdge + leftEdge + rightEdge;
    }
    vector<vector<char>> garden;
    void readFile() {
        ifstream input( "Day12/input.txt" );
        for ( string buf; getline( input, buf ) && !buf.empty(); ) {
            vector<char> row;
            for ( char c : buf ) {
                row.push_back( c );
            }
            garden.emplace_back( std::move( row ) );
        }
    }

   public:
    void Solution1() {
        readFile();
        ll res = 0;
        vector<vector<bool>> counted( garden.size(), vector<bool>( garden[0].size(), false ) );
        for ( size_t i = 0; i < garden.size(); i++ ) {
            // cout << "Processing line " << i << "." << endl;
            for ( size_t j = 0; j < garden[0].size(); j++ ) {
                if ( !counted[i][j] ) {
                    auto [area, perimeter] = getBoundarys( pos( i, j ), garden, counted );
                    res += area * perimeter;
                }
            }
        }
        printRes( 1, res );
    }
    void Solution2() {
        ll res = 0;
        vector<vector<bool>> counted( garden.size(), vector<bool>( garden[0].size(), false ) );
        for ( size_t i = 0; i < garden.size(); i++ ) {
            // cout << "Processing line " << i << "." << endl;
            for ( size_t j = 0; j < garden[0].size(); j++ ) {
                if ( !counted[i][j] ) {
                    vector<pos> boundary{};
                    int area = getBoundarys( pos( i, j ), garden, counted, boundary );
                    res += area * getBoundaryEdges( boundary, garden );
                }
            }
        }
        printRes( 2, res );
    }
};