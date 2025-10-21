#include <algorithm>
#include <fstream>
#include <map>
#include <ranges>
#include <string>
#include <vector>

#include "utils/ISolution.hpp"
class DiskCompact : public ISolution {
    REGISTER( DiskCompact )

    using ll = long long;

    void appendFileBlock( int id, int size, std::vector<int>& disk ) {
        while ( size-- ) {
            disk.push_back( id );
        }
    }

    void appendEmptyBlock( int size, std::vector<int>& disk ) {
        while ( size-- ) {
            disk.push_back( -1 );
        }
    }

    ll fileCompack( std::vector<int> disk ) {
        ll checkSum = 0;
        int len = (int)disk.size();
        int i = 0, j = len - 1;
        while ( i < len && j > i ) {
            if ( disk[i] == -1 ) {
                while ( j > i && disk[j] == -1 ) {
                    j--;
                }
                if ( j > i ) {
                    disk[i] = disk[j];
                    disk[j] = -1;
                    checkSum += disk[i] * i;
                    i++;
                } else {
                    break;
                }
            } else {
                checkSum += disk[i] * i;
                i++;
            }
        }
        return checkSum;
    }

    void readFile() {
        std::ifstream input( "Day09/input.txt" );
        std::string linebuf;
        int fileId = 0;
        bool isFile = false;
        while ( getline( input, linebuf ) ) {
            for ( char c : linebuf ) {
                isFile = !isFile;
                if ( c == '0' ) {
                    continue;
                }
                if ( isFile ) {
                    appendFileBlock( fileId++, c - '0', disk );
                    fileSizeTable.push_back( c - '0' );
                } else {
                    // atoi convets string to integer, but requires and end sign '\0' in the string;
                    freeSpaceTable.emplace_back( c - '0', disk.size() );
                    appendEmptyBlock( c - '0', disk );
                }
            }
        }
    }

    // void printDisk( const std::vector<int> disk, const char* path ) {
    //     // int cnt = 0;
    //     std::ofstream of( path );
    //     for ( size_t diskId = 0; diskId < disk.size(); diskId++ ) {
    //         if ( disk[diskId] == -1 ) {
    //             of << "_";
    //         } else {
    //             of << "X";
    //         }
    //     }
    // }

    auto findBlockHandle( int diskPos, bool endWith ) {
        int l = 0, r = (int)freeSpaceTable.size() - 1;
        auto best = freeSpaceTable.end();
        // Binary search
        while ( l <= r ) {
            int mid = ( l + r ) / 2;
            // If search fore end with, apply block length offset
            int cmp = freeSpaceTable[mid].second + ( endWith ? freeSpaceTable[mid].first - 1 : 0 );
            if ( cmp >= diskPos ) {
                best = freeSpaceTable.begin() + mid;
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }
        return best;
    }

    ll fileCompack( std::vector<int> disk, const std::vector<int>& fileSizeTable, std::vector<std::pair<int, int>>& freeSpaceTable ) {
        // printDisk( disk, "Day09/original.txt" );
        int ptr = (int)disk.size() - 1;
        while ( !freeSpaceTable.empty() && ptr > freeSpaceTable[0].second ) {
            if ( disk[ptr] == -1 ) {
                ptr--;
                continue;
            }
            int fileId = disk[ptr];
            int fileSize = fileSizeTable[fileId];
            ptr = ptr - fileSize;
            auto avaiBlock = std::ranges::find_if( freeSpaceTable, [&]( auto& space ) {
                return space.first >= fileSize && space.second <= ptr;
            } );
            bool canMove = avaiBlock != freeSpaceTable.end();
            if ( !canMove ) {
                continue;
            }
            for ( int fill = 0; fill < fileSize; fill++ ) {
                disk[ptr + 1 + fill] = -1;                // Overwrite file to empty
                disk[avaiBlock->second + fill] = fileId;  // Overwrite allocated to fileId
            }
            // Consume free space
            // printDisk( disk );
            if ( fileSize == avaiBlock->first ) {
                freeSpaceTable.erase( avaiBlock );  // Fully consume
            } else {
                avaiBlock->first -= fileSize;   // Decrease remain size
                avaiBlock->second += fileSize;  // Offset free block start pos by fileSize
            }
            // Release free space to Space Table
            int beforeFile = ptr, afterFile = ptr + 1 + fileSize;
            if ( disk[beforeFile] == -1 ) {                                     // Space before released space is free space
                auto handle = findBlockHandle( beforeFile, true );              // Find the former free space's handle
                handle->first += fileSize;                                      // Merge released block into former block
                if ( afterFile < (int)disk.size() && disk[afterFile] == -1 ) {  // After released space is also free space
                    handle->first += ( handle + 1 )->first;                     // Merge latter block into former block
                    freeSpaceTable.erase( handle + 1 );                         // Remove latter block
                }
            } else {  // Space before released space is not free space
                auto handle = findBlockHandle( afterFile, false );
                if ( handle == freeSpaceTable.end() ) {  // There is no after block, create a new one
                    freeSpaceTable.emplace_back( fileSize, beforeFile + 1 );
                } else {  // Merge released file to latter block
                    handle->first += fileSize;
                    handle->second = beforeFile + 1;
                }
            }
        }
        ll checkSum = 0;
        for ( size_t diskId = 0; diskId < disk.size(); diskId++ ) {
            if ( disk[diskId] != -1 )
                checkSum += diskId * disk[diskId];
        }
        // printDisk( disk, "Day09/output.txt" );
        return checkSum;
    }
    std::vector<int> disk;
    std::vector<int> fileSizeTable;
    // [FreeSpace : StartPos,...]
    std::vector<std::pair<int, int>> freeSpaceTable;

   public:
    void Solution1() {
        readFile();
        printRes( 1, fileCompack( disk ) );
    }

    void Solution2() {
        printRes( 2, fileCompack( disk, fileSizeTable, freeSpaceTable ) );
    }
};
