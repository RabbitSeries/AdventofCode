#include "bits/stdc++.h"
#define BUFFER_SIZE 5
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

void printDisk( const vector<int>disk, const string path ) {
    // This won't work
    // FILE* output = fopen( path.c_str(), "rw" );
    int cnt = 0;
    FILE* output = fopen( path.c_str(), "w" );
    for( int diskId = 0; diskId < disk.size(); diskId++ ) {
        // operands to ‘?:’ have different types ‘const char*’ and ‘__gnu_cxx::__alloc_traits<std::allocator<int>, int>::value_type’ {aka ‘int’}gcc
        // cout << ( disk[diskId] == -1 ? "." : disk[diskId] );
        if( disk[diskId] == -1 ) {
            // cout << ".";
            fprintf( output, "_" );
        }
        else {
            // stringstream ss;
            // ss << disk[diskId];
            // string outs = ss.str();
            fprintf( output, "%s", "X" );
            // fprintf(output,"%c",disk[diskId]);
            // cout << disk[diskId];
        }
        // cnt = (cnt+1)%BUFFER_SIZE;
        // if( !cnt )
        //     fprintf( output, "\n");
    }
    fclose( output );
    // cout << endl;
}

void scanFreeSpace( vector<int> disk, vector<pair<int, int>>& freeSpaceTable ) {

}

ll fileCompack( vector<int>disk, const vector<int> fileSizeTable, vector<pair<int, int>>& freeSpaceTable ) {
    printDisk( disk, "original.txt" );
    int denseHead = freeSpaceTable[0].second;
    ll checkSum = 0;
    int j = disk.size() - 1;
    while( j > denseHead ) {
        if( disk[j] == -1 ) {
            j--;
        }
        else {
            int fileId = disk[j];
            int fileSize = fileSizeTable[fileId];
            j = j - fileSize;
            int s = 0;
            bool flag = false;
            for( s = 0; freeSpaceTable[s].second <= j && s < freeSpaceTable.size(); s++ ) {
                if( freeSpaceTable[s].first >= fileSize ) {
                    flag = true;
                    break;
                }
            }
            if( flag ) {
                pair<int, int> blockInfo = freeSpaceTable[s];
                for( int fill = 0; fill < fileSize; fill++ ) {
                    disk[j + 1 + fill] = -1;
                    disk[blockInfo.second + fill] = fileId;
                }
                // Consume free space
                // printDisk( disk );
                if( fileSize == blockInfo.first ) {
                    freeSpaceTable[s].first = 0;
                    freeSpaceTable.erase( freeSpaceTable.begin() + s );
                }
                else {
                    freeSpaceTable[s].first -= fileSize;
                    freeSpaceTable[s].second += fileSize;
                }
                // Create free space
                int former = j, latter = j + 1 + fileSize;
                if( disk[former] == -1 ) {
                    if( latter < disk.size() && disk[j + fileSize] == -1 ) {
                        // Merge former and latter
                        for( int i = 0; i < freeSpaceTable.size(); i++ ) {
                            if( freeSpaceTable[i].second + freeSpaceTable[i].first - 1 == former ) {
                                freeSpaceTable[i].first += ( fileSize + freeSpaceTable[i + 1].first );
                                freeSpaceTable.erase( freeSpaceTable.begin() + i + 1 );
                            }
                        }
                    }
                    else {
                        // Merge former only
                        for( int i = 0; i < freeSpaceTable.size(); i++ ) {
                            if( freeSpaceTable[i].second + freeSpaceTable[i].first - 1 == former ) {
                                freeSpaceTable[i].first += fileSize;
                            }
                        }
                    }
                }
                else {
                    if( latter < disk.size() && disk[j + fileSize] == -1 ) {
                        // Merge latter only
                        for( int i = 0; i < freeSpaceTable.size(); i++ ) {
                            if( freeSpaceTable[i].second == latter ) {
                                freeSpaceTable[i].first += fileSize;
                                freeSpaceTable[i].second = j + 1;
                            }
                        }
                    }
                    else {
                        // Don't Merge
                        ;
                    }
                }
                denseHead = freeSpaceTable[0].second - 1;
            }
        }
    }
    for( int diskId = 0; diskId < disk.size(); diskId++ ) {
        if( disk[diskId] != -1 )
            checkSum += diskId * disk[diskId];
    }
    printDisk( disk, "output.txt" );
    return checkSum;
}

int main() {
    FILE* input = fopen( "input.txt", "r" );
    // 6467290479134
    char linebuf[BUFFER_SIZE] = { '\0' };
    int fileId = 0;
    vector<int> disk;
    vector<int> fileSizeTable;
    vector<pair<int, int>> freeSpaceTable;
    bool isFile = true;
    while( !feof( input ) && fgets( linebuf, BUFFER_SIZE, input ) ) {
        string line( linebuf );
        for( int i = 0; i < line.size(); i++ ) {
            if( line[i] != '\n' && line[i] != '\0' ) {
                if( isFile ) {
                    appendFileBlock( fileId++, line[i] - '0', disk );
                    fileSizeTable.push_back( line[i] - '0' );
                    isFile = false;
                }
                else {
                    // atoi convets string to integer, but requires and end sign '\0' in the string;
                    appendEmptyBlock( line[i] - '0', disk );
                    if( line[i] - '0' != 0 ) {
                        freeSpaceTable.push_back( pair<int, int>( line[i] - '0', disk.size() - ( line[i] - '0' ) ) );
                    }
                    isFile = true;
                }
            }
        }
    }
    cout << fileCompack( disk, fileSizeTable, freeSpaceTable ) << endl;
}