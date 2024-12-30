#include "2_CirclePatrol.h"

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
        cout << "Checking line  " << i << endl;
        for( int j = 0; j < routeMap[i].size(); j++ ) {
            if( i == guardPos.first && j == guardPos.second ) {
                continue;
            }
            if( isPatrolCircle( guardPos.first, guardPos.second, routeMap, i, j, routeMap.size(), routeMap[0].size() ) ) {
                distinctPlacement++;
            }
        }
        // cout << endl;
    }
    cout << "The distinct placement that makes the patrol in a circle counts: " << distinctPlacement << endl;
    return 0;
}

