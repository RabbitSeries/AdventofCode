#include "bits/stdc++.h"

using namespace std;
#include <utils/SolutionBase.hpp>
class CoinOptimize : public SolutionBase {
	static inline Register<CoinOptimize> _reg{};

    typedef long long ll;
    const ll TOP_UP = 10000000000000;
    typedef pair<ll, ll> point2D;
    typedef struct problem {
        point2D a, b, prize;
    } problem;

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
        ifstream input( "Day13/input.txt" );
        regex re( R"([\+=]([0-9]+).*[\+=]([0-9]+))" );
        smatch m;
        ll inProblem = 0;
        for ( string buf; getline( input, buf ); ) {
            std::regex_search( buf, m, re );
            if ( m.size() == 3 ) {
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

    vector<problem> problemSet;

   public:
    void Solution1() {
        createProblem();
        ll tokenCnt = 0;
        for_each( problemSet.begin(), problemSet.end(), [&]( const problem& p ) {
            ll res = solve( p );
            if ( res >= 0 ) {
                tokenCnt += res;
            }
        } );
        printRes( 1, tokenCnt );
    }

    void Solution2() {
        ll tokenCnt = 0;
        for_each( problemSet.begin(), problemSet.end(), [&]( problem& p ) {
            tie( p.prize.first, p.prize.second ) = make_pair( p.prize.first + TOP_UP, p.prize.second + TOP_UP );
            ll res = solve( p );
            if ( res >= 0 ) {
                tokenCnt += res;
            }
        } );
        printRes( 2, tokenCnt );
    }
};
