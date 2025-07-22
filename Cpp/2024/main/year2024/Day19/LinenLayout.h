#include <bits/stdc++.h>
using namespace std;
#include <utils/SolutionBase.hpp>
class LinenLayout : public SolutionBase {
	REGISTER( LinenLayout )

    typedef unsigned long long ull;
    void readKeys( vector<string>& keys, ifstream& input ) {
        regex pattern( R"(\b(.*?)(?:,|$))" );
        for ( string buf; getline( input, buf ) && !buf.empty(); ) {
            for ( sregex_iterator it( buf.begin(), buf.end(), pattern ), end_it; it != end_it; it++ ) {
                if ( !( *it )[1].str().empty() )
                    keys.push_back( ( *it )[1] );
            }
        }
    }

    bool match( vector<string> const& keys, string const& design, size_t curpos = 0 ) {
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
    vector<ull> dp;
    ull ALLMatch( vector<string> const& keys, string const& design ) {
        //     resize(size_type __new_size)
        //     {
        //   if (__new_size > size())
        //     _M_default_append(__new_size - size());
        //   else if (__new_size < size())
        //     _M_erase_at_end(this->_M_impl._M_start + __new_size);
        //     }
        dp.resize( design.size() + 1 );
        fill( dp.begin(), dp.end(), 0 );  // Must refill
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
    vector<string> keys, designs;

   public:
    void Solution1() {
        ifstream input( "Day19/input.txt" );  // RAII
        readKeys( keys, input );
        readKeys( designs, input );
        printRes( 1, ranges::count_if( designs.begin(), designs.end(), [this]( string const& design ) {
                      return match( keys, design );
                  } ) );
    }

    void Solution2() {
        printRes( 2, accumulate( designs.begin(), designs.end(), 0ull, [this]( ull init, string const& design ) {
                      return init + ALLMatch( keys, design );
                  } ) );
    }
};