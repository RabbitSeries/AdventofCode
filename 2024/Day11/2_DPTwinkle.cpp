#include "bits/stdc++.h"
#define BUFFER_SIZE 1024
using namespace std;
typedef long long ll;

map<ll, ll> nextBlink( const map<ll, ll> TwinkleList ) {
    map<ll, ll> nextList;
    for( auto stone : TwinkleList ) {
        stringstream ss;
        ss << stone.first;
        if( stone.first == 0 ) {
            nextList[1] += stone.second;
        }
        else if( ss.str().size() % 2 == 0 ) {
            // Rule 2
            int middle = ss.str().size() / 2;
            string left = ss.str().substr( 0, middle ), right = ss.str().substr( middle );
            nextList[stoll( left )] += stone.second;
            nextList[stoll( right )] += stone.second;
        }
        else {
            // Rule 3
            nextList[stone.first * 2024] += stone.second;
        }
    }
    return nextList;
}


int main() {
    map<ll, ll> TwinkleList;
    FILE* input = fopen( "input.txt", "r" );
    char linebuf[BUFFER_SIZE + 1] = "\0";
    while( !feof( input ) && fgets( linebuf, BUFFER_SIZE, input ) ) {
        stringstream line( linebuf );
        int tmp = 0;
        while( line >> tmp ) {
            TwinkleList[tmp]++;
        }
    }
    for( int twinkle = 0; twinkle < 75; twinkle++ ) {
        cout << "Twinkle times: " << twinkle << "." << endl;
        TwinkleList = nextBlink( TwinkleList );
    }
    ll res = 0;
    for( auto partialRes : TwinkleList ) {
        res += partialRes.second;
    }
    cout << "Final res :" << res << endl;
    return 0;
}