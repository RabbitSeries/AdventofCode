#include <bits/stdc++.h>
using namespace std;

class MonkeyMarket {

    typedef unsigned long long ull;

    static ull getNextSecret( ull derival ) {
        // This algorithm is too random, there is no need to dp.
        derival = ( ( derival * 64 ) ^ derival ) % 16777216;
        derival = ( ( derival / 32 ) ^ derival ) % 16777216;
        derival = ( ( derival * 2048 ) ^ derival ) % 16777216;
        return derival;
    }

    vector<ull> readFile() {
        vector<ull> secrets;
        ifstream input( "Day22/input.txt" );
        string buf;
        while( getline( input, buf ) ) {
            if( !buf.empty() ) {
                ull tmp = 0;
                stringstream ss( buf );
                ss >> tmp;
                secrets.push_back( tmp );
            }
        }
        input.close();
        return move( secrets );
    }

    static ull getSecret( ull derival, int curDepth = 0, int targetDepth = 2000 ) {
        vector<ull> deriveList;
        deriveList.push_back( derival );
        while( curDepth < targetDepth ) {
            ull nextSecret = getNextSecret( derival );
            deriveList.push_back( nextSecret );
            derival = nextSecret;
            curDepth++;

        }
        return deriveList.back();
    }

    static ull threadAcc;
    static mutex accMutex;
    static void threadTask( int rowStart, int rowEnd, vector<ull> const& secrets ) {
        ull res = 0;
        for( int i = rowStart; i < rowEnd; i++ ) {
            ull part = getSecret( secrets[i] );
            res += part;
        }
        lock_guard<mutex> accLock( accMutex );
        threadAcc += res;
        return;
    }

    // deque is too slow.
    // map<deque<int>, int> threadAcc2;
    static map<string, int> threadAcc2;
    static mutex accMutex2;
    static void findSequence( ull derival, int curDepth = 0, int targetDepth = 2000 ) {
        vector<ull> deriveList;

        string curChanges;

        deriveList.push_back( derival );

        ull nextSecret = derival;
        int curOffer = ( to_string( derival ).back() - '0' );

        // TODO Change the mapping to map<tuple<int,int,int,int>,int>
        map<string, int> optimal;

        while( curDepth < targetDepth ) {

            if( curChanges.size() == 4 ) {
                if( optimal.find( curChanges ) != optimal.end() ) {
                    if( curOffer > optimal.at( curChanges ) ) {
                        // Dont't update. The monkey sells once seen the change sequence
                        // optimal[curChanges] = curOffer;
                    }
                }
                else {
                    optimal.emplace( curChanges, curOffer );
                }
                // Nervertheless
                curChanges = curChanges.substr( 1 );
            }

            nextSecret = getNextSecret( derival );
            curOffer = ( to_string( nextSecret ).back() - '0' );
            curChanges.push_back( to_string( nextSecret ).back() - to_string( derival ).back() );

            deriveList.push_back( nextSecret );
            derival = nextSecret;
            curDepth++;
        }

        lock_guard<mutex> updateAcc( accMutex2 );
        for( auto [changes, offer] : optimal ) {
            threadAcc2[changes] += offer;
        }
        return;
    }

    static void threadTask2( int rowStart, int rowEnd, vector<ull> const& secrets ) {
        for( int i = rowStart; i < rowEnd; i++ ) {
            findSequence( secrets[i] );
        }
        return;
    }

public:
    void Solution1() {
        vector<ull> secrets = readFile();
        ull res = 0;
        int maxThread = thread::hardware_concurrency();
        int taskPerthread = secrets.size() / maxThread;
        vector<thread> threadList;
        for( int i = 0; i < maxThread; i++ ) {
            int rowStart = i * taskPerthread;
            int rowEnd = ( i == maxThread - 1 ) ? secrets.size() : ( i + 1 ) * taskPerthread;
            threadList.push_back( thread( threadTask, rowStart, rowEnd, ref( secrets ) ) );
        }

        for( auto& t : threadList ) {
            t.join();
        }
        cout << "Solution 1: " << threadAcc << endl;
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
        if( enableMultiThreading ) {
            for( int i = 0; i < maxThread; i++ ) {
                int rowStart = i * taskPerthread;
                int rowEnd = ( i == maxThread - 1 ) ? secrets.size() : ( i + 1 ) * taskPerthread;
                threadList.push_back( thread( threadTask2, rowStart, rowEnd, ref( secrets ) ) );
            }
        }
        else {
            threadList.push_back( thread( threadTask2, 0, secrets.size(), ref( secrets ) ) );
        }
        for( auto& t : threadList ) {
            t.join();
        }

        int maxIncome = INT_MIN;
        int found = 0;
        map<string, int>::iterator maxIt = threadAcc2.end(), cur = threadAcc2.begin();
        while( cur != threadAcc2.end() ) {
            if( ( *cur ).second > maxIncome ) {
                found = 1;
                maxIt = cur;
                maxIncome = ( *cur ).second;
            }
            else if( ( *cur ).second == maxIncome ) {
                found++;
            }
            cur++;
        }
        for( int change : ( *maxIt ).first ) {
            cout << change << " ";
        }
        cout << endl;
        cout << found << endl;
        cout << "Solution 2: " << ( *maxIt ).second << endl;
        return;
    }
};
