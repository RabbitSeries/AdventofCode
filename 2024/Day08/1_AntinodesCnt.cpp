#include "bits/stdc++.h"
#define BUFFER_SIZE 1024
using namespace std;
typedef struct Antenna {
    int x, y;
    Antenna() { x = y = 0; }
    Antenna( int _x, int _y ) : x( _x ), y( _y ) {}
}pos;

bool isValid( pos p, const vector<vector<char>> map ) {
    return p.x >= 0 && p.x < map.size() && p.y >= 0 && p.y < map[p.x].size();
}

void HarmonicAntenna( const pos s, const pos t, vector<vector<char>>& antinodeMap ) {
    pos st, ts;
    st.x = s.x + ( t.x - s.x ) * 2;
    st.y = s.y + ( t.y - s.y ) * 2;
    ts.x = t.x + ( s.x - t.x ) * 2;
    ts.y = t.y + ( s.y - t.y ) * 2;
    if( isValid( st, antinodeMap ) ) {
        antinodeMap[st.x][st.y] = '#';
    }
    if( isValid( ts, antinodeMap ) ) {
        antinodeMap[ts.x][ts.y] = '#';
    }
    return;
}
// class tmp {
//     const int* const Method1( const int* const& const var ) const;
// };
void antennaEffectZone( const vector<pos> antennaList, vector<vector<char>>& antinodeMap ) {
    for( int i = 0; i < antennaList.size() - 1; i++ ) {
        for( int j = i + 1; j < antennaList.size(); j++ ) {
            HarmonicAntenna( antennaList[i], antennaList[j], antinodeMap );
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

int main() {
    map<char, vector<pos>> antennaLists;
    vector<char> antennaNames;
    vector<vector<char>> antinodeMap;
    FILE* input = fopen( "input.txt", "r" );
    // char (*linebuf)[BUFFER_SIZE+1] = {'\0'}; //argument of type "char (*)[1025]" is incompatible with parameter of type "char *"C/C++(167)
    char linebuf[BUFFER_SIZE] = { '\0' };
    int posx = 0;
    while( !feof( input ) && fgets( linebuf, BUFFER_SIZE, input ) ) {
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
        antennaEffectZone( antennaLists[name], antinodeMap );
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
    cout << antinodeCnt << endl;;
    return 0;
}