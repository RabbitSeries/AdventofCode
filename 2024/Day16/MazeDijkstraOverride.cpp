#include "bits/stdc++.h"
using namespace std;
// ! If the Penalty is 0, then this is a simple undirected graph problem.
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
    inline bool operator==( const point2D& p ) const {
        return first == p.first && second == p.second && direction == p.direction;
    }
    // friend inline bool operator==( const point2D& p1, const point2D& p2 ) {
    //     return p1.first == p2.first && p1.second == p2.second && p1.direction == p2.direction;
    // }
    bool isSameLocation( const point2D& p ) {
        return this->first == p.first && this->second == p.second;
    }
    int cost = 0;
    vector<pair<int, int>> LinkRoad;

};

// TODO Add this to point2D.
template <>
struct std::hash<point2D> {
    // typedef size_t result_type;
    // typedef point2D argument_type;
    size_t operator()( const point2D& obj ) const {
        return hash<int>{}( obj.first ) ^ ( hash<int>{}( obj.second ) << 1 ) ^ ( hash<int>{}( static_cast<int>( obj.direction ) ) << 2 );
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
            } else if( cell == CELLEMPTY ) {
                row += ' ';
            } else {
                row += '0';
            }
        }
        cout << row << endl;
    }
}

void printAllPath( vector<vector<pair<int, int>>> const& pathRecord, vector<vector<int>> const& maze ) {
    int curPathCnt = 0;
    for_each( pathRecord.begin(), pathRecord.end(), [ & ]( vector<pair<int, int>> curPath ) {
        cout << "Path " << curPathCnt++ << ":" << endl;
        printPathInMaze( curPath, maze );
        cout << endl;
        cout << endl;
    } );
}


// Dijkstra + state machine transfer, four dimensional dijkstra.
int countSeats( vector<vector<int>> const& maze, point2D start, point2D const& end ) {
    int foo = 0;

    set<pair<int, int>> pathSeats;
    vector<vector<pair<int, int>>> pathRecord;
    unordered_map<point2D, int> pathCost;
    priority_queue<pair<int, point2D>, vector<pair<int, point2D>>, greater<>> pq;

    int rows = maze.size();
    int cols = maze[0].size();

    auto isValid = [ & ]( point2D p ) -> bool {
        return p.first >= 0 && p.first < rows && p.second >= 0 && p.second < cols && maze[p.first][p.second] == CELLEMPTY;
    };

    start.cost = 0;
    pq.push( { 0,start } );
    pathCost.emplace( start, 0 );
    int endCost = INT_MAX;
    size_t max_process = 0;

    bool found = false;
    while( !pq.empty() ) {
        max_process = max( max_process, pq.size() );
        auto [curCost, curPoint] = pq.top();
        if( found && curPoint != end ) {
            break;
        }
        pq.pop();
        Direction curDirection = curPoint.direction;
        curPoint.LinkRoad.push_back( curPoint );
        // ! To trace the pq, uncomment the code as follows:
        // printPathInMaze( curPoint.LinkRoad, maze );
        // this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
        // ! This happens when turning cost 1 penalty, and the input is inputCutOptimize.txt.
        if( pathCost.count( curPoint ) != 0 && curCost > pathCost[curPoint] ) {
            foo++;
            // Once the point is visited in this visit, it should be optimized therefore the following visit should be excluded except that this vertex is shared by different shortest path.
            continue;
        }


        if( curPoint.isSameLocation( end ) ) {
            found = true;
            if( curCost < endCost ) {
                // This enters only once.
                endCost = curCost;
                pathSeats.clear();
                pathRecord.clear();
                pathRecord.push_back( curPoint.LinkRoad );
                for_each( curPoint.LinkRoad.begin(), curPoint.LinkRoad.end(), [ & ]( pair<int, int> seat ) {pathSeats.insert( seat ); } );
            } else if( curCost == endCost ) {
                pathRecord.push_back( curPoint.LinkRoad );
                for_each( curPoint.LinkRoad.begin(), curPoint.LinkRoad.end(), [ & ]( pair<int, int> seat ) {pathSeats.insert( seat ); } );
            }
            continue;
        }

        for( int i = 0; i < 4; i++ ) {
            Direction nextDirection = Direction( ( curDirection + i ) % 4 );
            point2D nextPoint = point2D( curPoint.first + step[nextDirection].first, curPoint.second + step[nextDirection].second, nextDirection );
            int nextCost = curCost + 1;
            if( nextDirection != curDirection ) {
                if( nextDirection == ( curDirection + 2 ) % 4 ) {
                    continue;
                    // nextCost += PENALTY * 2;
                } else {
                    nextCost += PENALTY;
                }
            }
            if( isValid( nextPoint ) ) {
                // Reachable in Dijkstra.
                // if( pathCost.count( nextState ) == 0 ) {
                if( pathCost.count( nextPoint ) == 0 ) {
                    // Unvisited before.
                    pathCost.emplace( nextPoint, nextCost );
                    nextPoint.LinkRoad = curPoint.LinkRoad;
                    pq.push( { nextCost,nextPoint } );
                } else {
                    if( pathCost[nextPoint] != INT_MAX && nextCost <= pathCost[nextPoint] ) {
                        // Visited and can be relaxed or same path.
                        pathCost[nextPoint] = nextCost; // This line doesn't affect the result.
                        nextPoint.LinkRoad = curPoint.LinkRoad;
                        pq.push( { nextCost,nextPoint } );
                    }
                }
            }
        }
    }
    // ! To print the path, uncomment the following code.
    // printAllPath( pathRecord, maze );
    cout << "Optmized " << foo << " x 4 times." << endl;
    cout << "There are " << pathRecord.size() << " different ways that can reach destination at same cost." << endl;
    cout << "The mission in queue to process reach maximum " << max_process << " in this maze problem." << endl;
    cout << "The final lowest score is " << endCost << "." << endl;
    return pathSeats.size();
}

