#include <bits/stdc++.h>
using namespace std;
int main() {
    FILE* input( fopen( "EmptyFile.txt", "r" ) );
    char buf[1];
    while( fgets( buf, 1, input ) ) {
        int foo = 0;
        cout << sizeof( buf ) << endl;
    }
    return 0;
}