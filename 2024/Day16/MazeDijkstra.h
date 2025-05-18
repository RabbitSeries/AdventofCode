#include "bits/stdc++.h"
using namespace std;
#include "../../utils/SolutionBase.h"
class MazeDijkstra : public SolutionBase {
    struct Step : public pair<int, int> {
        Step( int x, int y, int _direction, int _cost ) : pair<int, int>( x, y ), direction( _direction ), cost{ _cost } {}
        bool operator>( const Step& p ) const {
            return cost > p.cost;
        }
        bool isSameLocation( const pair<int, int>& p ) const {
            return this->first == p.first && this->second == p.second;
        }
        int direction;
        int cost;
        vector<pair<int, int>> path{};
    };

    enum constants {
        PENALTY = 1000,  // If the Penalty is 0, then this is a simple undirected graph problem.
        CELLWALL = -2,
        CELLEMPTY = -1
    };

    const int dx[4]{ 0, 1, 0, -1 };
    const int dy[4]{ 1, 0, -1, 0 };
    int rows = 0;
    int cols = 0;

    vector<vector<int>> maze;
    pair<int, int> s, e;

    bool isValid( Step const& p ) {
        return p.first >= 0 && p.first < rows && p.second >= 0 && p.second < cols && maze[p.first][p.second] == CELLEMPTY;
    };

    /**
     * @brief Dijkstra + state machine transfer, four dimensional dijkstra.
     *
     */
    int countSeats( bool isSolution1 = true ) {
        set<pair<int, int>> pathSeats;
        vector<vector<array<int, 4>>> Cost = vector( rows, vector( cols, array{ INT_MAX, INT_MAX, INT_MAX, INT_MAX } ) );
        priority_queue<Step, vector<Step>, greater<>> pq;

        pq.emplace( s.first, s.second, 0, 0 );
        Cost[s.first][s.second] = array{ 0, 0, 0, 0 };

        int endCost = INT_MAX;
        while ( !pq.empty() ) {
            Step curStep = move( const_cast<Step&>( pq.top() ) );
            pq.pop();
            int curCost = curStep.cost, curDir = curStep.direction;
            curStep.path.emplace_back( curStep.first, curStep.second );
            if ( curStep.isSameLocation( e ) ) {
                if ( curCost <= endCost ) {
                    if ( isSolution1 ) {  // Here enters only once.
                        return curCost;
                    }
                    endCost = curCost;
                    for_each( curStep.path.begin(), curStep.path.end(), [&]( pair<int, int> const& seat ) {
                        pathSeats.emplace( seat.first, seat.second );
                    } );
                } else {
                    break;
                }
                continue;
            }
            if ( curCost > Cost[curStep.first][curStep.second][curDir] ) {
                continue;
            }
            for ( int i = 0; i < 4; i++ ) {
                int nDir = ( curDir + i ) % 4, nextCost = curCost + 1;
                if ( nDir != curDir ) {
                    if ( nDir == ( curDir + 2 ) % 4 ) {
                        continue;
                    } else {
                        nextCost += PENALTY;
                    }
                }
                Step nextStep( curStep.first + dx[nDir], curStep.second + dy[nDir], nDir, nextCost );
                if ( isValid( nextStep ) ) {
                    if ( nextCost <= Cost[nextStep.first][nextStep.second][nDir] ) {
                        Cost[nextStep.first][nextStep.second][nextStep.direction] = nextCost;
                        nextStep.path = curStep.path;
                        pq.emplace( move( nextStep ) );
                    }
                }
            }
        }
        return pathSeats.size();
    }

    void readFile() {
        ifstream input( "Day16/input.txt" );
        istringstream ss( "Hello" );
        for ( string buf; getline( input, buf ); ) {  // operator bool()
            vector<int> row;
            for ( char c : buf ) {
                if ( c == '#' ) {
                    row.push_back( CELLWALL );
                } else if ( c == '.' ) {
                    row.push_back( CELLEMPTY );
                } else if ( c == 'S' ) {
                    s = { maze.size(), row.size() };
                    row.push_back( CELLEMPTY );
                } else if ( c == 'E' ) {
                    e = { maze.size(), row.size() };
                    row.push_back( CELLEMPTY );
                }
            }
            if ( !row.empty() )
                maze.emplace_back( move( row ) );
        }
        rows = maze.size();
        cols = maze[0].size();
    }

   public:
    void Solution1() {
        readFile();
        printRes( 1, countSeats() );
    }
    void Solution2() {
        printRes( 2, countSeats( false ) );
    }
};