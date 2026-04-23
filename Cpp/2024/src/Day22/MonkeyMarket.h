#include <algorithm>
#include <numeric>
#include <ranges>
#include <set>
#include <string>
#include <vector>

#include "utils/BufferedReader.hpp"
#include "utils/ISolution.hpp"
class MonkeyMarket : public ISolution {
    REGISTER( MonkeyMarket )

    using ull = unsigned long long;

    static inline ull getNextSecret( ull curSecret ) {
        // This algorithm is too random, there is no need to dp.
        curSecret = ( ( curSecret * 64 ) ^ curSecret ) % 16777216;
        curSecret = ( ( curSecret / 32 ) ^ curSecret ) % 16777216;
        curSecret = ( ( curSecret * 2048 ) ^ curSecret ) % 16777216;
        return curSecret;
    }

    void readFile() {
        for ( const std::string& buf : BufferedReader( "Day22/input.txt" ).lines().yield() ) {
            secrets.push_back( std::stoi( buf ) );
        }
    }

    static inline ull getSecret( ull curSecret ) {
        for ( int i = 0; i < 2000; i++ ) {
            curSecret = getNextSecret( curSecret );
        }
        return curSecret;
    }

    int hasher( std::vector<int> const& v, ull lend ) {
        return std::accumulate( v.begin() + ( lend - 4 ), v.begin() + lend, 0, []( int init, int e ) {
            return init * 19 + e;
        } );
    }
    std::vector<ull> secrets;

   public:
    void Solution1() {
        readFile();
        printRes( 1, std::ranges::fold_left( secrets | std::views::transform( &MonkeyMarket::getSecret ), 0ull, std::plus<>{} ) );
        return;
    }

    void Solution2() {
        using namespace std;
        unordered_map<ull, ull> OfferAcc;
        vector<int> window( 2001 );
        for ( ull curSecret : secrets ) {
            unordered_map<ull, ull> optimal;
            for ( int i : views::iota( 1, 2001 ) ) {
                ull nextSecret = getNextSecret( curSecret );
                ull curOffer = ( std::to_string( nextSecret ).back() - '0' );
                window[i] = std::to_string( nextSecret ).back() - std::to_string( curSecret ).back() + 9;
                curSecret = nextSecret;
                if ( i >= 4 ) {
                    int wId = hasher( window, i + 1 );
                    if ( !optimal.contains( wId ) ) {  // Don't update if existed an offer. The monkey sells once seen the change sequence
                        optimal[wId] = curOffer;
                    }
                }
            }
            for ( auto const& [wId, offer] : optimal ) {
                OfferAcc[wId] += offer;
            }
        }
        printRes( 2, ranges::max( OfferAcc | std::views::values ) );
        return;
    }
};