#include <bits/stdc++.h>
using namespace std;

#define BUF_SIZE 100
#define SPACE 71

enum cellStatus {
    good,
    bad
};

typedef pair<int, int> pos;

const int dx[4]{ 0,0,1,-1 };
const int dy[4]{ 1,-1,0,0 };

vector<pos>  readSpaceStamp( int time, FILE* input ) {
    int i = 0;
    vector<pos> bytePos;
    char buf[BUF_SIZE];
    regex re( "[0-9]+" );
    while( i++ < time && fgets( buf, BUF_SIZE, input ) ) {
        string bufStr( buf );
        sregex_iterator it( bufStr.begin(), bufStr.end(), re ), end_it;
        int x, y;
        if( distance( it, end_it ) == 2 ) {
            //          |
            //          |
            //          |
            //          |
            //          V y
            // ---------> X
            x = stoi( ( *( it++ ) ).str() );
            y = stoi( ( *( it++ ) ).str() );
            bytePos.push_back( { x,y } );
        }
    }
    return bytePos;
}


bool isValid( pos const coordinate, vector<vector<cellStatus>> const stamp ) {
    int x = coordinate.first, y = coordinate.second;
    return x >= 0 && x < SPACE && y >= 0 && y < SPACE && stamp[y][x] == good;
}

int dijkstra( vector<vector<cellStatus>> stamp ) {
    vector<vector<int>> step( SPACE, vector<int>( SPACE, INT_MAX ) );
    priority_queue<pair<int, pos>, vector<pair<int, pos>>, greater<>> pq;
    step[0][0] = 0;
    pq.push( { 0,{0,0} } );
    while( !pq.empty() ) {
        auto [curCost, curPos] = pq.top();
        pq.pop();
        if( curCost > step[curPos.second][curPos.first] ) {
            continue;
        }
        if( curPos == pos( SPACE - 1, SPACE - 1 ) ) {
            return curCost;
        }
        for( int i = 0; i < 4; i++ ) {
            pos nextPos = { curPos.first + dx[i],curPos.second + dy[i] };
            if( isValid( nextPos, stamp ) && curCost + 1 < step[nextPos.second][nextPos.first] ) {
                step[nextPos.second][nextPos.first] = curCost + 1;
                pq.push( { curCost + 1,nextPos } );
            }
        }
    }
    return 0;
}

int countCorrupted( vector<vector<cellStatus>> stamp ) {
    int cnt = 0;
    for( auto row : stamp ) {
        for( auto col : row ) {
            if( col == bad ) {
                cnt++;
            }
        }
    }
    return cnt;
}

void Solution1( vector<vector<cellStatus>> stamp, FILE* input ) {
    vector<pos> bytePos = readSpaceStamp( 1024, input );
    for_each( bytePos.begin(), bytePos.end(), [ & ]( pos cur ) {
        stamp[cur.second][cur.first] = bad;
    } );
    if( dijkstra( stamp ) )
        cout << "Lowest cost: " << dijkstra( stamp ) << endl;
    else
        cout << "Path not found" << endl;
}


void Solution2( vector<vector<cellStatus>> stamp, FILE* input ) {
    vector<pos> bytePos = readSpaceStamp( INT_MAX, input );
    int left = 0, right = bytePos.size() - 1;
    while( left < right ) {
        vector<vector<cellStatus>> curStamp = stamp;
        int mid = ( left + right ) / 2;
        for_each( bytePos.begin(), bytePos.begin() + mid + 1, [ & ]( pos cur ) {
            curStamp[cur.second][cur.first] = bad;
        } );
        cout << "Search corrupted: " << countCorrupted( curStamp ) << endl;
        if( dijkstra( curStamp ) ) {
            // Search right half;
            // If not satisfied, position mid should be excluded.
            // Next search in (mid,right].
            left = mid + 1;
        }
        else {
            // Search left half;
            // If satisfied, position mid should not be excluded.
            // Next search in [left,mid].
            right = mid;
        }
    }
    assert( left == right );
    cout << "Found last byte at index:" << left << endl;
    vector<vector<cellStatus>> curStamp = stamp;
    for_each( bytePos.begin(), bytePos.begin() + left, [ & ]( pos cur ) {
        curStamp[cur.second][cur.first] = bad;
    } );
    assert( dijkstra( curStamp ) != 0 );
    curStamp = stamp;
    for_each( bytePos.begin(), bytePos.begin() + left + 1, [ & ]( pos cur ) {
        curStamp[cur.second][cur.first] = bad;
    } );
    assert( dijkstra( curStamp ) == 0 );
    cout << "Last byte: " << bytePos[left].first << "," << bytePos[left].second << endl;
}

int main() {
    FILE* input( fopen( "input.txt", "r" ) );
    // FILE* input( fopen( "example.txt", "r" ) );
    vector<vector<cellStatus>> corruptedStamp( SPACE, vector<cellStatus>( SPACE, good ) );
    Solution1( corruptedStamp, input );
    fclose( input );
    input = fopen( "input.txt", "r" );
    // input = fopen( "example.txt", "r" );
    Solution2( corruptedStamp, input );
    fclose( input );
    return 0;
}