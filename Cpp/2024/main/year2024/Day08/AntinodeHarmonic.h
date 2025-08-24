#include <algorithm>
#include <cstddef>
#include <map>
#include <ranges>
#include <set>
#include <vector>

#include "utils/BufferedReader.hpp"
#include "utils/SolutionBase.hpp"
class AntinodeHarmonic : public SolutionBase {
    REGISTER( AntinodeHarmonic )

    typedef struct Antenna {
        int x, y;
        Antenna() { x = y = 0; }
        Antenna( int _x, int _y ) : x( _x ), y( _y ) {}
    } pos;

    bool isValid( pos p, const std::vector<std::string>& map ) {
        return p.x >= 0 && p.x < rows && p.y >= 0 && p.y < cols;
    }

    void HarmonicAntenna( const pos& s, const pos& t, std::vector<std::string>& antinodeMap, bool isHarmony ) {
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
    void antennaEffectZone( const std::vector<pos>& antennaList, std::vector<std::string>& antinodeMap, bool isHarmony ) {
        for ( int i : std::views::iota( 0, (int)( antennaList.size() - 1 ) ) ) {
            for ( int j : std::views::iota( i + 1, (int)antennaList.size() ) ) {
                HarmonicAntenna( antennaList[i], antennaList[j], antinodeMap, isHarmony );
            }
        }
    }
    void find( bool isHarmony ) {
        using namespace std;
        vector<string> antinodeMap = data;
        // char (*linebuf)[BUFFER_SIZE+1] = {'\0'}; //argument of type "char (*)[1025]" is incompatible with parameter of type "char *"C/C++(167)
        for ( auto& name : antennaNames ) {
            antennaEffectZone( antennaLists.at( name ), antinodeMap, isHarmony );
        }
        std::ptrdiff_t antinodeCnt = 0;
        for ( const auto& row : antinodeMap ) {
            antinodeCnt += ranges::count_if( row, []( char c ) { return c == '#'; } );
        }
        printRes( !isHarmony ? 1 : 2, antinodeCnt );
    }

    std::map<char, std::vector<pos>> antennaLists;
    std::set<char> antennaNames;
    std::vector<std::string> data;
    int rows, cols;

    void readFile() {
        data = BufferedReader( "Day08/input.txt" ).lines().toList();
        rows = data.size();
        cols = data[0].size();
        for ( int x : std::views::iota( 0, rows ) ) {
            const std::string& line = data[x];
            for ( int y : std::views::iota( 0, cols ) ) {
                if ( line[y] != '.' ) {
                    antennaNames.emplace( line[y] );
                    antennaLists[line[y]].emplace_back( x, y );
                }
            }
        }
    }

   public:
    void Solution1() {
        readFile();
        find( false );
    }

    void Solution2() {
        find( true );
    }
};
