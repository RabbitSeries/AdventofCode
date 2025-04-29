#include <bits/stdc++.h>
using namespace std;
#include "../../utils/SolutionBase.h"
class LinenLayout : public SolutionBase {
    typedef unsigned long long ull;
    vector<string> readKeys( ifstream& input ) {
        vector<string> keys;
        regex pattern( R"(\b(.*?)(?:,|$))" );
        for ( string buf; getline( input, buf ) && !buf.empty(); ) {
            sregex_iterator it( buf.begin(), buf.end(), pattern ), end_it;
            while ( it != end_it ) {
                if ( !( *it )[1].str().empty() )
                    keys.push_back( ( *it )[1] );
                it++;
            }
        }
        return keys;
    }

    bool match( vector<string> const& keys, string const& design, size_t curpos = 0 ) {
        if ( curpos == design.size() ) {
            // cout << design << endl;
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

    ull ALLMatch( vector<string> const& keys, string const& design ) {
        vector<ull> dp( design.size() + 1, 0 );
        dp[0] = 1;
        for ( size_t i = 1; i <= design.size(); i++ ) {
            for ( auto& key : keys ) {
                if ( key.size() <= i && design.substr( i - key.size(), key.size() ) == key ) {
                    dp[i] += dp[i - key.size()];
                }
            }
        }
        return dp[design.size()];
    }
    vector<string> keys, designs;

   public:
    void Solution1() {
        ifstream keyInput( "Day19/input.txt" );
        keys = readKeys( keyInput );
        designs = readKeys( keyInput );
        int res = 0;
        for_each( designs.begin(), designs.end(), [&]( string design ) {
            if ( match( keys, design ) ) res++;
        } );
        printRes( 1, res );
        keyInput.close();
    }

    void Solution2() {
        ull res = 0;
        for_each( designs.begin(), designs.end(), [&]( string const& design ) {
            res += ALLMatch( keys, design );
            // cout << "Curruntly thre are: " << res << " ways" << endl;
        } );
        printRes( 2, res );
    }
};