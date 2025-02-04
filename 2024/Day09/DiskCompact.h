#include "bits/stdc++.h"
using namespace std;

class DiskCompact {
    typedef long long ll;

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

    ll fileCompack( vector<int>disk ) {
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

    void readFile( vector<int>& disk, vector<int>& fileSizeTable, vector<pair<int, int>>& freeSpaceTable ) {
        ifstream input( "Day09/input.txt" );
        string linebuf;
        int fileId = 0;
        bool isFile = true;
        while( getline( input, linebuf ) ) {
            for( int i = 0; i < linebuf.size(); i++ ) {
                if( linebuf[i] != '\n' && linebuf[i] != '\0' ) {
                    if( isFile ) {
                        appendFileBlock( fileId++, linebuf[i] - '0', disk );
                        fileSizeTable.push_back( linebuf[i] - '0' );
                        isFile = false;
                    }
                    else {
                        // atoi convets string to integer, but requires and end sign '\0' in the string;
                        appendEmptyBlock( linebuf[i] - '0', disk );
                        if( linebuf[i] - '0' != 0 ) {
                            freeSpaceTable.push_back( pair<int, int>( linebuf[i] - '0', disk.size() - ( linebuf[i] - '0' ) ) );
                        }
                        isFile = true;
                    }
                }
            }
        }
    }


    void printDisk( const vector<int>disk, const string path ) {
        int cnt = 0;
        FILE* output = fopen( path.c_str(), "w" );
        for( int diskId = 0; diskId < disk.size(); diskId++ ) {
            if( disk[diskId] == -1 ) {
                // cout << ".";
                fprintf( output, "_" );
            }
            else {
                fprintf( output, "%s", "X" );
            }
        }
        fclose( output );
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
        printDisk( disk, "Day09/output.txt" );
        return checkSum;
    }
public:
    void Solution1() {
        vector<int> disk;
        vector<int> fileSizeTable;
        vector<pair<int, int>> freeSpaceTable;
        readFile( disk, fileSizeTable, freeSpaceTable );
        cout << "Solution 1: " << fileCompack( disk ) << endl;
    }

    void Solution2() {
        vector<int> disk;
        vector<int> fileSizeTable;
        vector<pair<int, int>> freeSpaceTable;
        readFile( disk, fileSizeTable, freeSpaceTable );
        cout << "Solution 2: " << fileCompack( disk, fileSizeTable, freeSpaceTable ) << endl;
    }
};
