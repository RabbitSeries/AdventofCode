#include "bits/stdc++.h"
using namespace std;

enum { CELLWALL = -2, CELLEMPTY = -1 };
enum Direction {
    EAST,
    SOUTH,
    WEST,
    NORTH
};
struct point2D : public pair<int, int> {
    point2D( int x, int y ) : pair<int, int>( x, y ) {}
    point2D( int x, int y, Direction _direction ) : pair<int, int>( x, y ), direction( _direction ) {}
    Direction direction;
    string pointState() {
        return to_string( this->first + 1000 ) + to_string( this->second + 1000 ) + to_string( this->direction + 1000 );
    }
    vector<pair<int, int>> LinkRoad;
};
vector<pair<int, int>> step{
    {0,1},
    {1,0},
    {0,-1},
    {-1,0}
};

void printPathInMaze( vector<pair<int, int>> curPath, vector<vector<int>> m ) {
    // system( "clear" );
    cout << endl;
    cout << endl;
    // m[e.first][e.second] = 'E';
    for_each( curPath.begin(), curPath.end(), [ & ]( pair<int, int> road ) {m[road.first][road.second] = 0; } );
    for( auto line : m ) {
        string row = "";
        for( auto cell : line ) {
            if( cell == CELLWALL ) {
                row += '#';
            }
            else if( cell == CELLEMPTY ) {
                row += ' ';
            }
            else {
                row += '0';
            }
        }
        cout << row << endl;
    }
}


int countSeats( vector<vector<int>> maze, point2D start, point2D end ) {
    vector<vector<int>> cost;
    set<pair<int, int>> pathSeats;
    vector<vector<pair<int, int>>> pathRecord;
    unordered_map<string, int> pathState;
    priority_queue<pair<int, point2D>, vector<pair<int, point2D>>, greater<>> process;

    int rows = maze.size();
    int cols = maze[0].size();

    auto isValid = [ & ]( point2D p ) {
        return p.first >= 0 && p.first < rows && p.second >= 0 && p.second < cols && maze[p.first][p.second] == CELLEMPTY;
    };


    process.push( { 0,start } );
    pathState.emplace( start.pointState(), 0 );
    int endCost = INT_MAX;
    size_t max_process = 0;
    while( !process.empty() ) {
        max_process = max( max_process, process.size() );
        auto [curCost, curPoint] = process.top();
        process.pop();
        Direction curDirection = curPoint.direction;
        curPoint.LinkRoad.push_back( curPoint );
        // printPathInMaze( curPoint.LinkRoad, maze );
        // this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
        if( curPoint == end ) {
            if( curCost < endCost ) {
                endCost = curCost;
                pathSeats.clear();
                pathRecord.clear();
                pathRecord.push_back( curPoint.LinkRoad );
                for_each( curPoint.LinkRoad.begin(), curPoint.LinkRoad.end(), [ & ]( pair<int, int> seat ) {pathSeats.insert( seat ); } );
            }
            else if( curCost == endCost ) {
                pathRecord.push_back( curPoint.LinkRoad );
                for_each( curPoint.LinkRoad.begin(), curPoint.LinkRoad.end(), [ & ]( pair<int, int> seat ) {pathSeats.insert( seat ); } );
            }
            continue;
        }
        for( int i = 0; i < 4; i++ ) {
            Direction nextDirection = Direction( ( curDirection + i ) % 4 );
            point2D nextPoint = point2D( curPoint.first + step[nextDirection].first, curPoint.second + step[nextDirection].second, nextDirection );
            string nextState = nextPoint.pointState();
            int nextCost = curCost + 1;
            if( nextDirection != curDirection ) {
                if( nextDirection == ( curDirection + 2 ) % 4 ) {
                    nextCost += 2000;
                }
                else {
                    nextCost += 1000;
                }
            }
            if( isValid( nextPoint ) ) {
                if( pathState.count( nextState ) == 0 ) {
                    pathState.emplace( nextState, nextCost );
                    nextPoint.LinkRoad = curPoint.LinkRoad;
                    process.push( { nextCost,nextPoint } );
                }
                else {
                    if( nextCost <= pathState[nextState] ) {
                        nextPoint.LinkRoad = curPoint.LinkRoad;
                        process.push( { nextCost,nextPoint } );
                    }
                    else {
                        pathState.emplace( point2D( curPoint.first, curPoint.second, nextDirection ).pointState(), nextCost - 1 );
                    }
                }
            }
            else {
                pathState.emplace( point2D( curPoint.first, curPoint.second, nextDirection ).pointState(), nextCost - 1 );
            }
        }
    }
    // int curPathCnt = 0;
    // for_each( pathRecord.begin(), pathRecord.end(), [ & ]( vector<pair<int, int>> curPath ) {
    //     cout << "Path " << curPathCnt++ << ":" << endl;
    //     printPathInMaze( curPath, maze );
    // } );
    cout << "There are " << pathRecord.size() << " different ways that can reach des at same cost." << endl;
    cout << "The mission in queue to process reach " << max_process << " in this maze problem." << endl;
    return pathSeats.size();
}

int main() {
    vector<vector<int>> m;
    pair<int, int> s, e;
    // FILE* input = fopen( "input_example1.txt", "r" );
    // FILE* input = fopen( "input_example2.txt", "r" );
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
                    s = { m.size(), row.size() };
                    row.push_back( CELLEMPTY );
                }
                else if( c == 'E' ) {
                    e = { m.size(), row.size() };
                    row.push_back( CELLEMPTY );
                }
            }
        }
        m.push_back( row );
    }
    int spots = countSeats( m, { s.first,s.second,EAST }, { e.first,e.second } );
    cout << "Pile these paths in one maze, there are "<< spots << " comfotable spots to enjoy the event." << endl;
    return 0;
}