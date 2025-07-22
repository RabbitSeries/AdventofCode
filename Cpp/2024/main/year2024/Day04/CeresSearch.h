#include "bits/stdc++.h"
using namespace std;
#include <utils/SolutionBase.hpp>
class CeresSearch : public SolutionBase {
	static inline Register<CeresSearch> _reg{};

    const int dx[8][4]{
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 1, 2, 3 },
        { 0, 1, 2, 3 },
        { 0, 1, 2, 3 },
        { 0, -1, -2, -3 },
        { 0, -1, -2, -3 },
        { 0, -1, -2, -3 },
    };

    const int dy[8][4]{
        { 0, 1, 2, 3 },
        { 0, -1, -2, -3 },
        { 0, 1, 2, 3 },
        { 0, 0, 0, 0 },
        { 0, -1, -2, -3 },
        { 0, 1, 2, 3 },
        { 0, 0, 0, 0 },
        { 0, -1, -2, -3 },
    };

    const int xdx[4][4]{
        { -1, 0, 1 },
        { -1, 0, 1 },
        { 1, 0, -1 },
        { 1, 0, -1 },
    };

    const int xdy[4][4]{
        { -1, 0, 1 },
        { 1, 0, -1 },
        { -1, 0, 1 },
        { 1, 0, -1 },
    };

    inline bool isXMAS( int i, int j, int k, vector<vector<char>> const& m ) {
        int rows = m.size(), rols = m[0].size();
        return i + dx[k][3] >= 0 && i + dx[k][3] < rows && j + dy[k][3] >= 0 && j + dy[k][3] < rols && m[i + dx[k][0]][j + dy[k][0]] == 'X' && m[i + dx[k][1]][j + dy[k][1]] == 'M' && m[i + dx[k][2]][j + dy[k][2]] == 'A' && m[i + dx[k][3]][j + dy[k][3]] == 'S';
    }

    inline bool isCrossMAS( int i, int j, int k, vector<vector<char>> const& m ) {
        int rows = m.size(), rols = m[0].size();
        return i + xdx[k][2] >= 0 && i + xdx[k][2] < rows && j + xdy[k][2] >= 0 && j + xdy[k][2] < rols &&
               i + xdx[k][0] >= 0 && i + xdx[k][0] < rows && j + xdy[k][0] >= 0 && j + xdy[k][0] < rols && m[i + xdx[k][0]][j + xdy[k][0]] == 'M' && m[i + xdx[k][1]][j + xdy[k][1]] == 'A' && m[i + xdx[k][2]][j + xdy[k][2]] == 'S';
    }

    void readFile() {
        ifstream input( "Day04/input.txt" );
        string line;
        while ( getline( input, line ) ) {
            vector<char> row;
            for ( size_t i = 0; i < line.length(); i++ ) {
                if ( line[i] == '\n' )
                    continue;
                row.push_back( line[i] );
            }
            m.push_back( row );
        }
    }
    vector<vector<char>> m;

   public:
    void Solution1() {
        readFile();
        int res = 0;
        for ( size_t i = 0; i < m.size(); i++ ) {
            for ( size_t j = 0; j < m[i].size(); j++ ) {
                for ( int k = 0; k < 8; k++ ) {
                    if ( isXMAS( i, j, k, m ) ) {
                        res++;
                    }
                }
            }
        }
        printRes( 1, res );
    }

    void Solution2() {
        int res = 0;
        for ( size_t i = 0; i < m.size(); i++ ) {
            for ( size_t j = 0; j < m[i].size(); j++ ) {
                int crossTimes = 0;
                for ( int k = 0; k < 4; k++ ) {
                    if ( isCrossMAS( i, j, k, m ) ) {
                        crossTimes++;
                    }
                }
                if ( crossTimes >= 2 ) {
                    res++;
                }
            }
        }
        printRes( 2, res );
    }
};