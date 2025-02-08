#include "KeyPadStructure.h"

class CascadingRemote {

    typedef unsigned long long ull;

    unordered_map<pair<vector<char>, int>, ull > cacheMap;

    ull directionalCascadingCommand( vector<char> const& curComm, int robotCnt ) {

        if( cacheMap.find( { curComm,robotCnt } ) != cacheMap.end() ) {
            return cacheMap[{ curComm, robotCnt }];
        }
        if( robotCnt == 1 ) {
            // cout << "Generated level " << robotCnt << " command list, size: " << depth << " CurCommand size: " << curCommand.size() << endl;
            // The final human's command to the last robot.
            ull curLen = 0, i = 0;
            while( i < curComm.size() ) {
                curLen += getOnePath( i == 0 ? 'A' : curComm[i - 1], curComm[i], DIRECTIONAL_KEYPAD );
                i++;
            }
            cacheMap[{curComm, robotCnt}] = curLen;
            return curLen;
        }
        ull res = 0;
        for( int i = 0; i < curComm.size(); i++ ) {
            vector<vector<char>> nextRobotCommList = getKeyPadAllPath( i == 0 ? 'A' : curComm[i - 1], curComm[i], DIRECTIONAL_KEYPAD );
            ull curLen = ULONG_LONG_MAX;
            for( auto nextRobotComm : nextRobotCommList ) {
                curLen = min( directionalCascadingCommand( nextRobotComm, robotCnt - 1 ), curLen );
            }
            res += curLen;
        }
        cacheMap[{ curComm, robotCnt }] = res;
        return res;
    }

    ull numericCommand( vector<char> const password, int robotCnt ) {
        ull res = 0;
        for( int i = 0; i < password.size(); i++ ) {
            vector<vector<char>> nextRobotCommList = getKeyPadAllPath( i == 0 ? 'A' : password[i - 1], password[i], NUMERIC_KEYPAD );
            ull curLen = ULONG_LONG_MAX;
            for( auto nextRobotComm : nextRobotCommList ) {
                curLen = min( directionalCascadingCommand( nextRobotComm, robotCnt - 1 ), curLen );
            }
            res += curLen;
        }

        return res;
    }

    int getOnePath( char const s, char const t, unordered_map<char, vector<pair<char, char>>> const& keyPad ) {
        map<char, int> cost;
        // map<char, bool> visited;
        for( auto [key, nextKeyList] : keyPad ) {
            cost[key] = INT_MAX;
            // visited[key] = false;
        }
        struct point {
            point() {};
            point( char c ) : curKey( c ) {};
            point( char c, char direction ) : curKey( c ), curDirection( direction ) {};
            char curKey;
            char curDirection;
            vector<char> linkRoad;
            // Must use const qualifier
            // bool operator<( point & b ) {
            operator char() const {
                return curKey;
            }
            bool operator<( point b ) const {
                return this->curKey < b.curKey;
            }
        };
        point pointInitTest{ 1,1 };
        assert( pointInitTest.curKey == 1 );
        priority_queue<pair<int, point>, vector<pair<int, point>>, greater<>> pq;
        pq.push( { 0,{s,0} } );
        cost[s] = 0;
        while( !pq.empty() ) {
            auto [curCost, curPoint] = pq.top();
            char curKey = curPoint.curKey;
            char curDirection = curPoint.curDirection;
            pq.pop();
            // Exlcude same cost path and optimized key.
            if( /**visited.at( curKey )*/ curCost > cost.at( curKey ) ) {
                continue;
            }
            // Path process
            // visited[curKey] = true;
            if( curKey != s ) {
                curPoint.linkRoad.push_back( curDirection );
            }
            // Output process
            if( curKey == t ) {
                curPoint.linkRoad.push_back( 'A' );
                return curPoint.linkRoad.size();
            }
            // Proceed process
            for( auto [nextKey, nextDirection] : keyPad.at( curKey ) ) {
                // If there is not ==, then there is no need to add visited array, because same cost path is excluded due to this judgement.
                if( curCost + 1 < cost.at( nextKey ) ) {
                    point nextPoint( nextKey, nextDirection );
                    cost[nextKey] = curCost + 1;
                    nextPoint.linkRoad = curPoint.linkRoad;
                    pq.push( { curCost + 1,nextPoint } );
                }
            }
        }
        return  0;
    }

