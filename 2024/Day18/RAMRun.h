#include <bits/stdc++.h>
using namespace std;
class RAMRun {

#define BUF_SIZE 100
#define SPACE 71

    enum cellStatus {
        good,
        bad
    };

    typedef pair<int, int> pos;

    const int dx[4]{ 0,0,1,-1 };
    const int dy[4]{ 1,-1,0,0 };

    vector<pos>  readSpaceStamp( int time, ifstream& input ) {
        int i = 0;
        vector<pos> bytePos;
        string buf;
        regex re( "[0-9]+" );
        while( i++ < time && getline( input, buf ) ) {
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
    
public:
    void Solution1() {
        vector<vector<cellStatus>> stamp( SPACE, vector<cellStatus>( SPACE, good ) );
        ifstream input( "Day18/input.txt" );
        vector<pos> bytePos = readSpaceStamp( 1024, input );
        for_each( bytePos.begin(), bytePos.end(), [ & ]( pos cur ) {
            stamp[cur.second][cur.first] = bad;
        } );
        int res = dijkstra( stamp );
        if( res )
            cout << "Solution 1: " << res << endl;
        else {
            cout << "Solution 1 failed." << endl;
            exit( -1 );
        }
    }


    void Solution2() {
        vector<vector<cellStatus>> stamp( SPACE, vector<cellStatus>( SPACE, good ) );
        ifstream input( "Day18/input.txt" );
        vector<pos> bytePos = readSpaceStamp( INT_MAX, input );
        int left = 0, right = bytePos.size() - 1;
        while( left < right ) {
            vector<vector<cellStatus>> curStamp = stamp;
            int mid = ( left + right ) / 2;
            for_each( bytePos.begin(), bytePos.begin() + mid + 1, [ & ]( pos cur ) {
                curStamp[cur.second][cur.first] = bad;
            } );
            if( dijkstra( curStamp ) ) {
                left = mid + 1;
            }
            else {
                right = mid;
            }
        }
        cout << "Solution 2: " << bytePos[left].first << "," << bytePos[left].second << endl;
    }
};