void readFile( vector<vector<int>>& m, pair<int, int>& s, pair<int, int>& e ) {
    // ! To change test example, choose the following code to uncomment:
    // FILE* input = fopen( "input_example1.txt", "r" );
    // FILE* input = fopen( "input_example2.txt", "r" );
    // FILE* input = fopen( "inputCutOptimize.txt", "r" );
    ifstream input( "input.txt" );
    string buf;
    while( getline( input, buf ) ) {
        vector<int> row;
        for( char c : buf ) {
            if( c != '\n' && c != '\0' ) {
                if( c == '#' ) {
                    row.push_back( CELLWALL );
                } else if( c == '.' ) {
                    row.push_back( CELLEMPTY );
                } else if( c == 'S' ) {
                    s = { m.size(), row.size() };
                    row.push_back( CELLEMPTY );
                } else if( c == 'E' ) {
                    e = { m.size(), row.size() };
                    row.push_back( CELLEMPTY );
                }
            }
        }
        m.push_back( row );
    }
}

int main() {
    auto now = chrono::high_resolution_clock::now();
    vector<vector<int>> m;
    pair<int, int> s, e;
    // readFile( m, s, e );
    ifstream input( "input.txt" );
    string buf;
    while( getline( input, buf ) ) {
        vector<int> row;
        for( char c : buf ) {
            if( c != '\n' && c != '\0' ) {
                if( c == '#' ) {
                    row.push_back( CELLWALL );
                } else if( c == '.' ) {
                    row.push_back( CELLEMPTY );
                } else if( c == 'S' ) {
                    s = { m.size(), row.size() };
                    row.push_back( CELLEMPTY );
                } else if( c == 'E' ) {
                    e = { m.size(), row.size() };
                    row.push_back( CELLEMPTY );
                }
            }
        }
        m.push_back( row );
    }
    int spots = countSeats( m, { s.first,s.second,EAST }, { e.first,e.second } );
    cout << "Pile these paths in one maze, there are " << spots << " comfotable spots to enjoy the event." << endl;
    auto end = chrono::high_resolution_clock::now();
    // Slightly quicker
    // Fastest at 45.699 ms
    cout << chrono::duration_cast<chrono::microseconds>( end - now ).count() / 1000.0 << " ms" << endl;
    return 0;
}