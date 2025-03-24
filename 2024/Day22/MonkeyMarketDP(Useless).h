#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;

ull getNextSecret( ull derival ) {
    // This algorithm is too random, there is no need to dp.
    derival = ( ( derival * 64 ) ^ derival ) % 16777216;
    derival = ( ( derival / 32 ) ^ derival ) % 16777216;
    derival = ( ( derival * 2048 ) ^ derival ) % 16777216;
    return derival;
}

vector<ull> readFile() {
    vector<ull> secrets;
    ifstream input( "Day22/input.txt" );
    for ( string buf; getline( input, buf ); ) {
        if ( !buf.empty() ) {
            ull tmp = 0;
            stringstream ss( buf );
            ss >> tmp;
            secrets.push_back( tmp );
        }
    }
    input.close();
    return move( secrets );
}

ull getSecret( ull derival, int curDepth = 0, int targetDepth = 2000 ) {
    vector<ull> deriveList;
    // map<ull, int> deriveHashTable;
    deriveList.push_back( derival );
    // deriveHashTable.emplace( derival, 0 );
    while ( curDepth < targetDepth ) {
        ull nextSecret = getNextSecret( derival );
        // if( deriveHashTable.find( nextSecret ) != deriveHashTable.end() ) {
        //     int foundPlace = deriveHashTable.at( nextSecret );
        //     int fastFoward = deriveList.size() - foundPlace;
        //     return deriveList[foundPlace + ( targetDepth - curDepth - 1 ) % fastFoward];
        // }
        // else {
        // deriveHashTable.emplace( nextSecret, deriveList.size() );
        deriveList.push_back( nextSecret );
        derival = nextSecret;
        curDepth++;
        // }
    }
    return deriveList.back();
}

ull threadAcc = 0;
mutex accMutex;
void threadTask( int rowStart, int rowEnd, vector<ull> const& secrets ) {
    ull res = 0;
    for ( int i = rowStart; i < rowEnd; i++ ) {
        ull part = getSecret( secrets[i] );
        res += part;
    }
    lock_guard<mutex> accLock( accMutex );
    threadAcc += res;
    return;
}

void Solution1() {
    vector<ull> secrets = readFile();
    ull res = 0;
    int maxThread = thread::hardware_concurrency();
    int taskPerthread = secrets.size() / maxThread;
    vector<thread> threadList;
    for ( int i = 0; i < maxThread; i++ ) {
        int rowStart = i * taskPerthread;
        int rowEnd = ( i == maxThread - 1 ) ? secrets.size() : ( i + 1 ) * taskPerthread;
        threadList.push_back( thread( threadTask, rowStart, rowEnd, ref( secrets ) ) );
    }

    for ( auto& t : threadList ) {
        t.join();
    }
    cout << "Solution 1: " << threadAcc << endl;
    return;
}

map<deque<int>, int> threadAcc2;
mutex accMutex2;
void findSequence( ull derival, int curDepth = 0, int targetDepth = 2000 ) {
    vector<ull> deriveList;
    map<ull, int> deriveHashTable;
    deque<int> curChanges;

    deriveList.push_back( derival );
    deriveHashTable.emplace( derival, 0 );
    ull nextSecret = derival;
    int curOffer = ( to_string( derival ).back() - '0' );

    // TODO Change the mapping to map<tuple<int,int,int,int>,int>
    map<deque<int>, int> optimal;

    while ( curDepth < targetDepth ) {
        if ( curChanges.size() == 4 ) {
            if ( optimal.find( curChanges ) != optimal.end() ) {
                if ( curOffer > optimal.at( curChanges ) ) {
                    optimal[curChanges] = curOffer;
                }
            } else {
                optimal.emplace( curChanges, curOffer );
            }
            // Nervertheless
            curChanges.pop_front();
        }

        nextSecret = getNextSecret( derival );
        curOffer = ( to_string( nextSecret ).back() - '0' );
        curChanges.push_back( to_string( nextSecret ).back() - to_string( derival ).back() );

        if ( deriveHashTable.find( nextSecret ) != deriveHashTable.end() ) {
            if ( curChanges.size() == 4 ) {
                break;
            } else {
                derival = nextSecret;
                curDepth++;
            }
        } else {
            deriveHashTable.emplace( nextSecret, deriveList.size() );
            deriveList.push_back( nextSecret );
            derival = nextSecret;
            curDepth++;
        }
    }
    lock_guard<mutex> updateAcc( accMutex2 );
    for ( auto [changes, bananas] : optimal ) {
        threadAcc2[changes] += bananas;
    }
    return;
}

void threadTask2( int rowStart, int rowEnd, vector<ull> const& secrets ) {
    for ( int i = rowStart; i < rowEnd; i++ ) {
        findSequence( secrets[i] );
    }
    return;
}

void Solution2() {
    vector<ull> secrets = readFile();
    ull res = 0;
    bool enableMultiThreading = true;

    int maxThread = thread::hardware_concurrency();
    int taskPerthread = secrets.size() / maxThread;
    threadAcc = 0;
    vector<thread> threadList;
    if ( enableMultiThreading ) {
        for ( int i = 0; i < maxThread; i++ ) {
            int rowStart = i * taskPerthread;
            int rowEnd = ( i == maxThread - 1 ) ? secrets.size() : ( i + 1 ) * taskPerthread;
            threadList.push_back( thread( threadTask2, rowStart, rowEnd, ref( secrets ) ) );
        }
    } else {
        threadList.push_back( thread( threadTask2, 0, secrets.size(), ref( secrets ) ) );
    }
    for ( auto& t : threadList ) {
        t.join();
    }

    int maxIncome = INT_MIN;
    int found = 0;
    map<deque<int>, int>::iterator maxIt = threadAcc2.end(), cur = threadAcc2.begin();
    while ( cur != threadAcc2.end() ) {
        if ( ( *cur ).second > maxIncome ) {
            found = 1;
            maxIt = cur;
            maxIncome = ( *cur ).second;
        } else if ( ( *cur ).second == maxIncome ) {
            found++;
        }
        cur++;
    }
    cout << "Solution 2: " << ( *maxIt ).second << endl;
    return;
}

// int main() {
//     auto now = chrono::high_resolution_clock::now();
//     Solution1();
//     Solution2();
//     auto end = chrono::high_resolution_clock::now();
//     cout << "Algorithm takes: " << setprecision( 2 ) << chrono::duration_cast<chrono::milliseconds>( end - now ).count() / 1000.0 << " seconds" << endl;
//     return 0;
// }