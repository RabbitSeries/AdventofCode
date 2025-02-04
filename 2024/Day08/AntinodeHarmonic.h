#include "bits/stdc++.h"
#define BUFFER_SIZE 1024
using namespace std;


class AntinodeHarmonic {
    typedef struct Antenna {
        int x, y;
        Antenna() { x = y = 0; }
        Antenna( int _x, int _y ) : x( _x ), y( _y ) {}
    }pos;

    bool isValid( pos p, const vector<vector<char>> map ) {
        return p.x >= 0 && p.x < map.size() && p.y >= 0 && p.y < map[p.x].size();
    }

    void HarmonicAntenna( const pos s, const pos t, vector<vector<char>>& antinodeMap, bool isHarmony ) {
        pos st, ts;
        for( int harmonicTimes = isHarmony ? 1 : 2; isHarmony ? true : harmonicTimes < 3; harmonicTimes++ ) {
            bool withinLocale = false;
            st.x = s.x + ( t.x - s.x ) * harmonicTimes;
            st.y = s.y + ( t.y - s.y ) * harmonicTimes;
            ts.x = t.x + ( s.x - t.x ) * harmonicTimes;
            ts.y = t.y + ( s.y - t.y ) * harmonicTimes;
            if( isValid( st, antinodeMap ) ) {
                antinodeMap[st.x][st.y] = '#';
                withinLocale = true;
            }
            if( isValid( ts, antinodeMap ) ) {
                antinodeMap[ts.x][ts.y] = '#';
                withinLocale = true;
            }
            if( !withinLocale ) {
                break;
            }
        }

        return;
    }

    void antennaEffectZone( const vector<pos> antennaList, vector<vector<char>>& antinodeMap, bool isHarmony ) {
        for( int i = 0; i < antennaList.size() - 1; i++ ) {
            for( int j = i + 1; j < antennaList.size(); j++ ) {
                HarmonicAntenna( antennaList[i], antennaList[j], antinodeMap, isHarmony );
            }
        }
    }

    void showAntinode( const vector<vector<char>> antinodeMap ) {
        for( auto row : antinodeMap ) {
            for( auto sign : row ) {
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
        string linebuf;

        int posx = 0;
        while( getline( input, linebuf ) ) {
            string line( linebuf );
            vector<char> row;
            for( int i = 0; i < line.size(); i++ ) {
                if( line[i] == '\n' || line[i] == '\0' ) {
                    continue;
                }
                else {
                    row.push_back( line[i] );
                    if( line[i] != '.' ) {
                        if( antennaLists.find( line[i] ) == antennaLists.end() ) {
                            antennaLists[line[i]] = vector<pos>();
                            antennaNames.push_back( line[i] );
                        }
                        antennaLists[line[i]].push_back( pos( posx, i ) );
                    }
                }
            }
            antinodeMap.push_back( row );
            posx++;
            // cout << endl;
        }
        int debugLineCnt = 0;
        for( auto name : antennaNames ) {
            cout << "Processing antenna name: " << name << ". At " << ++debugLineCnt << "/" << antennaNames.size() << endl;
            antennaEffectZone( antennaLists[name], antinodeMap, isHarmony );
            showAntinode( antinodeMap );
        }
        int antinodeCnt = 0;
        for( auto row : antinodeMap ) {
            for( auto sign : row ) {
                if( sign == '#' ) {
                    antinodeCnt++;
                }
            }
        }

        cout << antinodeCnt << endl;
    }
public:
    void Solution1() {
        find( false );
    }

    void Solution2() {
        find( true );
    }
};
