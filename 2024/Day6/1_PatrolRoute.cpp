#include "bits/stdc++.h"
#define BUFFER_SIZE 1024
using namespace std;
void patrol( int i, int j, vector<vector<char>>& routeMap ) {
    while( i >= 0 && i < routeMap.size() && j >= 0 && j < routeMap[i].size() ) {
        // Determine the face
        if( routeMap[i][j] == '^' ) {
            if( i - 1 >= 0 ) {
                if( routeMap[i - 1][j] == '#' )
                    routeMap[i][j] = '>';
                else {
                    routeMap[i--][j] = 'X';
                    routeMap[i][j] = '^';
                }
            }
            else {
                routeMap[i--][j] = 'X';
            }
        }
        else if( routeMap[i][j] == '>' ) {
            if( j + 1 < routeMap[i].size() ) {
                if( routeMap[i][j + 1] == '#' )
                    routeMap[i][j] = 'v';
                else {
                    routeMap[i][j++] = 'X';
                    routeMap[i][j] = '>';
                }
            }
            else {
                routeMap[i][j++] = 'X';
            }
        }
        else if( routeMap[i][j] == 'v' ) {
            if( i + 1 < routeMap.size() ) {
                if( routeMap[i + 1][j] == '#' )
                    routeMap[i][j] = '<';
                else {
                    routeMap[i++][j] = 'X';
                    routeMap[i][j] = 'v';
                }
            }
            else {
                routeMap[i++][j] = 'X';
            }
        }
        else if( routeMap[i][j] == '<' ) {
            if( j - 1 >= 0 ) {
                if( routeMap[i][j - 1] == '#' )
                    routeMap[i][j] = '^';
                else {
                    routeMap[i][j--] = 'X';
                    routeMap[i][j] = '<';
                }
            }
            else {
                routeMap[i][j--] = 'X';
            }
        }
        else {
            cout << "Erro situation" << endl;
        }

    }
}

int main() {
    vector<vector<char>> routeMap;
    pair<int, int> guardPos( 0, 0 );
    char lineBuf[BUFFER_SIZE + 1] = { '\0' };
    FILE* input = fopen( "input.txt", "r" );
    while( !feof( input ) && fgets( lineBuf, BUFFER_SIZE, input ) ) {
        vector<char> row;
        int i = 0;
        while( lineBuf[i] != '\n' && lineBuf[i] != '\0' ) {
            if( lineBuf[i] == '<' || lineBuf[i] == '>' || lineBuf[i] == '^' || lineBuf[i] == 'v' ) {
                guardPos.first = routeMap.size();
                guardPos.second = i;
            }
            row.push_back( lineBuf[i] );
            i++;
        }
        if( !row.empty() ) {
            routeMap.push_back( row );
        }
    }
    patrol( guardPos.first, guardPos.second, routeMap );
    int distinctPosCnt = 0;
    for( vector<char> row : routeMap ) {
        for( char c : row ) {
            cout << c;
            if( c == 'X' ) {
                distinctPosCnt++;
            }
        }
        cout << endl;
    }
    cout << distinctPosCnt << endl;
    return 0;
}

