#include "string"
#include "iostream"
#include "map"
using namespace std;
// Todo: this algorithm's output has something to do with the difined BUFFER_SZIE, to solve the problem, change the algorithm to line process unit or figure out a way to concile BUFFER_SZIE and the string sent to next process.
#define BUFFER_SIZE 5000
int getNum( string& str ) {
    int i = 0, num = 0;
    while( i < str.length() && isdigit( str[i] ) ) {
        num = num * 10 + str[i] - '0';
        i++;
    }
    if( i < str.length() )
        str = str.substr( i );
    else str = "";
    return num;
}

int getZoneResult( string& str ) {
    if( str.find( "don't()" ) != string::npos )  cout << "error algorithm at " << str.find( "don't()" ) << "\n";
    size_t mulPos = str.find( "mul(" );
    long long res = 0;
    while( mulPos != string::npos && mulPos + 4 < str.length() ) {
        str = str.substr( mulPos + 4 );
        int a = 0, b = 0;
        bool flaga = false, flagb = false;
        if( !str.empty() && isdigit( str[0] ) ) {
            a = getNum( str );
            flaga = true;
        }
        if( str.length() >= 2 && str[0] == ',' && isdigit( str[1] ) ) {
            str = str.substr( 1 );
            b = getNum( str );
            flagb = true;
        }
        if( flaga && flagb && !str.empty() && str[0] == ')' ) {
            res += a * b;
            str = str.substr( 1 );
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
            enabled = false;
            string process = str.substr( 0, dontPos );
            zoneAdd += getZoneResult( process );
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
            }
            else {
                str = "";
                return 0;
            }
        }
        else {
            return 0;
        }
    }
    return zoneAdd;
}


int main() {
    FILE* input = fopen( "input.txt", "r" );
    char linebuffer[BUFFER_SIZE + 1] = { "\0" };
    long long addUp = 0;
    bool enabled = true;
    string leftOver = "";
    while( !feof( input ) && fgets( linebuffer, BUFFER_SIZE, input ) ) {
        string currentLine( linebuffer );
        currentLine = currentLine + leftOver;
        addUp += getLineResult( currentLine, enabled );
        if( *currentLine.end() != '\n' )
            leftOver = currentLine;
        else
            leftOver = "";
        // linebuffer[0] = '\0';
    }
    cout << addUp;
    return 0;
}