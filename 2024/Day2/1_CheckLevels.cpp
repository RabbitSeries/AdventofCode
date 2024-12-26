#include "algorithm"
#include "vector"
#include "iostream"
#include "sstream"
using namespace std;
bool AnalyseLine( FILE* input ) {
    char cline[1 << 10 + 1] = { '\0' };
    // If fgets read a empty line, it won't set cline to "\0", thus remain ramdom value (if cline is two big, there is a chance that cline is allocated at a rather later position of the stack, and the space is not realocated to other usage, and because of which cline may also be reallocated to the same position because there might be such long free space ahead ) for cline.
    // Always innitialize your datasets.
    if( !fgets( cline, 1 << 10, input ) )
        // return false
        ;
    string line( cline );
    stringstream lineInput;
    lineInput.clear();
    lineInput << line;
    int tmpLevel = 0;
    vector<int> Levels;
    while( lineInput >> tmpLevel ) {
        Levels.push_back( tmpLevel );
    }
    // Line check
    if( Levels.empty() ) return false;
    int i = 0;
    if( Levels[0] > Levels[1] ) {
        while( i < Levels.size() - 1 && Levels[i]>Levels[i + 1] && Levels[i] - Levels[i + 1] <= 3 ) {
            i++;
        }
    }
    else if( Levels[0] < Levels[1] ) {
        while( i < Levels.size() - 1 && Levels[i] < Levels[i + 1] && Levels[i + 1] - Levels[i] <= 3 ) {
            i++;
        }
    }
    if( i == Levels.size() - 1 ) return true;
    return false;
}

int main() {
    FILE* input = fopen( "input.txt", "r" );
    int cnt = 0;
    while( !feof( input ) ) {
        if( AnalyseLine( input ) ) {
            cnt++;
        }
    }
    cout << cnt;
    return 0;
}