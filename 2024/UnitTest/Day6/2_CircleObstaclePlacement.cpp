#include "../../Day6/2_CirclePatrol.h"
/**
 * @brief Copied from <../../Day6/2_CircleObstacle.cpp> and add trial param
 * @param i
 * @param j
 * @param routeMap
 * @param x
 * @param y
 * @param rowSize
 * @param colSize
 * @param trial
 * @return
 */
bool isPatrolCircle( int i, int j, vector<vector<char>> routeMap, int x, int y, int rowSize, int colSize, int& times ) {
    routeMap[x][y] = '#';
    bool isCircle = false;
    vector<vector<pair<bool, char>>> passed( rowSize, vector<pair<bool, char>>( colSize, pair<bool, char>( false, '.' ) ) );
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
    int distinctPlacement = 0;
    for( int i = 0; i < routeMap.size(); i++ ) {
        // Linux IO buff works differently with Windows, only endl will signal the flush command.
        for( int j = 0; j < routeMap[i].size(); j++ ) {
            int trial = 0;
            if( i == guardPos.first && j == guardPos.second ) {
                continue;
            }
            if( isPatrolCircle( guardPos.first, guardPos.second, routeMap, i, j, routeMap.size(), routeMap[0].size(), trial ) ) {
                distinctPlacement++;
            }
            cout << "Checking line " << i << " column: " << j << " " << "tial: " << trial << endl;
        }
        // cout << endl;
    }
    cout << "The distinct placement that makes the patrol in a circle counts: " << distinctPlacement << endl;
    return 0;
}

