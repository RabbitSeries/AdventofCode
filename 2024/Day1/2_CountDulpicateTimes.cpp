#include "iostream"
#include "vector"
#include "algorithm"
using namespace std;
pair<int, int>  readTwoNum( FILE* input ) {
    int num1 = 0, num2 = 0;
    if( fscanf( input, "%d%d", &num1, &num2 ) == 2 ) {
        fgetc( input );
        return pair<int, int>( num1, num2 );
    }
    else {
        fgetc( input );
        return pair<int, int>( 0, 0 );
    }
}
int main() {
    FILE* input = fopen( "input.txt", "r" );
    vector<int> list1, list2( 100000, 0 );
    while( !feof( input ) ) {
        pair<int, int> tmp = readTwoNum( input );
        list1.push_back( tmp.first );
        list2[tmp.second]++;
    }
    unsigned long long tmAddup = 0;
    for_each( list1.begin(), list1.end(), [ & ]( int index ) {
        tmAddup += list2[index] * index;
    } );
    cout << tmAddup;
    return 0;
}