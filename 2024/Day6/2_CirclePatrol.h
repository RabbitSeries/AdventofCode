#include "bits/stdc++.h"
#define BUFFER_SIZE 1024
using namespace std;
void disPlayMap( vector<vector<char>> routeMap ) {
    int left = 0;
    for( int i = 0; i < routeMap.size(); i++ ) {
        for( int j = 0; j < routeMap[i].size(); j++ ) {
            if( routeMap[i][j] == '.' ) {
                left++;
            }
            cout << routeMap[i][j];
        }
        cout << endl;
    }
    cout << " empty space : " << left << endl;
}
bool isPatrolCircle( int i, int j, vector<vector<char>> routeMap, int x, int y, int rowSize, int colSize ) {
    routeMap[x][y] = '#';
    bool isCircle = false;
    vector<vector<pair<bool, char>>> passed( rowSize, vector<pair<bool, char>>( colSize, pair<bool, char>( false, '.' ) ) );
    int times = 0;
    while( i >= 0 && i < routeMap.size() && j >= 0 && j < routeMap[i].size() ) {
        times++;
        // if( x == 39 && y == 91 && times >= 30000 ) {
        // cout << "Checking line " << x << " column: " << y << " " << "trial: " << times << endl;
        // disPlayMap( routeMap );
        // }
        // disPlayMap( routeMap );
        if( passed[i][j].first && passed[i][j].second == routeMap[i][j] ) {
            isCircle = true;
            return isCircle;
        }
        // Determine the face
        if( routeMap[i][j] == '^' ) {
            if( i - 1 >= 0 ) {
                if( routeMap[i - 1][j] == '#' ) {
                    routeMap[i][j] = '>';
                }
                else {
                    passed[i][j].first = true;
                    passed[i][j].second = '^';
                    routeMap[i--][j] = '|';
                    routeMap[i][j] = '^';
                }
            }
            else {
                return false;
            }
        }
        else if( routeMap[i][j] == '>' ) {
            if( j + 1 < routeMap[i].size() ) {
                if( routeMap[i][j + 1] == '#' ) {
                    routeMap[i][j] = 'v';
                }
                else {
                    passed[i][j].first = true;
                    passed[i][j].second = '>';
                    routeMap[i][j++] = '-';
                    routeMap[i][j] = '>';
                }
            }
            else {
                return false;
            }
        }
        else if( routeMap[i][j] == 'v' ) {
            if( i + 1 < routeMap.size() ) {
                if( routeMap[i + 1][j] == '#' ) {
                    routeMap[i][j] = '<';
                }
                else {
                    passed[i][j].first = true;
                    passed[i][j].second = 'v';
                    routeMap[i++][j] = '|';
                    routeMap[i][j] = 'v';
                }
            }
            else {
                return false;
            }
        }
        else if( routeMap[i][j] == '<' ) {
            if( j - 1 >= 0 ) {
                if( routeMap[i][j - 1] == '#' ) {
                    routeMap[i][j] = '^';
                }
                else {
                    passed[i][j].first = true;
                    passed[i][j].second = '<';
                    routeMap[i][j--] = '-';
                    routeMap[i][j] = '<';
                }
            }
            else {
                return false;
            }
        }
        else {
            cout << "Erro situation" << endl;
        }
        // if( x == 4 && y == 8 ) {
        //     disPlayMap( routeMap );
        // }
        // cout << endl;
    }
    return isCircle;
}
