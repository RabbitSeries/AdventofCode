#include "bits/stdc++.h"
using namespace std;
#include <utils/SolutionBase.hpp>
class KeyPair : public SolutionBase {
    static inline Register<KeyPair> _reg{};

    typedef vector<int> schematics;
    // TODO Change this algorithm to schema hash code and mapping.
    bool isUnique( schematics const& schema, vector<schematics> const& schemaList ) {
        for ( auto const& s : schemaList ) {
            bool isEuqal = true;
            for ( int pin = 0; pin < 5; pin++ ) {
                if ( schema[pin] != s[pin] ) {
                    isEuqal = false;
                }
            }
            if ( isEuqal ) {
                return false;
            }
        }
        return true;
    }

   public:
    void Solution1() {
        vector<schematics> lockList, keyList;
        ifstream input( "Day25/input.txt" );
        int rowTimes = 0;
        bool isLock = true;
        schematics row( 5, 0 );
        for ( string linebuf; getline( input, linebuf ); ) {
            if ( linebuf.size() < 5 ) {
                continue;
            } else {
                if ( rowTimes == 0 ) {
                    if ( linebuf.substr( 0, 5 ) == "#####" ) {
                        isLock = true;
                        rowTimes++;
                    } else if ( linebuf.substr( 0, 5 ) == "....." ) {
                        isLock = false;
                        rowTimes++;
                    }
                } else {
                    if ( rowTimes != 6 ) {
                        for ( int pin = 0; pin < 5; pin++ ) {
                            if ( linebuf[pin] == '#' ) {
                                row[pin]++;
                            }
                        }
                        rowTimes++;
                    } else {
                        if ( isLock ) {
                            if ( isUnique( row, lockList ) ) {
                                lockList.push_back( row );
                            }
                        } else {
                            if ( isUnique( row, keyList ) ) {
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
        for ( schematics& lock : lockList ) {
            for ( schematics& key : keyList ) {
                bool fit = true;
                for ( int pin = 0; pin < 5; pin++ ) {
                    if ( lock[pin] + key[pin] > 5 ) {
                        fit = false;
                    }
                }
                if ( fit ) {
                    res++;
                }
            }
        }
        printRes( 1, res );
    }

    void Solution2() {
        printRes( 2, "\t\t\t------All 50 stars AoC Finished!!!!------" );
    }
};