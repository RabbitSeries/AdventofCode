#include "iostream"
#include "algorithm"
#include "map"
#include "vector"
#include "stdlib.h"
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
int absAbstract( int num1, int num2 ) { return num1 >= num2 ? num1 - num2 : num2 - num1; }
int main() {
    FILE* input = fopen( "input.txt", "r" );
    vector<int> list1, list2;
    int distanceAddUp = 0;
    while( !feof( input ) ) {
        pair<int, int> tmp = readTwoNum( input );
        list1.push_back( tmp.first );
        list2.push_back( tmp.second );
    }
    sort( list1.begin(), list1.end(), less<int>() );
    sort( list2.begin(), list2.end(), less<int>() );
    for( int i = 0; i < list1.size(); i++ ) {
        distanceAddUp += absAbstract( list1[i], list2[i] );
    }
    cout << distanceAddUp;
    return 0;
}