    vector<vector<char>>  getKeyPadAllPath( char const s, char const t, unordered_map<char, vector<pair<char, char>>> const& keyPad ) {
        map<char, int> cost;
        for( auto [key, nextKeyList] : keyPad ) {
            cost[key] = INT_MAX;
        }
        struct point {
            point() {};
            point( char c, char comm ) : curKey( c ), curCommand( comm ) {};
            char curKey;
            char curCommand;
            vector<vector<char>> linkRoad;
            // Must use const qualifier
            // bool operator<( point & b ) {
            bool operator<( point b ) const {
                return this->curKey < b.curKey;
            }
        };
        priority_queue<pair<int, point>, vector<pair<int, point>>, greater<>> pq;
        pq.push( { 0,point( s,0 ) } );
        cost[s] = 0;

        point endPoint( t, 0 );

        while( !pq.empty() ) {
            auto [curCost, curPoint] = pq.top();
            char curKey = curPoint.curKey;
            char curCommand = curPoint.curCommand;
            pq.pop();
            // Exlcude same cost path, optimized key.
            if( curCost > cost.at( curKey ) ) {
                continue;
            }
            // Path process
            if( curKey != s ) {
                if( !curPoint.linkRoad.empty() )
                    for( auto& path : curPoint.linkRoad )
                        path.push_back( curCommand );
                else {
                    curPoint.linkRoad.push_back( { {curCommand,}, } );
                }
            }
            // Output process
            if( curKey == t ) {
                for( auto road : curPoint.linkRoad ) {
                    endPoint.linkRoad.push_back( road );
                }
            }
            // Proceed process
            for( auto [nextKey, nextDirection] : keyPad.at( curKey ) ) {
                point nextPoint( nextKey, nextDirection );
                if( curCost + 1 < cost.at( nextKey ) ) {
                    nextPoint.linkRoad = curPoint.linkRoad;
                    cost[curKey] = curCost + 1;
                    pq.push( { curCost + 1,nextPoint } );
                }
                else if( curCost + 1 == cost.at( nextKey ) ) {
                    for( auto& path : curPoint.linkRoad )
                        nextPoint.linkRoad.push_back( path );
                    pq.push( { curCost + 1,nextPoint } );
                }
            }
        }
        if( endPoint.linkRoad.empty() ) {
            endPoint.linkRoad.push_back( { { 'A' , }, } );
        }
        else {
            for( auto& road : endPoint.linkRoad ) {
                road.push_back( 'A' );
            }
        }
        return endPoint.linkRoad;
    }

    vector<vector<char>> readPassword() {
        vector<vector<char>> commandList;
        ifstream input( "Day21/input.txt" );
        // 126384
        // FILE* input( fopen( "example.txt", "r" ) );
        string buf;
        while( getline( input, buf ) ) {
            string strbuf( buf );
            vector<char> row;
            for( auto c : strbuf ) {
                if( c != '\n' && c != '\0' ) {
                    row.push_back( c );
                }
            }
            commandList.push_back( row );
        }
        input.close();
        return commandList;
    }

public:
    void Solution1() {

        int res = 0;
        vector<vector<char>> passwordList = readPassword();
        for( auto password : passwordList ) {
            int manCommand = numericCommand( password, 3 );
            cout << "Password: " << string( password.begin(), password.end() )
                << " Complexity: " << stoi( string( password.begin(), password.end() - 1 ) ) * manCommand << endl;
            res += stoi( string( password.begin(), password.end() - 1 ) ) * manCommand;
        }
        cout << "Solution 1: " << res << endl;
        cout << "Cache size: " << cacheMap.size() << endl;
    }
    void Solution2() {
        ull res = 0;
        vector<vector<char>> passwordList = readPassword();

        for( auto password : passwordList ) {
            ull manCommand = numericCommand( password, 26 );
            cout << "Password: " << string( password.begin(), password.end() )
                << " Complexity: " << stoi( string( password.begin(), password.end() - 1 ) ) * manCommand << endl;
            res += stoll( string( password.begin(), password.end() - 1 ) ) * manCommand;
        }
        cout << "Solution 2: " << res << endl;
        cout << "Cache size: " << cacheMap.size() << endl;
    }
};

