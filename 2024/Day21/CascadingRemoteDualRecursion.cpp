#include "KeyPadStructure.h"
#define BUF_SIZE 50

// once used default param then the following param must all have default
// This is error.
// void findShortestCommand( vector<vector<vector<char>>> const  curRobotCommList, vector<char>& curCommand, int depth = 0, int& res ) {
void directionalCascadingCommand( vector<vector<vector<char>>> const& curRobotCommList, vector<char> curCommand, int depth, int robotCnt, ull& res ) {
    static map<tuple<vector<vector<vector<char>>>, vector<char>, int, int>, ull> cacheMap;
    if( cacheMap.find( tuple( curRobotCommList, curCommand, depth, robotCnt ) ) != cacheMap.end() ) {
        res = cacheMap[tuple( curRobotCommList, curCommand, depth, robotCnt )];
        return;
    }
    if( depth == curRobotCommList.size() ) {
        // cout << "Generated level " << robotCnt << " command list, size: " << depth << " CurCommand size: " << curCommand.size() << endl;
        // When recursively enters this level, one permutation is obtained.
        if( robotCnt != 1 ) {
            vector<vector<vector<char>>> nextRobotCommList;
            for( int j = 0; j < curCommand.size(); j++ ) {
                nextRobotCommList.push_back( getKeyPadAllPath( j == 0 ? 'A' : curCommand[j - 1], curCommand[j], DIRECTIONAL_KEYPAD ) );
            }
            vector<char> nextComm;
            directionalCascadingCommand( nextRobotCommList, nextComm, 0, robotCnt - 1, res );
            cacheMap[tuple( nextRobotCommList, nextComm, 0, robotCnt - 1 )] = res;
            return;
        }
        // The final human.
        ull curLen = 0, i = 0;
        for( auto comm : curCommand ) {
            curLen += getOnePath( i == 0 ? 'A' : curCommand[i - 1], curCommand[i], DIRECTIONAL_KEYPAD );
            i++;
        }
        res = min( curLen, res );
        cacheMap[tuple( curRobotCommList, curCommand, 0, robotCnt)] = res;
        return;
    }
    // Generate the permutation between each edge.
    for( int i = 0; i < curRobotCommList[depth].size(); i++ ) {
        for( auto c : curRobotCommList[depth][i] ) {
            curCommand.push_back( c );
        }
        // ! There is not need to perform global password optimization. Just perform optimization point to point.
        directionalCascadingCommand( curRobotCommList, curCommand, depth + 1, robotCnt, res );
        cacheMap[tuple( curRobotCommList, curCommand, depth + 1, robotCnt)] = res;
        for( auto c : curRobotCommList[depth][i] ) {
            curCommand.pop_back();
        }
    }
}

ull numericCommand( vector<char> const password, int robotCnt ) {
    ull res = 0;
    vector<vector<vector<char>>> robot1CommList;
    for( int i = 0; i < password.size(); i++ ) {
        robot1CommList.push_back( getKeyPadAllPath( i == 0 ? 'A' : password[i - 1], password[i], NUMERIC_KEYPAD ) );
    }
    robotCnt--;
    // The following 24 robot
    for( auto robot1Comm : robot1CommList ) {
        vector<vector<vector<char>>> nextComm{ robot1Comm };
        ull curLen = ULONG_LONG_MAX;
        directionalCascadingCommand( nextComm, vector<char>(), 0, robotCnt, curLen );
        res += curLen;
    }

    return res;
}

void Solution1() {

    int res = 0;
    vector<vector<char>> passwordList = readPassword();
    for( auto password : passwordList ) {

        int manCommand = numericCommand( password, 3 );
        cout << "Password: " << string( password.begin(), password.end() )
            << " Complexity: " << stoi( string( password.begin(), password.end() - 1 ) ) * manCommand << endl;
        res += stoi( string( password.begin(), password.end() - 1 ) ) * manCommand;
    }
    cout << "sum of the complexities: " << res << endl;
}
void Solution2() {

    ull res = 0;
    vector<vector<char>> passwordList = readPassword();
    for( auto password : passwordList ) {

        ull manCommand = numericCommand( password, 26 );
        cout << "Password: " << string( password.begin(), password.end() )
            << " Complexity: " << stoi( string( password.begin(), password.end() - 1 ) ) * manCommand << endl;
        res += stoi( string( password.begin(), password.end() - 1 ) ) * manCommand;
    }
    cout << "sum of the complexities: " << res << endl;
}

int main() {
    Solution1();
    Solution2();
    return 0;
}