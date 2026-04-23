#include <algorithm>
#include <fstream>
#include <functional>
#include <map>
#include <ranges>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

#include "utils/ISolution.hpp"
class CoinOptimize : public ISolution {
    REGISTER( CoinOptimize )

    using ll = long long;
    const ll TOP_UP = 10000000000000;
    using point2D = std::pair<ll, ll>;
    struct problem {
        point2D a, b, prize;
    };

    ll solve( const problem& p ) {
        // Restriction: ta : button a attempt count, tb : button b
        // a.fitst*ta+b.first*tb = prize.first
        // a.second*ta+b.second*tb = prize.second
        // Target = 3*ta + tb;
        point2D a = p.a, b = p.b, prize = p.prize;
        ll ta = 0, tb = 0;
        bool ba = false;
        if ( ( prize.first * b.second - prize.second * b.first ) % ( a.first * b.second - a.second * b.first ) == 0 ) {
            ta = ( prize.first * b.second - prize.second * b.first ) / ( a.first * b.second - a.second * b.first );
            // if( ta <= MAX_ATTEMPT ) {
            ba = true;
            // }
        }
        if ( ba && ( prize.first * a.second - prize.second * a.first ) % ( a.second * b.first - a.first * b.second ) == 0 ) {
            tb = ( prize.first * a.second - prize.second * a.first ) / ( a.second * b.first - a.first * b.second );
            // if( tb <= MAX_ATTEMPT ) {
            return 3 * ta + tb;
            // }
        }
        return -1;
    }

    void createProblem() {
        using namespace std;
        ifstream input( "Day13/input.txt" );
        regex re( R"([\+=]([0-9]+).*[\+=]([0-9]+))" );
        smatch m;
        ll inProblem = 0;
        for ( string buf; getline( input, buf ); ) {
            if ( std::regex_search( buf, m, re ) ) {
                if ( !inProblem ) {
                    problemSet.push_back( problem() );
                    problemSet.back().a = pair<ll, ll>( stoi( m[1] ), stoi( m[2] ) );
                    inProblem++;
                } else {
                    if ( inProblem == 1 ) {
                        problemSet.back().b = pair<ll, ll>( stoi( m[1] ), stoi( m[2] ) );
                        inProblem++;
                    } else {
                        problemSet.back().prize = pair<ll, ll>( stoi( m[1] ), stoi( m[2] ) );
                        inProblem = 0;
                    }
                }
            }
        }
    }

    std::vector<problem> problemSet;

    ll solveAll() {
        return std::ranges::fold_left( problemSet |
                                           std::views::transform( [this]( problem& p ) {
                                               ll res = solve( p );
                                               return res >= 0 ? res : 0ll;
                                           } ),
                                       0ll, std::plus<>{} );
    }

   public:
    void Solution1() {
        createProblem();
        printRes( 1, solveAll() );
    }

    void Solution2() {
        for ( auto& p : problemSet ) {
            std::tie( p.prize.first, p.prize.second ) = std::make_pair( p.prize.first + TOP_UP, p.prize.second + TOP_UP );
        }
        printRes( 2, solveAll() );
    }
};
