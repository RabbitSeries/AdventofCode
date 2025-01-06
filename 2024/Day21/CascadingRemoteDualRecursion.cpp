#include "KeyPadStructure.h"
#define BUF_SIZE 50

// once used default param then the following param must all have default
// This is error.
// void findShortestCommand( vector<vector<vector<char>>> const  curRobotCommList, vector<char>& curCommand, int depth = 0, int& res ) {
void generatePermutations( vector<vector<vector<char>>> const& curRobotCommList, vector<char> curCommand, int depth, int robotCnt, ull& res ) {
    if( depth == curRobotCommList.size() ) {
        // cout << "Generated level " << robotCnt << " command list, size: " << depth << " CurCommand size: " << curCommand.size() << endl;
        // When recursively enters this level, one permutation is obtained.
        if( robotCnt != 1 ) {
            vector<vector<vector<char>>> nextRobotCommList;
            for( int j = 0; j < curCommand.size(); j++ ) {
                nextRobotCommList.push_back( getKeyPadAllPath( j == 0 ? 'A' : curCommand[j - 1], curCommand[j], DIRECTIONAL_KEYPAD ) );
            }
            vector<char> nextComm;
            generatePermutations( nextRobotCommList, nextComm, 0, robotCnt - 1, res );
            return;
        }
        // The final human.
        ull curLen = 0, i = 0;
        for( auto comm : curCommand ) {
            curLen += getOnePath( i == 0 ? 'A' : curCommand[i - 1], curCommand[i], DIRECTIONAL_KEYPAD );
            i++;
        }
        res = min( curLen, res );
        return;
    }
    // Generate the permutation between each edge.
    for( int i = 0; i < curRobotCommList[depth].size(); i++ ) {
        for( auto c : curRobotCommList[depth][i] ) {
            curCommand.push_back( c );
        }
        generatePermutations( curRobotCommList, curCommand, depth + 1, robotCnt, res );
        for( auto c : curRobotCommList[depth][i] ) {
            curCommand.pop_back();
        }
    }
}

ull generateCommand( vector<char> const password, int robotCnt ) {
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
        generatePermutations( nextComm, vector<char>(), 0, robotCnt, curLen );
        res += curLen;
    }

    return res;
}

void Solution1() {

    int res = 0;
    vector<vector<char>> passwordList = readPassword();
    for( auto password : passwordList ) {

        int manCommand = generateCommand( password, 3 );
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

        ull manCommand = generateCommand( password, 25 );
        cout << "Password: " << string( password.begin(), password.end() )
            << " Complexity: " << stoi( string( password.begin(), password.end() - 1 ) ) * manCommand << endl;
        res += stoi( string( password.begin(), password.end() - 1 ) ) * manCommand;
    }
    cout << "sum of the complexities: " << res << endl;
}

int main() {
    Solution1();
    // Solution2();
    return 0;
}