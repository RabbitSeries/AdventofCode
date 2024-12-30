#include "bits/stdc++.h"
#define BUFFER_SIZE 1024
using namespace std;
typedef pair<int,int> point2D;
typedef struct problem{
    point2D a,b;
    point2D prize;
}problem;

int main() {
    FILE* input = fopen( "input.txt", "r" );
    char buf[BUFFER_SIZE + 1];
    int lineCnt = 0;
    vector<problem> problemSet;
    while( !feof( input ) && fgets( buf, BUFFER_SIZE + 1, input ) ) {
        stringstream line( buf );
        if( line.str().substr( 0, string( "Button " ).size() ) == "Button " ) {
            // Regex "^.*\+([0-9]+),.*\+([0-9]+)$"
            // Regex "\+([0-9]+).*\+([0-9]+)"
        }
    }
    return 0;

}