#include "bits/stdc++.h"
using namespace std;
bool checkLU( int i, int j, vector<vector<char>> m ) {
    if( i - 1 >= 0 && j - 1 >= 0 && i + 1 < m.size() && j + 1 < m[i].size() ) {
        if( m[i - 1][j - 1] == 'M' && m[i][j] == 'A' && m[i + 1][j + 1] == 'S' )
            return true;
        else
            return false;
    }
    else
        return false;
}
bool checkRU( int i, int j, vector<vector<char>> m ) {
    if( i - 1 >= 0 && j + 1 < m[i].size() && i + 1 < m.size() && j - 1 >= 0 ) {
        if( m[i - 1][j + 1] == 'M' && m[i][j] == 'A' && m[i + 1][j - 1] == 'S' )
            return true;
        else
            return false;
    }
    else
        return false;
}
bool checkLD( int i, int j, vector<vector<char>> m ) {
    if( i - 1 >= 0 && j - 1 >= 0 && i + 1 < m.size() && j + 1 < m[i].size() ) {
        if( m[i + 1][j - 1] == 'M' && m[i][j] == 'A' && m[i - 1][j + 1] == 'S' )
            return true;
        else
            return false;
    }
    else
        return false;
}
bool checkRD( int i, int j, vector<vector<char>> m ) {
    if( i - 1 >= 0 && j - 1 >= 0 && i + 1 < m.size() && j + 1 < m[i].size() ) {
        if( m[i + 1][j + 1] == 'M' && m[i][j] == 'A' && m[i - 1][j - 1] == 'S' )
            return true;
        else
            return false;
    }
    else
        return false;
}

int main() {
    vector<vector<char>> m;
    FILE* input = fopen( "input.txt", "r" );
    char linebuffer[( 1 << 10 ) + 1] = { "\0" };
    while( !feof( input ) && fgets( linebuffer, 1 << 10, input ) ) {
        vector<char> row;
        string line = string( linebuffer );
        for( int i = 0; i < line.length(); i++ ) {
            if( line[i] == '\n' )
                continue;
            row.push_back( line[i] );
        }
        m.push_back( row );
    }
    int res = 0;
    for( int i = 0; i < m.size(); i++ ) {
        cout << "line: " << i << endl;
        for( int j = 0; j < m[i].size(); j++ ) {
            int crossTiems = 0;
            if( checkLU( i, j, m ) ) {
                crossTiems++;
            }
            if( checkRU( i, j, m ) ) {
                crossTiems++;
            }
            if( checkLD( i, j, m ) ) {
                crossTiems++;
            }
            if( checkRD( i, j, m ) ) {
                crossTiems++;
            }
            if( crossTiems >= 2 ) {
                res++;
            }
        }
    }
    cout << res << endl;
    return 0;
}