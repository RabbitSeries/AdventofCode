#include <algorithm>
#include <numeric>
#include <ranges>
#include <set>
#include <string>
#include <vector>

#include "utils/BufferedReader.hpp"
#include "utils/SolutionBase.hpp"
class MonkeyMarket : public SolutionBase {
    REGISTER( MonkeyMarket )

    typedef unsigned long long ull;

    ull getNextSecret( ull curSecret ) {
        // This algorithm is too random, there is no need to dp.
        curSecret = ( ( curSecret * 64 ) ^ curSecret ) % 16777216;
        curSecret = ( ( curSecret / 32 ) ^ curSecret ) % 16777216;
        curSecret = ( ( curSecret * 2048 ) ^ curSecret ) % 16777216;
        return curSecret;
    }

    void readFile() {
        for ( const std::string& buf : BufferedReader( "Day22/input.txt" ).lines().yield() ) {
            secrets.push_back( stoi( buf ) );
        }
    }

    ull getSecret( ull curSecret ) {
        for ( int i = 0; i < 2000; i++ ) {
            curSecret = getNextSecret( curSecret );
        }
        return curSecret;
    }

    int index( std::vector<int> const& v, ull lend ) {
        return accumulate( lend - 4 + v.begin(), v.begin() + lend, 0, []( int init, int e ) {
            return init * 19 + e;
        } );
    }
    std::vector<ull> secrets;

   public:
    void Solution1() {
        readFile();
        printRes( 1, accumulate( secrets.begin(), secrets.end(), 0ull, [this]( ull init, ull s ) {
                      return init + getSecret( s );
                  } ) );
        return;
    }

    void Solution2() {
        using namespace std;
        vector<ull> OfferAcc( 19 * 19 * 19 * 19 ), optimal( 19 * 19 * 19 * 19 );
        vector<int> window( 2001 );
        for ( ull curSecret : secrets ) {
            ranges::fill( optimal, 10 );
            for ( ull i = 1, nextSecret, curOffer; i <= 2000; i++ ) {
                nextSecret = getNextSecret( curSecret );
                curOffer = ( to_string( nextSecret ).back() - '0' );
                window[i] = to_string( nextSecret ).back() - to_string( curSecret ).back() + 9;
                curSecret = nextSecret;
                if ( i >= 4 ) {
                    int wId = index( window, i + 1 );
                    if ( optimal[wId] == 10 ) {  // Don't update if existed an offer. The monkey sells once seen the change sequence
                        optimal[wId] = curOffer;
                    }
                }
            }
            for ( ull i = 0; i < optimal.size(); i++ ) {
                OfferAcc[i] += ( optimal[i] == 10 ? 0 : optimal[i] );
            }
        }
        printRes( 2, *ranges::max_element( OfferAcc.begin(), OfferAcc.end() ) );
        return;
    }
};