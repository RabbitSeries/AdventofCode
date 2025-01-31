#include <bits/stdc++.h>
using namespace std;
#define BUF_SIZE 50
typedef unsigned long long ull;

unordered_map<char, vector<pair<char, char>>> NUMERIC_KEYPAD{
    // map<char, vector<pair<char, char>>> NUMERIC_KEYPAD{
        //     +---+
        //     | 3 |
        // +---+---+
        // | 0 | A |
        // +---+---+
        {'A',{
                        {'3','^'},
                {'0','<'}
            }
        },
    // +---+
    // | 2 |
    // +---+---+
    // | 0 | A |
    // +---+---+
    {'0',
        {
                    {'2','^'},
                            {'A','>'}
        }
    },
    // +---+
    // | 4 |
    // +---+---+
    // | 1 | 2 |
    // +---+---+
    {'1',
        {
                    {'4','^'},
                            {'2','>'}
        }
    },
    {'2',
        {
                    {'5','^'},
            {'1','<'},
                            {'3','>'},
                    {'0','v'}
        }
    },
    {'3',
        {
                    {'6','^'},
            {'2','<'},
                    {'A','v'},
        }
    },
    {'4',
        {
                    {'7','^'},
                            {'5','>'},
                    {'1','v'},
        }
    },
    {'5',
        {
                    {'8','^'},
            {'4','<'},
                            {'6','>'},
                    {'2','v'},
        }
    },
    {'6',
        {
                    {'9','^'},
            {'5','<'},
                    {'3','v'},
        }
    },
    {'7',
        {
                            {'8','>'},
                    {'4','v'},
        }
    },
    {'8',
        {
            {'7','<'},
                            {'9','>'},
                    {'5','v'},
        }
    },
    {'9',
        {
            {'8','<'},
                    {'6','v'},
        }
    },
};


unordered_map<char, vector<pair<char, char>>> DIRECTIONAL_KEYPAD{
    // map<char, vector<pair<char, char>>> DIRECTIONAL_KEYPAD{
        {'A',
            {
                {'^','<'},
                        {'>','v'},
            }
        },
        {'^',
            {
                        {'A','>'},
                {'v','v'},
            }
        },
        {'v',
            {
                        {'^','^'},
                {'<','<'},
                                {'>','>'},
            }
        },
        {'<',
            {
                                {'v','>'},
            }
        },
        {'>',
            {
                        {'A','^'},
                {'v','<'},
            }
        }
    //     +---+---+
    //     | ^ | A |
    // +---+---+---+
    // | < | v | > |
    // +---+---+---+
};

//     +---+---+
//     | ^ | A |
// +---+---+---+
// | < | v | > |
// +---+---+---+

// My key pad
//     +---+---+
//     | ^ | A |
// +---+---+---+
// | < | v | > |
// +---+---+---+

inline int getOnePath( char const s, char const t, unordered_map<char, vector<pair<char, char>>> const& keyPad ) {
    // inline int getOnePath( char const s, char const t, map<char, vector<pair<char, char>>> const& keyPad ) {
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

// inline vector<vector<char>>  getKeyPadAllPath( char const s, char const t, map<char, vector<pair<char, char>>> const& keyPad ) {
inline vector<vector<char>>  getKeyPadAllPath( char const s, char const t, unordered_map<char, vector<pair<char, char>>> const& keyPad ) {
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
