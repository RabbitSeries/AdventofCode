#include <algorithm>
#include <fstream>
#include <map>
#include <ranges>
#include <sstream>
#include <string>

#include "utils/BufferedReader.hpp"
#include "utils/SolutionBase.hpp"
class TwinkleStones : public SolutionBase {
    REGISTER( TwinkleStones )

    typedef long long ll;
    std::map<ll, ll> TwinkleList;
    void readFile() {
        for ( const std::string& line : BufferedReader( "Day11/input.txt" ).lines().yield() ) {
            std::stringstream ss( line );
            for ( int stone : std::views::istream<ll>( ss ) ) {
                TwinkleList[stone]++;
            }
        }
    }
    auto blink( int times ) {
        for ( int twinkle = 0; twinkle < times; twinkle++ ) {
            // cout << "Twinkle times: " << twinkle << "." << endl;
            std::map<ll, ll> nextList;
            for ( auto [stone, cnt] : TwinkleList ) {
                std::string str = std::to_string( stone );
                if ( stone == 0 ) {
                    nextList[1] += cnt;
                } else if ( str.size() % 2 == 0 ) {
                    // Rule 2
                    int middle = str.size() / 2;
                    std::string left = str.substr( 0, middle ), right = str.substr( middle );
                    nextList[std::stoll( left )] += cnt;
                    nextList[std::stoll( right )] += cnt;
                } else {
                    // Rule 3
                    nextList[stone * 2024] += cnt;
                }
            }
            TwinkleList = std::move( nextList );
        }
    }
    ll countRes() {
        return std::ranges::fold_left( TwinkleList | std::views::transform( []( auto const& res ) { return res.second; } ), 0, std::plus<>{} );
    }

   public:
    void Solution1() {
        readFile();
        blink( 25 );
        printRes( 1, countRes() );
    }
    void Solution2() {
        blink( 75 - 25 );
        printRes( 2, countRes() );
    }
};