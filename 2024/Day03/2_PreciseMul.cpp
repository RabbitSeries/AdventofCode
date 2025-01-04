#include <bits/stdc++.h>
using namespace std;
// // Todo: this algorithm's output has something to do with the difined BUFFER_SZIE, to solve the problem, change the algorithm to line process unit or figure out a way to concile BUFFER_SZIE and the string sent to next process.
// Now the buffer_size does not affect the result.
#define BUFFER_SIZE 100
typedef long long ll;
int getNum( string str, int pos ) {
    int i = pos, num = 0;
    while( i < str.length() && isdigit( str[i] ) ) {
        num = num * 10 + stoi( string( 1, str[i] ) );
        i++;
    }
    return num;
}

// Will trip all available matched multiply result.
ll getZoneResult( string& str ) {
    if( str.find( "don't()" ) != string::npos )  cout << "error algorithm at " << str.find( "don't()" ) << "\n";
    size_t nextPos = 0;
    ll res = 0;
    while( nextPos != string::npos && nextPos < str.length() ) {
        // Remember to find "mul(" from next position.
        nextPos = str.find( "mul(", nextPos );
        if( nextPos != string::npos ) {
            // Not trimed yet.
            int a = 0, b = 0;
            nextPos = nextPos + 4;
            if( nextPos < str.length() ) {
                if( isdigit( str[nextPos] ) ) {
                    a = getNum( str, nextPos );
                    nextPos += to_string( a ).size();
                }
                else {
                    continue;
                }
            }
            if( nextPos < str.length() ) {
                if( str[nextPos] == ',' )
                    nextPos++;
                else {
                    continue;
                }
            }
            if( nextPos < str.length() ) {
                if( isdigit( str[nextPos] ) ) {
                    b = getNum( str, nextPos );
                    nextPos += to_string( b ).size();
                }
                else {
                    continue;
                }
            }
            if( nextPos < str.length() ) {
                if( str[nextPos] == ')' ) {
                    res += a * b;
                    nextPos++;
                    cout << "mul(" << a << "," << b << ")" << endl;
                    if( nextPos < str.length() ) {
                        str = str.substr( nextPos );
                        // Once trimed the string, remember to reset next search position.
                        nextPos = 0;
                    }
                    else {
                        nextPos = 0;
                        str = "";
                    }
                }
                else {
                    continue;
                }
            }
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
            zoneAdd += getZoneResult( process );
            enabled = false;
            if( dontPos + string( "don't()" ).size() < str.length() ) {
                str = str.substr( dontPos + string( "don't()" ).size() );
                zoneAdd += getLineResult( str, enabled );
                return zoneAdd;
            }
            else {
                str = "";
                return zoneAdd;
            }
        }
        else {
            zoneAdd += getZoneResult( str );
            return zoneAdd;
        }
    }
    else {
        size_t doPos = str.find( "do()" );
        if( doPos != string::npos ) {
            enabled = true;
            if( doPos + string( "do()" ).size() < str.length() ) {
                // cout << str.substr( 0, doPos + string( "do()" ).size() );
                // cout.flush();
                str = str.substr( doPos + string( "do()" ).size() );
                zoneAdd += getLineResult( str, enabled );
                return zoneAdd;
            }
            else {
                str = "";
                return zoneAdd;
            }
        }
        else {
            return zoneAdd;
        }
    }
    return zoneAdd;
}


int main() {
    // FILE* input = fopen( "example.txt", "r" );
    FILE* input = fopen( "input.txt", "r" );
    char linebuffer[BUFFER_SIZE] = { '\0' };
    ll addUp = 0;
    bool enabled = true;
    string leftOver = "";
    while( fgets( linebuffer, BUFFER_SIZE, input ) ) {
        string currentLine( linebuffer );
        if( !currentLine.empty() ) {
            currentLine = leftOver + currentLine;
            addUp += getLineResult( currentLine, enabled );
            leftOver = currentLine;
        }
    }
    cout << addUp << endl;
    return 0;
}