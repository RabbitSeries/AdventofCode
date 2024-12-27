#include "bits/stdc++.h"
#define BUFFER_SIZE 1024
typedef long long ll;
using namespace std;

void appendFileBlock( int id, int size, vector<int>& disk ) {
    while( size-- ) {
        disk.push_back( id );
    }
}

void appendEmptyBlock( int size, vector<int>& disk ) {
    while( size-- ) {
        disk.push_back( -1 );
    }
}

ll diskCompact( vector<int>disk ) {
    ll checkSum = 0;
    int i = 0, j = disk.size() - 1;
    while( i < disk.size() && j>i ) {
        if( disk[i] == -1 ) {
            while( j > i ) {
                if( disk[j] == -1 ) j--;
                else break;
            }
            if( j > i ) {
                disk[i] = disk[j];
                disk[j] = -1;
                checkSum += disk[i] * i;
                i++;
            }
            else {
                break;
            }
        }
        else {
            checkSum += disk[i] * i;
            i++;
        }
    }
    return checkSum;
}

int main() {
    FILE* input = fopen( "input.txt", "r" );
    char linebuf[BUFFER_SIZE + 1] = { '\0' };
    int fileId = 0;
    vector<int> disk;
    bool isFile = true;
    while( !feof( input ) && fgets( linebuf, BUFFER_SIZE, input ) ) {
        string line( linebuf );
        for( int i = 0; i < line.size(); i++ ) {
            if( line[i] != '\n' && line[i] != '\0' ) {
                if( isFile ) {
                    appendFileBlock( fileId++, line[i] - '0', disk );
                    isFile = false;
                }
                else {
                    // atoi convets string to integer, but requires and end sign '\0' in the string;
                    appendEmptyBlock( line[i] - '0', disk );
                    isFile = true;
                }
            }
        }
    }
    cout << diskCompact( disk ) << endl;
}