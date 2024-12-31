#include "bits/stdc++.h"
#include <cstddef>
#include <iostream>
#include <regex>
#include <string>
#define BUFFER_SIZE 1024
#define TOP_UP 10000000000000
#define MAX_ATTEMPT 100
using namespace std;
typedef long long ll;
typedef pair<ll, ll> point2D;
typedef struct problem {
    point2D a, b;
    point2D prize;
}problem;

ll solve( const problem p ) {
    // Restriction: ta : button a attempt count, tb : button b
    // a.fitst*ta+b.first*tb = prize.first
    // a.second*ta+b.second*tb = prize.second
    // Target = 3*ta + tb;
    point2D a = p.a, b = p.b, prize = p.prize;
    ll ta = 0, tb = 0;
    bool ba = false;
    if( ( prize.first * b.second - prize.second * b.first ) % ( a.first * b.second - a.second * b.first ) == 0 ) {
        ta = ( prize.first * b.second - prize.second * b.first ) / ( a.first * b.second - a.second * b.first );
        // if( ta <= MAX_ATTEMPT ) {
        ba = true;
        // }
    }
    if( ba && ( prize.first * a.second - prize.second * a.first ) % ( a.second * b.first - a.first * b.second ) == 0 ) {
        tb = ( prize.first * a.second - prize.second * a.first ) / ( a.second * b.first - a.first * b.second );
        // if( tb <= MAX_ATTEMPT ) {
        return 3 * ta + tb;
        // }
    }
    return -1;
}

int main() {
    FILE* input = fopen( "input.txt", "r" );
    char buf[BUFFER_SIZE + 1];
    vector<problem> problemSet;
    // Regex "^.*\+([0-9]+),.*\+([0-9]+)$"
    // Regex "\+([0-9]+).*\+([0-9]+)"
    // Regex "(?=(\+.*?)[0-9]+,)\1([0-9]+)(?=(.*\+)[0-9])\3([0-9]+)"
    regex re( "[\\+=]([0-9]+).*[\\+=]([0-9]+)" );
    smatch m;
    ll inProblem = 0;
    while( !feof( input ) && fgets( buf, BUFFER_SIZE + 1, input ) ) {
        // _GLIBCXX_RESOLVE_LIB_DEFECTS
        // 2329. regex_search() with match_results should forbid temporary strings
        /// Prevent unsafe attempts to get match_results from a temporary string.
        // std::regex_search( string( buf ), m, re );
        string tmp( buf );
        std::regex_search( tmp, m, re );
        // cout << m.prefix() << endl;
        // cout << m.suffix() << endl;

        if( m.size() == 3 ) {
            if( !inProblem ) {
                problemSet.push_back( problem() );
                problemSet.back().a = pair<ll, ll>( stoi( m[1] ), stoi( m[2] ) );
                inProblem++;
            }
            else {
                if( inProblem == 1 ) {
                    problemSet.back().b = pair<ll, ll>( stoi( m[1] ), stoi( m[2] ) );
                    inProblem++;
                }
                else {
                    // InProblem == 2
                    problemSet.back().prize = pair<ll, ll>( stoi( m[1] ) + TOP_UP, stoi( m[2] ) + TOP_UP );
                    inProblem = 0;
                }
            }
        }
    }
    ll tokenCnt = 0;
    for_each( problemSet.begin(), problemSet.end(), [ & ]( const  problem p ) {
        ll res = solve( p );
        if( res >= 0 ) {
            tokenCnt += res;
        }
    } );
    cout << tokenCnt << endl;
    return 0;

}