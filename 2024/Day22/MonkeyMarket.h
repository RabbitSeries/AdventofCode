#include <bits/stdc++.h>
using namespace std;

#include "../../utils/SolutionBase.h"
class MonkeyMarket : public SolutionBase {
    typedef unsigned long long ull;

    static ull getNextSecret( ull curSecret ) {
        // This algorithm is too random, there is no need to dp.
        curSecret = ( ( curSecret * 64 ) ^ curSecret ) % 16777216;
        curSecret = ( ( curSecret / 32 ) ^ curSecret ) % 16777216;
        curSecret = ( ( curSecret * 2048 ) ^ curSecret ) % 16777216;
        return curSecret;
    }

    void readFile() {
        ifstream input( "Day22/input.txt" );
        for ( string buf; getline( input, buf ); ) {
            secrets.push_back( stoi( buf ) );
        }
        input.close();
    }

    static ull getSecret( ull curSecret ) {
        for ( int i = 0; i < 2000; i++ ) {
            curSecret = getNextSecret( curSecret );
        }
        return curSecret;
    }

    static ull threadTask( int rowStart, int rowEnd, vector<ull> const& secrets ) {
        ull res = 0;
        for ( int i = rowStart; i < rowEnd; i++ ) {
            ull part = getSecret( secrets[i] );
            res += part;
        }
        return res;
    }

    static int index( deque<int> const& dq ) {
        return accumulate( dq.begin(), dq.end(), 0, []( int init, int e ) {
            return init * 19 + e;
        } );
    }

    static map<int, int> threadTask2( int rowStart, int rowEnd, vector<ull> const& secrets ) {
        map<int, int> zoneAcc;
        for ( int i = rowStart; i < rowEnd; i++ ) {
            int curSecret = secrets[i];
            string curChanges;
            ull nextSecret = curSecret;
            int curOffer = 0;
            map<int, int> optimal;
            deque<int> window;
            for ( int i = 0; i < 2000; i++ ) {
                nextSecret = getNextSecret( curSecret );
                curOffer = ( to_string( nextSecret ).back() - '0' );
                window.push_back( to_string( nextSecret ).back() - to_string( curSecret ).back() + 10 );
                curSecret = nextSecret;
                if ( window.size() == 4 ) {
                    int wId = index( window );
                    if ( !optimal.contains( wId ) ) {  // Dont't update. The monkey sells once seen the change sequence
                        optimal.emplace( wId, curOffer );
                    }
                    window.pop_front();
                }
            }
            for ( auto& p : optimal ) {
                zoneAcc[p.first] += p.second;
            }
        }
        return zoneAcc;
    }
    vector<ull> secrets;

   public:
    void Solution1() {
        readFile();
        int maxThread = thread::hardware_concurrency();
        int taskPerthread = secrets.size() / maxThread;
        vector<future<ull>> threadList;
        threadList.reserve( maxThread );
        for ( int i = 0; i < maxThread; i++ ) {
            int rowStart = i * taskPerthread;
            int rowEnd = ( i == maxThread - 1 ) ? secrets.size() : ( i + 1 ) * taskPerthread;
            threadList.push_back( async( threadTask, rowStart, rowEnd, ref( secrets ) ) );
        }
        ull threadAcc = 0;
        for ( auto& t : threadList ) {
            threadAcc += t.get();
        }
        printRes( 1, threadAcc );
        return;
    }

    void Solution2() {
        bool enableMultiThreading = true;
        vector<future<map<int, int>>> threadList;
        if ( int maxThread = thread::hardware_concurrency(), taskPerthread = secrets.size() / maxThread; enableMultiThreading ) {
            for ( int i = 0; i < maxThread; i++ ) {
                int rowStart = i * taskPerthread;
                int rowEnd = ( i == maxThread - 1 ) ? secrets.size() : ( i + 1 ) * taskPerthread;
                threadList.push_back( async( threadTask2, rowStart, rowEnd, ref( secrets ) ) );
            }
        } else {
            threadList.push_back( async( threadTask2, 0, secrets.size(), ref( secrets ) ) );
        }
        map<int, int> threadAcc;
        for ( auto& t : threadList ) {
            for ( auto& p : t.get() ) {
                threadAcc[p.first] += p.second;
            }
        }
        printRes( 2, ranges::max_element( threadAcc.begin(), threadAcc.end(), {}, []( pair<const int, int>& p ) {
                         return p.second;
                     } )->second );
        return;
    }
};