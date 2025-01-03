#include <bits/stdc++.h>
using namespace std;
// Todo: this algorithm's output has something to do with the difined BUFFER_SZIE, to solve the problem, change the algorithm to line process unit or figure out a way to concile BUFFER_SZIE and the string sent to next process.
#define BUFFER_SIZE 4000
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
    size_t mulPos = str.find( "mul(" );
    ll res = 0;
    int nextPos = 0;
    while( mulPos != string::npos && mulPos + 4 < str.length() ) {
        // Not trimed yet.
        int a = 0, b = 0;
        nextPos = mulPos + 4;
        if( nextPos < str.length() ) {
            if( isdigit( str[nextPos] ) ) {
                a = getNum( str, nextPos );
                nextPos += to_string( a ).size();
            }
            else {
                return res;
            }
        }
        if( nextPos < str.length() ) {
            if( str[nextPos] == ',' )
                nextPos++;
            else
                return res;
        }
        if( nextPos < str.length() ) {
            if( isdigit( str[nextPos] ) ) {
                b = getNum( str, nextPos );
                nextPos += to_string( b ).size();
            }
            else {
                return res;
            }
        }
        if( nextPos < str.length() ) {
            if( str[nextPos] == ')' ) {
                res += a * b;
                nextPos++;
                if( nextPos < str.length() )
                    str = str.substr( nextPos );
                else
                    str = "";
            }
            else {
                return res;
            }
        }
        if( !str.empty() )
            mulPos = str.find( "mul(" );
        else
            mulPos = string::npos;
    }
    return res;
}

int getLineResult( string& str, bool& enabled ) {
    int zoneAdd = 0;
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
    string LinkBuf = "";
    while( fgets( linebuffer, BUFFER_SIZE, input ) ) {
        string currentLine( linebuffer );
        if( !currentLine.empty() ) {
            if( currentLine.back() != '\n' )
                LinkBuf = currentLine;
            else
                LinkBuf = currentLine.substr( 0, currentLine.size() - 1 );
            addUp = +getLineResult( LinkBuf, enabled );
            enabled = true;
        }
        // linebuffer[0] = '\0';
    }

    cout << addUp << endl;
    return 0;
}