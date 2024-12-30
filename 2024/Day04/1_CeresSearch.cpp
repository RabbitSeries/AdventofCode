#include "bits/stdc++.h"
using namespace std;

bool checkHR( int i, int j, vector<vector<char>> m ) {
    if( j + 3 < m[i].size() ) {
        if( m[i][j] == 'X' && m[i][j + 1] == 'M' && m[i][j + 2] == 'A' && m[i][j + 3] == 'S' )
            return true;
        else
            return false;
    }
    else
        return false;
}
bool checkHL( int i, int j, vector<vector<char>> m ) {
    if( j - 3 >= 0 ) {
        if( m[i][j] == 'X' && m[i][j - 1] == 'M' && m[i][j - 2] == 'A' && m[i][j - 3] == 'S' )
            return true;
        else
            return false;
    }
    else
        return false;
}
bool checkVD( int i, int j, vector<vector<char>> m ) {
    if( i + 3 < m.size() ) {
        if( m[i][j] == 'X' && m[i + 1][j] == 'M' && m[i + 2][j] == 'A' && m[i + 3][j] == 'S' )
            return true;
        else
            return false;
    }
    else
        return false;
}
bool checkVU( int i, int j, vector<vector<char>> m ) {
    if( i - 3 >= 0 ) {
        if( m[i][j] == 'X' && m[i - 1][j] == 'M' && m[i - 2][j] == 'A' && m[i - 3][j] == 'S' )
            return true;
        else
            return false;
    }
    else
        return false;
}
bool checkDDR( int i, int j, vector<vector<char>> m ) {
    if( i + 3 < m.size() && j + 3 < m[i].size() ) {
        if( m[i][j] == 'X' && m[i + 1][j + 1] == 'M' && m[i + 2][j + 2] == 'A' && m[i + 3][j + 3] == 'S' )
            return true;
        else
            return false;
    }
    else
        return false;
}
bool checkDDL( int i, int j, vector<vector<char>>m ) {
    if( i + 3 < m.size() && j - 3 >= 0 ) {
        if( m[i][j] == 'X' && m[i + 1][j - 1] == 'M' && m[i + 2][j - 2] == 'A' && m[i + 3][j - 3] == 'S' )
            return true;
        else
            return false;
    }
    else
        return false;
}
bool checkDUR( int i, int j, vector<vector<char>>m ) {
    if( i - 3 >= 0 && j + 3 < m[i].size() ) {
        if( m[i][j] == 'X' && m[i - 1][j + 1] == 'M' && m[i - 2][j + 2] == 'A' && m[i - 3][j + 3] == 'S' )
            return true;
        else
            return false;
    }
    else
        return false;
}
bool checkDUL( int i, int j, vector<vector<char>> m ) {
    if( i - 3 >= 0 && j - 3 >= 0 ) {
        if( m[i][j] == 'X' && m[i - 1][j - 1] == 'M' && m[i - 2][j - 2] == 'A' && m[i - 3][j - 3] == 'S' )
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
            // Use char to string conversion, must call the constructor with 2 arguments in which the first argument is the number of times the second argument is repeated.
            // string s( 1, line[i] );
            row.push_back( line[i] );
        }
        m.push_back( row );
    }
    int res = 0;
    for( int i = 0; i < m.size(); i++ ) {
        cout << "line: " << i << endl;
        for( int j = 0; j < m[i].size(); j++ ) {
            if( checkHR( i, j, m ) ) {
                res++;
            }
            if( checkHL( i, j, m ) ) {
                res++;
            }
            if( checkVD( i, j, m ) ) {
                res++;
            }
            if( checkVU( i, j, m ) ) {
                res++;
            }
            if( checkDDR( i, j, m ) ) {
                res++;
            }
            if( checkDDL( i, j, m ) ) {
                res++;
            }
            if( checkDUR( i, j, m ) ) {
                res++;
            }
            if( checkDUL( i, j, m ) ) {
                res++;
            }
        }
    }
    cout << res;
    return 0;
}