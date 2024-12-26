#include "string"
#include "iostream"
#include "map"
using namespace std;

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

int getLineResult( string str ) {
    int res = 0;
    int mulPos = str.find( "mul(" );
    // for one of the characters of __s within this string.
    // int mulPos = str.find_first_of( "mul(" );
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
        }
        mulPos = str.find( "mul(" );
    }
    return res;
}

int main() {
    FILE* input = fopen( "input.txt", "r" );
    char linebuffer[( 1 << 10 ) + 1] = { "\0" };
    long long addUp = 0;
    while( !feof( input ) && fgets( linebuffer, 1 << 15, input ) ) {
        addUp += getLineResult( string( linebuffer ) );
    }
    cout << addUp;
    return 0;
}