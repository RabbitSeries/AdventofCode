#include "bits/stdc++.h"
#define BUFFER_SIZE 1024
using namespace std;

#include "../../utils/SolutionBase.h"
class AntinodeHarmonic : public SolutionBase {
    typedef struct Antenna {
        size_t x, y;
        Antenna() { x = y = 0; }
        Antenna( size_t _x, size_t _y ) : x( _x ), y( _y ) {}
    } pos;

    bool isValid( pos p, const vector<vector<char>> map ) {
        return p.x >= 0 && p.x < map.size() && p.y >= 0 && p.y < map[p.x].size();
    }

    void HarmonicAntenna( const pos& s, const pos& t, vector<vector<char>>& antinodeMap, bool isHarmony ) {
        pos st, ts;
        for ( int harmonicTimes = isHarmony ? 1 : 2; isHarmony ? true : harmonicTimes < 3; harmonicTimes++ ) {
            bool withinLocale = false;
            st.x = s.x + ( t.x - s.x ) * harmonicTimes;
            st.y = s.y + ( t.y - s.y ) * harmonicTimes;
            ts.x = t.x + ( s.x - t.x ) * harmonicTimes;
            ts.y = t.y + ( s.y - t.y ) * harmonicTimes;
            if ( isValid( st, antinodeMap ) ) {
                antinodeMap[st.x][st.y] = '#';
                withinLocale = true;
            }
            if ( isValid( ts, antinodeMap ) ) {
                antinodeMap[ts.x][ts.y] = '#';
                withinLocale = true;
            }
            if ( !withinLocale ) {
                break;
            }
        }

        return;
    }

    void antennaEffectZone( const vector<pos>& antennaList, vector<vector<char>>& antinodeMap, bool isHarmony ) {
        for ( size_t i = 0; i < antennaList.size() - 1; i++ ) {
            for ( size_t j = i + 1; j < antennaList.size(); j++ ) {
                HarmonicAntenna( antennaList[i], antennaList[j], antinodeMap, isHarmony );
            }
        }
    }

    void showAntinode( const vector<vector<char>>& antinodeMap ) {
        for ( auto& row : antinodeMap ) {
            for ( auto& sign : row ) {
                cout << sign;
            }
            cout << endl;
        }
    }

    void find( bool isHarmony ) {
        map<char, vector<pos>> antennaLists;
        vector<char> antennaNames;
        vector<vector<char>> antinodeMap;
        ifstream input( "Day08/input.txt" );
        // char (*linebuf)[BUFFER_SIZE+1] = {'\0'}; //argument of type "char (*)[1025]" is incompatible with parameter of type "char *"C/C++(167)
        int posx = 0;
        for ( string linebuf; getline( input, linebuf ); ) {
            vector<char> row;
            for ( size_t i = 0; i < linebuf.size(); i++ ) {
                if ( linebuf[i] == '\n' || linebuf[i] == '\0' ) {
                    continue;
                } else {
                    row.push_back( linebuf[i] );
                    if ( linebuf[i] != '.' ) {
                        if ( antennaLists.find( linebuf[i] ) == antennaLists.end() ) {
                            antennaLists[linebuf[i]] = vector<pos>();
                            antennaNames.push_back( linebuf[i] );
                        }
                        antennaLists[linebuf[i]].push_back( pos( posx, i ) );
                    }
                }
            }
            antinodeMap.push_back( row );
            posx++;
            // cout << endl;
        }
        // int debugLineCnt = 0;
        for ( auto& name : antennaNames ) {
            // cout << "Processing antenna name: " << name << ". At " << ++debugLineCnt << "/" << antennaNames.size() << endl;
            antennaEffectZone( antennaLists[name], antinodeMap, isHarmony );
            // showAntinode( antinodeMap );
        }
        int antinodeCnt = 0;
        for ( auto& row : antinodeMap ) {
            for ( auto& sign : row ) {
                if ( sign == '#' ) {
                    antinodeCnt++;
                }
            }
        }
        !isHarmony ? printRes( 1, antinodeCnt ) : printRes( 2, antinodeCnt );
    }

   public:
    void Solution1() {
        find( false );
    }

    void Solution2() {
        find( true );
    }
};
