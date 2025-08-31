#include <algorithm>
#include <fstream>
#include <map>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

#include "utils/ISolution.hpp"
class RobotPatrol : public ISolution {
    REGISTER( RobotPatrol )

    struct robot {
        robot() {};
        robot( int leftDis, int topDis, int horizenVel, int verticalVel ) : curPos( leftDis, topDis ),
                                                                            vel( horizenVel, verticalVel ) {};
        std::pair<int, int> curPos{};
        std::pair<int, int> vel{};
    };
    const int WIDTH = 101, HEIGHT = 103;
    using ull = unsigned long long;
    ull quadrantCount( std::vector<robot> const& s ) {
        // vector<vector<bool>> stacked( HEIGHT, vector<bool>( WIDTH, false ) );
        int wmiddle = WIDTH / 2, hmiddle = HEIGHT / 2;
        ull q1 = 0, q2 = 0, q3 = 0, q4 = 0;
        for ( auto const& rbt : s ) {
            int i = rbt.curPos.second;
            int j = rbt.curPos.first;
            if ( i < hmiddle && j < wmiddle ) {
                q1++;
            } else if ( i > hmiddle && j < wmiddle ) {
                q2++;
            } else if ( i > hmiddle && j > wmiddle ) {
                q3++;
            } else if ( i < hmiddle && j > wmiddle ) {
                q4++;
            }
        }
        return q1 * q2 * q3 * q4;
    }

    std::vector<robot> step( std::vector<robot> s, int cnt ) {
        for ( auto& rbt : s ) {
            rbt.curPos.first = ( rbt.curPos.first + ( WIDTH + rbt.vel.first ) * cnt ) % WIDTH;
            rbt.curPos.second = ( rbt.curPos.second + ( HEIGHT + rbt.vel.second ) * cnt ) % HEIGHT;
        }
        return s;
    }
    std::vector<robot> specs;

    void readFile() {
        std::ifstream input( "Day14/input.txt" );
        std::regex re( "p=([0-9]+),([0-9]+).*v=(-?[0-9]+),(-?[0-9]+)" );
        std::smatch m;
        for ( std::string buf; getline( input, buf ); ) {
            regex_search( buf, m, re );
            if ( m.size() == 5 ) {
                specs.push_back( robot( stoi( m[1] ), stoi( m[2] ), stoi( m[3] ), stoi( m[4] ) ) );
            }
        }
    }
    template <typename T>
    T getter( std::pair<T, T> const& p, bool getFirst ) {
        return getFirst ? p.first : p.second;
    }
    int centerVariance( int stepCnt, std::vector<robot> const& specs, bool isFirst, int SCALE ) {
        int var = 0;
        for ( const auto& rbt : specs ) {
            var += abs( ( getter( rbt.curPos, isFirst ) + ( SCALE + getter( rbt.vel, isFirst ) ) * stepCnt ) % SCALE - ( SCALE - 1 ) / 2 );
        }
        return var;
    }
    int extendGCD( int a, int b, int& x, int& y ) {
        if ( !b ) {
            x = 1, y = 0;
            return a;
        }
        int x1, y1;
        int gcd = extendGCD( b, a % b, x1, y1 );
        x = y1;
        y = x1 - a / b * y1;
        return gcd;
    }
    int reverseMod( int a, int b ) {
        int x = 0, y;
        int res = extendGCD( a, b, x, y );
        if ( res != 1 ) {
            std::cerr << "Error reverserMod operands\n";
            return -1;
        }
        // assert( extendGCD( a, b, x, y ) == 1 );// release mode will skip assert
        return x;
    }
    int positiveMod( int a, int b ) {
        return ( b + ( a % b ) ) % b;
    }

   public:
    void Solution1() {
        readFile();
        printRes( 1, quadrantCount( step( specs, 100 ) ) );
    }
    void Solution2() {
        int minDxX = *std::ranges::min_element( std::views::iota( 0, WIDTH ),
                                                {},
                                                bind( mem_fn( &RobotPatrol::centerVariance ), *this, std::placeholders::_1, cref( specs ), true, WIDTH ) );
        int minDyY = *std::ranges::min_element( std::views::iota( 0, HEIGHT ),
                                                {},
                                                bind( &RobotPatrol::centerVariance, this, std::placeholders::_1, cref( specs ), false, HEIGHT ) );
        printRes( 2, positiveMod( reverseMod( WIDTH, HEIGHT ) * ( minDyY - minDxX ), HEIGHT ) * WIDTH + minDxX );
    }
};
