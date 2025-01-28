#include <iostream>
#include <unordered_map>
#include <utility> // For std::pair
using namespace std;

enum class Direction {
    North,
    South,
    East,
    West
};

struct point2D : public std::pair<int, int> {
    point2D( int x, int y ) : std::pair<int, int>( x, y ) {}
    point2D( int x, int y, Direction _direction ) : std::pair<int, int>( x, y ), direction( _direction ) {}

    Direction direction;
    // Equality operator for point2D
    bool operator==( const point2D& lhs ) const {
        return lhs.first == this->first && lhs.second == this->second && lhs.direction == this->direction;
    }
};


// Specialize std::hash for point2D
template <>
struct std::hash<point2D> {
    std::size_t operator()( const point2D& p ) const {
        // Combine hash values of x, y, and direction
        std::size_t h1 = std::hash<int>{}( p.first );
        std::size_t h2 = std::hash<int>{}( p.second );
        std::size_t h3 = std::hash<int>{}( static_cast<int>( p.direction ) ); // Cast enum to int for hashing
        return h1 ^ ( h2 << 1 ) ^ ( h3 << 2 ); // Combine hashes
    }
};

int main() {
    // Define an unordered_map with point2D as the key
    std::unordered_map<point2D, std::string> pointMap;

    // Insert elements into the map
    pointMap[{1, 2, Direction::North}] = "North Point";
    pointMap[{3, 4, Direction::South}] = "South Point";

    // Access elements
    cout << ( point2D( { 1, 2, Direction::North } ) == point2D( { 1, 21, Direction::North } ) ) << endl;

    cout << pointMap.count( point2D( { 1, 2, Direction::North } ) ) << endl;
    std::cout << "Value at (1, 2, North): " << pointMap[{1, 2, Direction::North}] << std::endl;
    std::cout << "Value at (3, 4, South): " << pointMap[{3, 4, Direction::South}] << std::endl;

    return 0;
}