#include "KeyPadStructure.h"
#define BUF_SIZE 50

inline ull generatePermutations( vector<char> const& curComm, int robotCnt ) {
    // WTF?
    // Learned from <https://github.com/TrueBurn/advent-of-code/blob/main/2024/day-21/solution.py> this cache method is insane.
    static map<pair<vector<char>, int>, ull > cacheMap;
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
            curLen = min( generatePermutations( nextRobotComm, robotCnt - 1 ), curLen );
        }
        res += curLen;
    }
    cacheMap[{ curComm, robotCnt }] = res;
    return res;
}

ull generateCommand( vector<char> const password, int robotCnt ) {
    ull res = 0;
    for( int i = 0; i < password.size(); i++ ) {
        vector<vector<char>> nextRobotCommList = getKeyPadAllPath( i == 0 ? 'A' : password[i - 1], password[i], NUMERIC_KEYPAD );
        ull curLen = ULONG_LONG_MAX;
        for( auto nextRobotComm : nextRobotCommList ) {
            curLen = min( generatePermutations( nextRobotComm, robotCnt - 1 ), curLen );
        }
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
    cout << "Sum of the complexities: " << res << endl;
}
void Solution2() {
    ull res = 0;
    vector<vector<char>> passwordList = readPassword();

    for( auto password : passwordList ) {
        ull manCommand = generateCommand( password, 26 );
        cout << "Password: " << string( password.begin(), password.end() )
            << " Complexity: " << stoi( string( password.begin(), password.end() - 1 ) ) * manCommand << endl;
        res += stoll( string( password.begin(), password.end() - 1 ) ) * manCommand;
    }
    cout << "Sum of the complexities: " << res << endl;
}

int main() {
    Solution1();
    Solution2();
    return 0;
}