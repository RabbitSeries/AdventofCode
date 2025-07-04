#include "bits/stdc++.h"
using namespace std;

#include <utils/SolutionBase.hpp>
class TwinkleStones : public SolutionBase {
    typedef long long ll;
    map<ll, ll> TwinkleList;
    void readFile() {
        ifstream input( "Day11/input.txt" );
        for ( string linebuf; getline( input, linebuf ); ) {
            stringstream line( linebuf );
            for ( int tmp = 0; line >> tmp; ) {
                TwinkleList[tmp]++;
            }
        }
    }
    auto blink( int times ) {
        for ( int twinkle = 0; twinkle < times; twinkle++ ) {
            // cout << "Twinkle times: " << twinkle << "." << endl;
            map<ll, ll> nextList;
            for ( auto const& stone : TwinkleList ) {
                stringstream ss;
                ss << stone.first;
                if ( stone.first == 0 ) {
                    nextList[1] += stone.second;
                } else if ( ss.str().size() % 2 == 0 ) {
                    // Rule 2
                    int middle = ss.str().size() / 2;
                    string left = ss.str().substr( 0, middle ), right = ss.str().substr( middle );
                    nextList[stoll( left )] += stone.second;
                    nextList[stoll( right )] += stone.second;
                } else {
                    // Rule 3
                    nextList[stone.first * 2024] += stone.second;
                }
            }
            TwinkleList = nextList;
        }
    }
    ll countRes() {
        ll res = 0;
        for ( auto const& partialRes : TwinkleList ) {
            res += partialRes.second;
        }
        return res;
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