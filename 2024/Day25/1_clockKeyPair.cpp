#include "bits/stdc++.h"
#define BUFFER_SIZE 10
using namespace std;
typedef vector<int> schematics;

bool isUnique( schematics schema, vector<schematics> schemaList ) {
    for( auto s : schemaList ) {
        bool isEuqal = true;
        for( int pin = 0; pin < 5; pin++ ) {
            if( schema[pin] != s[pin] ) {
                isEuqal = false;
            }
        }
        if( isEuqal ) {
            return false;
        }
    }
    return true;
}

int main() {
    vector<schematics> lockList, keyList;
    FILE* input = fopen( "input.txt", "r" );
    char linebuf[BUFFER_SIZE + 1] = "\0";
    int rowTimes = 0;
    bool isLock = true;
    schematics row( 5, 0 );
    while( !feof( input ) && fgets( linebuf, BUFFER_SIZE, input ) ) {
        string line( linebuf );
        if( line.size() < 5 ) {
            continue;
        }
        else {
            if( rowTimes == 0 ) {
                if( line.substr( 0, 5 ) == "#####" ) {
                    // lockList.push_back( schematics( 5, 0 ) );
                    isLock = true;
                    rowTimes++;
                }
                else if( line.substr( 0, 5 ) == "....." ) {
                    // keyList.push_back( schematics( 5, 0 ) );
                    isLock = false;
                    rowTimes++;
                }
            }
            else {
                if( rowTimes != 6 ) {
                    for( int pin = 0; pin < 5; pin++ ) {
                        if( line[pin] == '#' ) {
                            row[pin]++;
                        }
                    }
                    rowTimes++;
                }
                else {
                    if( isLock ) {
                        if( isUnique( row, lockList ) ) {
                            lockList.push_back( row );
                        }
                    }
                    else {
                        if( isUnique( row, keyList ) ) {
                            keyList.push_back( row );
                        }
                    }
                    fill( row.begin(), row.end(), 0 );
                    rowTimes = 0;
                }
            }
        }

    }
    int res = 0;
    for( schematics lock : lockList ) {
        for( schematics key : keyList ) {
            bool fit = true;
            for( int pin = 0; pin < 5; pin++ ) {
                if( lock[pin] + key[pin] > 5 ) {
                    fit = false;
                }
            }
            if( fit ) {
                res++;
            }
        }
    }
    cout << res << endl;
}