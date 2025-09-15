#include <algorithm>
#include <cstddef>
#include <fstream>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

#include "utils/ISolution.hpp"
class LinenLayout : public ISolution {
    REGISTER( LinenLayout )

    using ull = unsigned long long;
    void readKeys( std::vector<std::string>& keys, std::ifstream& input ) {
        std::regex pattern( R"(\b(.*?)(?:,|$))" );
        for ( std::string buf; std::getline( input, buf ) && !buf.empty(); ) {
            for ( std::sregex_iterator it( buf.begin(), buf.end(), pattern ), end_it; it != end_it; it++ ) {
                if ( !( *it )[1].str().empty() )
                    keys.push_back( ( *it )[1] );
            }
        }
    }

    bool match( std::vector<std::string> const& keys, std::string const& design, size_t curpos = 0 ) {
        if ( curpos == design.size() ) {
            return true;
        }
        for ( auto const& key : keys ) {
            if ( key == design.substr( curpos, key.size() ) ) {
                curpos += key.size();
                if ( match( keys, design, curpos ) ) {
                    return true;
                } else {
                    curpos -= key.size();
                }
            }
        }
        return false;
    }
    std::vector<ull> dp;
    ull ALLMatch( std::vector<std::string> const& keys, std::string const& design ) {
        //     resize(size_type __new_size)
        //     {
        //   if (__new_size > size())
        //     _M_default_append(__new_size - size());
        //   else if (__new_size < size())
        //     _M_erase_at_end(this->_M_impl._M_start + __new_size);
        //     }
        dp.resize( design.size() + 1 );
        std::ranges::fill( dp, 0 );  // Must refill
        dp[0] = 1;
        for ( size_t i = 1; i <= design.size(); i++ ) {
            for ( auto& key : keys ) {
                if ( key.size() <= i && equal( i - key.size() + design.begin(), i + design.begin(), key.begin() ) ) {
                    dp[i] += dp[i - key.size()];
                }
            }
        }
        return dp[design.size()];
    }
    std::vector<std::string> keys, designs;

   public:
    void Solution1() {
        std::ifstream input( "Day19/input.txt" );  // RAII
        readKeys( keys, input );
        readKeys( designs, input );
        printRes( 1, std::ranges::count_if( designs, [this]( std::string const& design ) {
                      return match( keys, design );
                  } ) );
    }

    void Solution2() {
        printRes( 2, std::ranges::fold_left( designs, 0ull, [this]( ull init, std::string const& design ) {
                      return init + ALLMatch( keys, design );
                  } ) );
    }
};