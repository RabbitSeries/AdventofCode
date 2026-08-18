#include <immintrin.h>

#include <algorithm>
#include <map>
#include <numeric>
#include <queue>
#include <ranges>
#include <regex>
#include <unordered_map>
#include <vector>

#include "utils/ISolution.hpp"
#include "utils/Streams.hpp"

class PairAndAdd: public ISolution {
    REGISTER( PairAndAdd )

    std::vector<std::pair<int, int>> numList;

    public:
    void Solution1() {
        using namespace std;
        numList = fileLinesStream( "Day01/input.txt" ) |
                  views::transform( [&]( string&& line ) {
                      int a, b;
                      std::stringstream( std::move( line ) ) >> a >> b;
                      return pair<int, int>{ a, b };
                  } ) |
                  ranges::to<vector<pair<int, int>>>();

        vector<int> v1, v2;
        v1.reserve( numList.size() );
        v2.reserve( numList.size() );
        for ( auto& [num1, num2] : numList ) {
            v1.push_back( num1 );
            v2.push_back( num2 );
        }
        sort( v1.begin(), v1.end() );
        sort( v2.begin(), v2.end() );

        int sum = 0;
        size_t i = 0;
        size_t n = v1.size();

        __m256i sum_vec = _mm256_setzero_si256();
        for ( ; i + 7 < n; i += 8 ) {
            // substraction
            __m256i a = _mm256_loadu_si256( (__m256i*)&v1[i] );
            __m256i b = _mm256_loadu_si256( (__m256i*)&v2[i] );
            __m256i diff = _mm256_sub_epi32( a, b );
            // abs
            __m256i sign_mask = _mm256_srai_epi32( diff, 31 );
            // x = (-x) + 1 = (-x) - (-1)
            __m256i abs_diff = _mm256_xor_si256( diff, sign_mask );
            abs_diff = _mm256_sub_epi32( abs_diff, sign_mask );
            sum_vec = _mm256_add_epi32( sum_vec, abs_diff );
        }
        // Store
        int32_t temp[8];
        _mm256_storeu_si256( (__m256i*)temp, sum_vec );
        for ( int j = 0; j < 8; j++ ) {
            sum += temp[j];
        }
        // % 8
        for ( ; i < n; i++ ) {
            sum += abs( v1[i] - v2[i] );
        }
        printRes( 1, sum );
    }

    void Solution2() {
        using namespace std;
        unordered_map<int, int> elemCount;
        for ( auto& [num1, num2] : numList ) {
            elemCount[num2]++;
        }
        int addUp = ranges::fold_left(
            numList | views::transform( [&]( const pair<int, int>& elem ) {
                return elem.first * elemCount[elem.first];
            } ),
            0, std::plus<>{} );
        printRes( 2, addUp );
    }
};
