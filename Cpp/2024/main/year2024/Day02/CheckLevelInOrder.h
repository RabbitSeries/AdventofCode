#include <algorithm>
#include <string>
#include <vector>

#include "utils/BufferedReader.hpp"
#include "utils/ISolution.hpp"
class CheckLevelInOrder : public ISolution {
    REGISTER( CheckLevelInOrder )

    bool checkOrder( std::vector<int> const& Levels, bool desc = true, size_t skip = -1 ) {
        bool res = true;
        for ( size_t i = 0; i < Levels.size() - 1; i++ ) {
            if ( ( i == skip && i == 0 ) || i + 1 == skip ) {
                continue;
            }
            int former = i == skip ? Levels[i - 1] : Levels[i];
            int latter = Levels[i + 1];
            if ( !desc ) {
                std::swap( former, latter );
            }
            if ( !( former > latter && former - latter <= 3 ) ) {
                return false;
            }
        }
        return res;
    }

    bool canBeSafeByRemovingOneLevel( std::vector<int> const& Levels ) {
        for ( size_t i = 0; i < Levels.size(); ++i ) {
            if ( checkOrder( Levels, true, i ) || checkOrder( Levels, false, i ) ) {
                return true;
            }
        }
        return false;
    }
    std::vector<std::vector<int>> LevelLists;

    void readFile() {
        LevelLists = BufferedReader( "Day02/input.txt" ).lines().yield() | std::views::transform( []( const std::string& line ) {
                         std::istringstream ss( std::move( const_cast<std::string&>( line ) ) );
                         return std::views::istream<int>( ss ) | std::ranges::to<std::vector<int>>();
                     } ) |
                     std::ranges::to<std::vector<std::vector<int>>>();
    }

   public:
    void Solution1() {
        readFile();
        printRes( 1, std::ranges::count_if( LevelLists, [this]( auto const& Levels ) { return checkOrder( Levels ) || checkOrder( Levels, false ); } ) );
    }

    void Solution2() {
        printRes( 2, std::ranges::count_if( LevelLists, [this]( auto const& Levels ) { return checkOrder( Levels ) || checkOrder( Levels, false ) ||
                                                                                              canBeSafeByRemovingOneLevel( Levels ); } ) );
    }
};