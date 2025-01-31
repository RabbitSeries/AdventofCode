#include "KeyPadStructure.h"
#define BUF_SIZE 50

static map<pair<vector<char>, int>, ull > cacheMap;
inline ull directionalCascadingCommand( vector<char> const& curComm, int robotCnt ) {
    // WTF?
    // Learned from <https://github.com/TrueBurn/advent-of-code/blob/main/2024/day-21/solution.py> this cache method is insane.
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

void Solution1() {

    int res = 0;
    vector<vector<char>> passwordList = readPassword();
    for( auto password : passwordList ) {
        int manCommand = numericCommand( password, 3 );
        cout << "Password: " << string( password.begin(), password.end() )
            << " Complexity: " << stoi( string( password.begin(), password.end() - 1 ) ) * manCommand << endl;
        res += stoi( string( password.begin(), password.end() - 1 ) ) * manCommand;
    }
    cout << "Sum of the complexities: " << res << endl;
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
    cout << "Sum of the complexities: " << res << endl;
    cout << "Cache size: " << cacheMap.size() << endl;
}

int main() {
    auto now = chrono::high_resolution_clock::now();
    Solution1();
    Solution2();
    auto end = chrono::high_resolution_clock::now();
    // 56.983 ms
    cout << chrono::duration_cast<chrono::microseconds>( end - now ).count() / 1000.0 << endl;
    return 0;
}