#include <bits/stdc++.h>
using namespace std;
class LinenLayout {
    typedef unsigned long long ull;
    vector<string> readKeys( ifstream& input ) {
        vector<string> keys;
        string buf;
        regex pattern( "\\b(.*?)(?:,|$|\\n)" );
        while( getline( input, buf ) ) {
            string strbuf( buf );
            sregex_iterator it( strbuf.begin(), strbuf.end(), pattern ), end_it;
            while( distance( it, end_it ) != 0 ) {
                if( !( *it )[1].str().empty() )
                    keys.push_back( ( *it )[1] );
                it++;
            }
        }
        return keys;
    }

    bool match( vector<string> keys, string design, int curpos = 0 ) {
        if( curpos == design.size() ) {
            // cout << design << endl;
            return true;
        }
        for( auto key : keys ) {
            if( key == design.substr( curpos, key.size() ) ) {
                curpos += key.size();
                if( match( keys, design, curpos ) ) {
                    return true;
                }
                else {
                    curpos -= key.size();
                }
            }
        }
        return false;
    }


    ull ALLMatch( vector<string> keys, string design ) {
        vector<ull> dp( design.size() + 1, 0 );
        dp[0] = 1;
        for( int i = 1; i <= design.size(); i++ ) {
            for( auto key : keys ) {
                if( key.size() <= i && design.substr( i - key.size(), key.size() ) == key ) {
                    dp[i] += dp[i - key.size()];
                }
            }
        }
        return dp[design.size()];
    }
public:
    void Solution1() {
        vector<string> keys, designs;
        // FILE* keyInput( fopen( "ExampleKeys.txt", "r" ) ), * designInput( fopen( "ExampleInput.txt", "r" ) );
        ifstream keyInput( "Day19/keys.txt" ), designInput( "Day19/input.txt" );
        keys = readKeys( keyInput );
        designs = readKeys( designInput );
        int res = 0;
        for_each( designs.begin(), designs.end(), [ & ]( string design ) {
            if( match( keys, design ) ) res++;
        } );
        cout << "Solution 1: " << res << endl;
        keyInput.close(), designInput.close();
    }

    void Solution2() {
        vector<string> keys, designs;
        // FILE* keyInput( fopen( "ExampleKeys.txt", "r" ) ), * designInput( fopen( "ExampleInput.txt", "r" ) );
        ifstream keyInput( "Day19/keys.txt" ), designInput( "Day19/input.txt" );
        keys = readKeys( keyInput );
        designs = readKeys( designInput );
        ull res = 0;
        for_each( designs.begin(), designs.end(), [ & ]( string design ) {
            res += ALLMatch( keys, design );
            // cout << "Curruntly thre are: " << res << " ways" << endl;
        } );
        cout << "Solution 2: " << res << endl;
        keyInput.close(), designInput.close();
    }


};