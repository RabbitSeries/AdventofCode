#include <algorithm>
#include <fstream>
#include <ranges>
#include <string>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

#include "utils/ISolution.hpp"
class KeyPair : public ISolution {
    REGISTER( KeyPair )

    using schematic = std::vector<int>;
    struct SchemaHasher {
        size_t operator()( const schematic& obj ) const {
            return std::ranges::fold_left( obj, 0, []( int sum, int num ) {
                return sum * 10 + num;
            } );
        }
    };
    using SchemaSet = std::unordered_set<schematic, SchemaHasher>;

   public:
    void Solution1() {
        SchemaSet lockList, keyList;
        std::ifstream input( "Day25/input.txt" );
        int rowTimes = 0;
        bool isLock = true;
        schematic row( 5, 0 );
        for ( std::string linebuf; getline( input, linebuf ); ) {
            if ( linebuf.empty() ) {
                rowTimes = 0;
                continue;
            }
            if ( rowTimes == 0 ) {
                isLock = ( linebuf == "#####" );
                rowTimes++;
            } else {
                if ( rowTimes <= 5 ) {
                    for ( auto [r, p] : std::views::zip( row, linebuf ) ) {
                        r += ( p == '#' );
                    }
                    rowTimes++;
                } else {
                    if ( isLock ) {
                        lockList.emplace( std::move( row ) );
                    } else {
                        keyList.emplace( std::move( row ) );
                    }
                    row = schematic( 5, 0 );
                }
            }
        }
        int res = 0;
        for ( auto& lock : lockList ) {
            for ( auto& key : keyList ) {
                res += std::ranges::all_of( std::views::zip( lock, key ), []( auto const& pin ) {
                    return ( std::get<0>( pin ) + std::get<1>( pin ) ) <= 5;
                } );
            }
        }
        printRes( 1, res );
    }

    void Solution2() {
        printRes( 2, "\t\t\t------All 50 stars AoC Finished!!!!------" );
    }
};