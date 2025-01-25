#include <bits/stdc++.h>
using namespace std;
// // Todo: this algorithm's output has something to do with the difined BUFFER_SZIE, to solve the problem, change the algorithm to line process unit or figure out a way to concile BUFFER_SZIE and the string sent to next process.
// Now the buffer_size does not affect the result.
typedef long long ll;
int getNum( string str, int pos ) {
    int i = pos, num = 0;
    while( i < str.length() && isdigit( str[i] ) ) {
        num = num * 10 + stoi( string( 1, str[i] ) );
        i++;
    }
    return num;
}

// Reference
ll getLineResult( string& str ) {
    size_t nextPos = 0;
    ll res = 0;
    while( nextPos != string::npos && nextPos < str.length() ) {
        nextPos = str.find( "mul(", nextPos );
        if( nextPos != string::npos ) {
            nextPos = nextPos + 4;
        } else {
            break;
        }

        int a = 0, b = 0;
        if( nextPos < str.length() && isdigit( str[nextPos] ) ) {
            a = getNum( str, nextPos );
            nextPos += to_string( a ).size();
        } else {
            nextPos++;
            continue;
        }

        if( nextPos < str.length() && str[nextPos] == ',' ) {
            nextPos++;
        } else {
            nextPos++;
            continue;
        }

        if( nextPos < str.length() && isdigit( str[nextPos] ) ) {
            b = getNum( str, nextPos );
            nextPos += to_string( b ).size();
        } else {
            nextPos++;
            continue;
        }

        if( nextPos < str.length() && str[nextPos] == ')' ) {
            res += a * b;
            nextPos++;
            if( nextPos < str.length() ) {
                str = str.substr( nextPos );
                nextPos = 0;
            } else {
                str = "";
                nextPos = string::npos;
            }
        } else {
            nextPos++;
            continue;
        }
    }
    return res;
}

ll getLineResult( string& str, bool& enabled ) {
    ll zoneAdd = 0;
    if( enabled ) {
        size_t dontPos = str.find( "don't()" );
        if( dontPos != string::npos ) {
            string process = str.substr( 0, dontPos );
            zoneAdd += getLineResult( process );
            enabled = false;
            if( dontPos + string( "don't()" ).size() < str.length() ) {
                str = str.substr( dontPos + string( "don't()" ).size() );
                zoneAdd += getLineResult( str, enabled );
                return zoneAdd;
            } else {
                str = "";
                return zoneAdd;
            }
        } else {
            zoneAdd += getLineResult( str );
            return zoneAdd;
        }
    } else {
        size_t doPos = str.find( "do()" );
        if( doPos != string::npos ) {
            enabled = true;
            if( doPos + string( "do()" ).size() < str.length() ) {
                // cout << str.substr( 0, doPos + string( "do()" ).size() );
                // cout.flush();
                str = str.substr( doPos + string( "do()" ).size() );
                zoneAdd += getLineResult( str, enabled );
                return zoneAdd;
            } else {
                str = "";
                return zoneAdd;
            }
        } else {
            return zoneAdd;
        }
    }
    // return zoneAdd;
}

void Solution1() {
    FILE* input = fopen( "input.txt", "r" );
    char linebuffer[BUFSIZ];
    ll addUp = 0;
    string leftOver = "";
    while( fgets( linebuffer, BUFSIZ, input ) ) {
        string currentLine( linebuffer );
        if( !currentLine.empty() ) {
            currentLine = leftOver + currentLine;
            addUp += getLineResult( currentLine );
            leftOver = currentLine;
        }
    }
    cout << "Solution 1: " << addUp << endl;
}

void Solution2() {
    FILE* input = fopen( "input.txt", "r" );
    char linebuffer[BUFSIZ];
    ll addUp = 0;
    bool enabled = true;
    string leftOver = "";
    while( fgets( linebuffer, BUFSIZ, input ) ) {
        string currentLine( linebuffer );
        if( !currentLine.empty() ) {
            currentLine = leftOver + currentLine;
            addUp += getLineResult( currentLine, enabled );
            leftOver = currentLine;
        }
    }
    cout << "Solution 2: " << addUp << endl;
}

int main() {
    // FILE* input = fopen( "example.txt", "r" );
    Solution1();
    Solution2();
    return 0;
}