#include <bits/stdc++.h>

#include <utils/SolutionBase.hpp>
using namespace std;
class PairAndAdd : public SolutionBase {
	static inline Register<PairAndAdd> _reg{};

    using ull = unsigned long long;

    void readFile( string filePath ) {
        regex re( "[0-9]+" );
        ifstream in( filePath );
        string buf;
        while ( getline( in, buf ) ) {
            sregex_iterator it( buf.begin(), buf.end(), re ), end;
            if ( distance( it, end ) == 2 ) {
                numList.push_back( { stoi( ( *it ).str() ), stoi( ( *( ++it ) ).str() ) } );
            }
        }
    }
    vector<pair<int, int>> numList;

   public:
    void Solution1() {
        readFile( "Day01/input.txt" );
        priority_queue<int, vector<int>, greater<>> pq1, pq2;
        for ( auto& [num1, num2] : numList ) {
            pq1.push( num1 );
            pq2.push( num2 );
        }
        int distanceAddUp = 0;
        int elemCnt = pq1.size();
        for ( int i = 0; i < elemCnt; i++ ) {
            distanceAddUp += abs( pq1.top() - pq2.top() );
            pq1.pop();
            pq2.pop();
        }
        printRes( 1, distanceAddUp );
    }

    void Solution2() {
        unordered_map<int, int> elemCount;
        for ( auto& [num1, num2] : numList ) {
            elemCount[num2]++;
        }
        int addUp = accumulate( numList.begin(), numList.end(), 0, [&]( int init, pair<int, int> elem ) {
            return init + elem.first * elemCount[elem.first];
        } );
        printRes( 2, addUp );
    }
};