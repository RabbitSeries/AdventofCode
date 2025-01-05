#include "KeyPadStructure.h"
#define BUF_SIZE 50
// +---+---+---+
// | 7 | 8 | 9 |
// +---+---+---+
// | 4 | 5 | 6 |
// +---+---+---+
// | 1 | 2 | 3 |
// +---+---+---+
//     | 0 | A |
//     +---+---+

// 780A 7->8->0->A
int getOnePath( char const s, char const t, map<char, vector<pair<char, char>>> const & keyPad ) {
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

// 029A A->0->2->9->A
// Ant the end of each path add a 'A';
// A->0:< A
// 0->2:^ A
// 2->9:>^^ A
// 9->A:vvv A
// <A^A>^^AvvvA

// <A^A>^^AvvvA
// <->A:>>^ A
vector<vector<char>>  getKeyPadAllPath( char const s, char const t, map<char, vector<pair<char, char>>> const& keyPad ) {
    // map<char, int> cost{ {'0',INT_MAX}, {'1',INT_MAX},{'2',INT_MAX},{'3',INT_MAX},{'4',INT_MAX},{'5',INT_MAX},{'6',INT_MAX},{'7',INT_MAX},{'8',INT_MAX},{'9',INT_MAX},{'A',INT_MAX} };
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
    FILE* input( fopen( "input.txt", "r" ) );
    // 126384
    // FILE* input( fopen( "example.txt", "r" ) );
    char buf[BUF_SIZE];
    while( fgets( buf, BUF_SIZE, input ) ) {
        string strbuf( buf );
        vector<char> row;
        for( auto c : strbuf ) {
            if( c != '\n' && c != '\0' ) {
                row.push_back( c );
            }
        }
        commandList.push_back( row );
    }
    fclose( input );
    return commandList;
}
// once used default param then the following param must all have default
// This is error.
// void findShortestCommand( vector<vector<vector<char>>> const  robot2CommList, vector<char>& curCommand, int depth = 0, int& res ) {
void findShortestCommand( vector<vector<vector<char>>> const& robot2CommList, vector<char>& curCommand, int depth, int& res ) {
    if( depth == robot2CommList.size() ) {
        int curLen = 0, i = 0;
        for( auto comm : curCommand ) {
            curLen += getOnePath( i == 0 ? 'A' : curCommand[i - 1], curCommand[i], DIRECTIONAL_KEYPAD );
            i++;
        }
        res = min( curLen, res );
        return;
    }
    for( int i = 0; i < robot2CommList[depth].size(); i++ ) {
        for( auto c : robot2CommList[depth][i] ) {
            curCommand.push_back( c );
        }
        findShortestCommand( robot2CommList, curCommand, depth + 1, res );
        for( auto c : robot2CommList[depth][i] ) {
            curCommand.pop_back();
        }
    }
}

int generateCommand( vector<char> const password ) {
    int humanCommLen = 0;
    for( int i = 0; i < password.size(); i++ ) {

        vector<vector<char>> robot1CommList = getKeyPadAllPath( i == 0 ? 'A' : password[i - 1], password[i], NUMERIC_KEYPAD );
        int res = INT_MAX;
        for( auto robot1Comm : robot1CommList ) {

            vector<vector<vector<char>>> robot2CommList;
            for( int j = 0; j < robot1Comm.size(); j++ ) {
                robot2CommList.push_back( getKeyPadAllPath( j == 0 ? 'A' : robot1Comm[j - 1], robot1Comm[j], DIRECTIONAL_KEYPAD ) );
            }

            vector<char> curComm;
            int curLen = INT_MAX;
            findShortestCommand( robot2CommList, curComm, 0, curLen );
            res = min( res, curLen );
        }

        humanCommLen += res;
    }
    return humanCommLen;
}

void Solution1() {
    // Robot0
    int res = 0;
    vector<vector<char>> passwordList = readPassword();
    for( auto password : passwordList ) {

        int manCommand = generateCommand( password );
        cout << "Password: " << string( password.begin(), password.end() )
            << " Complexity: " << stoi( string( password.begin(), password.end() - 1 ) ) * manCommand << endl;
        res += stoi( string( password.begin(), password.end() - 1 ) ) * manCommand;
    }
    cout << "sum of the complexities: " << res << endl;
}

int main() {
    Solution1();
    return 0;
}

// Construct a map of this numeric keypad
// map<int,vector<int>>
// For each vertex in an NUMERIC_KEYPAD, perform dijkstra.
// Get a path:
// 6546543216546A

//     +---+---+
//     | ^ | A |
// +---+---+---+
// | < | v | > |
// +---+---+---+

