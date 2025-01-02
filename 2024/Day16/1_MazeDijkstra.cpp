#include "bits/stdc++.h"
using namespace std;
// ! Penalty is 0, then this is a simple undirected map problem.
#define PENALTY 1000
// #define PENALTY 1
// #define PENALTY 0
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
    int cost = 0;
    string hashCode() {
        return to_string( this->first + 1000 ) + to_string( this->second + 1000 ) + to_string( this->direction + 1000 );
    }
    vector<pair<int, int>> LinkRoad;

};

// TODO Add this to point2D.
template <>
struct std::hash<point2D> {
    size_t operator()( const point2D& obj ) const {
        return hash<int>()( obj.first ) ^ ( hash<int>()( obj.second ) << 1 ) ^ ( hash<Direction>()( obj.direction ) << 2 );
    }
};

vector<pair<int, int>> step{
    {0,1},
    {1,0},
    {0,-1},
    {-1,0}
};

void printPathInMaze( vector<pair<int, int>> curPath, vector<vector<int>> m ) {
    // system( "clear" );
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

// Dijikstra + state machine transfer, four dimensionnal dijikstra.
int countSeats( vector<vector<int>> maze, point2D start, point2D end ) {
    int foo = 0;

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
    start.cost = 0;
    process.push( { 0,start } );
    pathState.emplace( start.hashCode(), 0 );
    int endCost = INT_MAX;
    size_t max_process = 0;
    while( !process.empty() ) {
        max_process = max( max_process, process.size() );
        auto [curCost, curPoint] = process.top();
        process.pop();
        Direction curDirection = curPoint.direction;
        curPoint.LinkRoad.push_back( curPoint );
        // ! To trace the process, uncomment the code as follows:
        // printPathInMaze( curPoint.LinkRoad, maze );
        // this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
        // ! This happens when turning cost 1 penalty, and the input is inputCutOptimize.txt.
        if( pathState.count( curPoint.hashCode() ) != 0 && curCost > pathState[curPoint.hashCode()] ) {
            foo++;
            continue;
        }
        // There are 1 different ways that can reach destination at same cost.
        // The mission in queue to process reach maximum 1585 in this maze problem.
        // The final lowest score is 422.
        // Pile these paths in one maze, there are 331 comfotable spots to enjoy the event.
        if( curPoint == end ) {
            if( curCost < endCost ) {
                // This enters one time.
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
            string nextState = nextPoint.hashCode();
            int nextCost = curCost + 1;
            if( nextDirection != curDirection ) {
                if( nextDirection == ( curDirection + 2 ) % 4 ) {
                    nextCost += PENALTY*2;
                }
                else {
                    nextCost += PENALTY;
                }
            }
            if( isValid( nextPoint ) ) {
                // Reachable in Dijkstra.
                if( pathState.count( nextState ) == 0 ) {
                    // Unvisited before.
                    pathState.emplace( nextState, nextCost );
                    nextPoint.LinkRoad = curPoint.LinkRoad;
                    process.push( { nextCost,nextPoint } );
                }
                else {
                    if( nextCost <= pathState[nextState] ) {
                        // Visited and can be relaxed or same path.
                        pathState[nextState] = nextCost; // This line doesn't affect the result.
                        nextPoint.LinkRoad = curPoint.LinkRoad;
                        process.push( { nextCost,nextPoint } );
                    }
                    // else {
                    //     // TODO this lien seems no use, as the cost does not need to get larger if it is already too large to relax.
                    //     pathState.emplace( point2D( curPoint.first, curPoint.second, nextDirection ).pointState(), nextCost - 1 );
                    // }
                }
            }
            else {
                pathState.emplace( point2D( curPoint.first, curPoint.second, nextDirection ).hashCode(), nextCost - 1 );
            }
        }
    }
    // ! To print the path, uncomment the following code.
    int curPathCnt = 0;
    for_each( pathRecord.begin(), pathRecord.end(), [ & ]( vector<pair<int, int>> curPath ) {
        cout << "Path " << curPathCnt++ << ":" << endl;
        printPathInMaze( curPath, maze );
        cout << endl;
        cout << endl;
    } );
    cout << "Optmized " << foo << " x 4 times." << endl;
    cout << "There are " << pathRecord.size() << " different ways that can reach destination at same cost." << endl;
    cout << "The mission in queue to process reach maximum " << max_process << " in this maze problem." << endl;
    cout << "The final lowest score is " << endCost << "." << endl;
    return pathSeats.size();
}

int main() {
    vector<vector<int>> m;
    pair<int, int> s, e;
    // ! To change test example, choose the following code to uncomment:
    FILE* input = fopen( "input_example1.txt", "r" );
    // FILE* input = fopen( "input_example2.txt", "r" );
    // FILE* input = fopen( "inputCutOptimize.txt", "r" );
    // FILE* input = fopen( "input.txt", "r" );
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
    cout << "Pile these paths in one maze, there are " << spots << " comfotable spots to enjoy the event." << endl;
    return 0;
}