#include <algorithm>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

#include "utils/BufferedReader.hpp"
#include "utils/ISolution.hpp"
#include "utils/Stream/RegexStream.hpp"
class Calibration : public ISolution {
    REGISTER( Calibration )

    using ull = unsigned long long;
    void readFile() {
        using namespace std;
        for ( string buf : BufferedReader( "Day07/input.txt" ).lines().yield() ) {
            auto nums = RegexStream( R"(\d+)", buf ).yield() | views::transform( []( const smatch& m ) {
                            return stoull( m.str() );
                        } ) |
                        ranges::to<vector<ull>>();
            numList.emplace_back( nums[0], vector( nums.begin() + 1, nums.end() ) );
        }
    }
    size_t dfsParse( size_t test, std::vector<ull> const& nums, const size_t curPos, const size_t curRes, const int options ) {
        if ( curPos == 0 ) return ( curRes == 0 ? test : 0 );
        if ( curRes >= nums[curPos - 1] ) {
            if ( dfsParse( test, nums, curPos - 1, curRes - nums[curPos - 1], options ) == test )
                return test;
            if ( ( curRes % nums[curPos - 1] ) == 0 && dfsParse( test, nums, curPos - 1, curRes / nums[curPos - 1], options ) == test )
                return test;
            if ( options == 3 ) {
                std::string curStr = std::to_string( curRes );
                std::string formerStr = std::to_string( nums[curPos - 1] );
                if ( curStr.ends_with( formerStr ) ) {
                    size_t pos = curStr.size() - formerStr.size(), nextRes = 0;
                    if ( pos != 0 ) {
                        nextRes = stoull( curStr.substr( 0, pos ) );
                    }
                    if ( dfsParse( test, nums, curPos - 1, nextRes, options ) == test ) {
                        return test;
                    }
                }
            }
        }
        return 0;
    }
    using K_V = std::pair<ull, std::vector<ull>>;
    std::vector<K_V> numList;
    size_t parseWith( const int options ) {
        return std::ranges::fold_left( numList |
                                           std::views::transform( [options, this]( const K_V& k_v ) {
                                               return dfsParse( k_v.first, k_v.second, k_v.second.size(), k_v.first, options );
                                           } ),
                                       0ull, std::plus<>{} );
    }

   public:
    void Solution1() {
        readFile();
        printRes( 1, parseWith( 2 ) );
    }

    void Solution2() {
        printRes( 2, parseWith( 3 ) );
    }
};