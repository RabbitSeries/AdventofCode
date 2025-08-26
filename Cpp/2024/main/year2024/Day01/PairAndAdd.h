#include <algorithm>
#include <map>
#include <numeric>
#include <queue>
#include <ranges>
#include <regex>
#include <unordered_map>
#include <vector>

#include "utils/BufferedReader.hpp"
#include "utils/SolutionBase.hpp"
#include "utils/Stream/Generator.hpp"

class PairAndAdd : public SolutionBase {
    REGISTER( PairAndAdd )

    std::vector<std::pair<int, int>> numList;

   public:
    void Solution1() {
        using namespace std;
        regex re( "[0-9]+" );
        numList = BufferedReader( "Day01/input.txt" ).lines().yield() | views::transform( [&]( const string& line ) {
                      sregex_iterator it( line.begin(), line.end(), re ), end;
                      return pair<int, int>{ stoi( ( *it ).str() ), stoi( ( *( ++it ) ).str() ) };
                  } ) |
                  ranges::to<vector<pair<int, int>>>();
        priority_queue<int, vector<int>, greater<>> pq1, pq2;
        for ( auto& [num1, num2] : numList ) {
            pq1.push( num1 );
            pq2.push( num2 );
        }
        printRes( 1, ranges::fold_left( [&]() -> utils::Generator<int> {
                      while ( !pq1.empty() && !pq2.empty() ) {
                          co_yield abs( pq1.top() - pq2.top() );
                          pq1.pop();
                          pq2.pop();
                      }
                  }(),
                                        0, std::plus<>{} ) );
    }

    void Solution2() {
        using namespace std;
        unordered_map<int, int> elemCount;
        for ( auto& [num1, num2] : numList ) {
            elemCount[num2]++;
        }
        int addUp = ranges::fold_left( numList | views::transform( [&]( const pair<int, int>& elem ) {
                                           return elem.first * elemCount[elem.first];
                                       } ),
                                       0, std::plus<>{} );
        printRes( 2, addUp );
    }
};