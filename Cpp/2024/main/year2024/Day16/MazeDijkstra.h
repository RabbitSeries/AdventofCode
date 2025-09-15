#include <array>
#include <fstream>
#include <functional>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "utils/ISolution.hpp"
class MazeDijkstra : public ISolution {
    REGISTER( MazeDijkstra )
    struct Step : public std::pair<int, int> {
        int direction, cost;
        std::vector<std::pair<int, int>> path;
        Step( int x, int y, int _direction, int _cost ) : std::pair<int, int>( x, y ), direction( _direction ), cost{ _cost } {}
        bool operator>( const Step& p ) const {
            return cost > p.cost;
        }
        bool isSameLocation( const std::pair<int, int>& p ) const {
            return this->first == p.first && this->second == p.second;
        }
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

    std::vector<std::vector<int>> maze;
    std::pair<int, int> s, e;

    bool isValid( Step const& p ) {
        return p.first >= 0 && p.first < rows && p.second >= 0 && p.second < cols && maze[p.first][p.second] == CELLEMPTY;
    };

    /**
     * @brief Dijkstra + state machine transfer, four dimensional dijkstra.
     *
     */
    int countSeats() {
        std::set<std::pair<int, int>> pathSeats;
        std::vector Cost = std::vector( rows, std::vector( cols, std::array{ INT_MAX, INT_MAX, INT_MAX, INT_MAX } ) );
        // std::priority_queue<Step, std::vector<Step>, std::greater<>> pq( std::from_range_t{}, std::vector{ Step( s.first, s.second, 0, 0 ) } );
        // Don't use this, or else, compiler may warn this:
        // array subscript 1 is outside array bounds of 'MazeDijkstra::Step [1]' [-Warray-bounds=]GCC
        // new_allocator.h(151, 73): at offset 40 into object of size 40 allocated by 'operator new'
        std::priority_queue<Step, std::vector<Step>, std::greater<>> pq;
        pq.emplace( s.first, s.second, 0, 0 );
        Cost[s.first][s.second] = std::array{ 0, 0, 0, 0 };
        int endCost = INT_MAX;
        while ( !pq.empty() ) {
            Step curStep = std::move( const_cast<Step&>( pq.top() ) );
            pq.pop();
            int curCost = curStep.cost, curDir = curStep.direction;
            curStep.path.emplace_back( curStep.first, curStep.second );
            if ( curStep.isSameLocation( e ) ) {
                if ( curCost <= endCost ) {
                    if ( curCost < endCost ) {  // Here enters only once.
                        printRes( 1, curCost );
                    }
                    endCost = curCost;
                    pathSeats.insert( curStep.path.begin(), curStep.path.end() );
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
                        pq.emplace( std::move( nextStep ) );
                    }
                }
            }
        }
        return pathSeats.size();
    }

    void readFile() {
        std::ifstream input( "Day16/input.txt" );
        std::istringstream ss( "Hello" );
        for ( std::string buf; getline( input, buf ); ) {  // operator bool()
            std::vector<int> row;
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
                maze.emplace_back( std::move( row ) );
        }
        rows = maze.size();
        cols = maze[0].size();
    }

   public:
    void Solution1() {
        readFile();
    }
    void Solution2() {
        printRes( 2, countSeats() );
    }
};