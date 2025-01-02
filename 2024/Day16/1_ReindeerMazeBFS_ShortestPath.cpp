#include "bits/stdc++.h"
using namespace std;

typedef enum Direction {
    EAST,
    SOUTH,
    WEST,
    NORTH
}Direction;
struct point2D : public pair<int, int> {
    point2D( int x, int y, Direction _direction ) : pair<int, int>( x, y ), direction( _direction ) {}
    Direction direction = EAST;
    string pointState() {
        return to_string( this->first ) + to_string( this->second ) + to_string( this->direction );
    }
};
vector<pair<int, int>> step{
    {0,1},
    {1,0},
    {0,-1},
    {-1,0}
};
#define WALL -2
#define EMPTY -1

int countSeats( vector<vector<int>> maze, point2D start, point2D end ) {
    vector<vector<int>> cost;
    unordered_set<int> pathSeats;
    unordered_set<string> pathState;
    priority_queue<pair<int, point2D>, vector<pair<int, point2D>>, greater<>> process;
    process.push( { 0,start } );
    pathState.insert( start.pointState() );
    while( !process.empty() ) {

    }
    return pathSeats.size();
}

int main() {
    return 0;
}