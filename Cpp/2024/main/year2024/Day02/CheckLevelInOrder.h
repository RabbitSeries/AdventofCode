#include <algorithm>
#include <string>
#include <vector>

#include "utils/BufferedReader.hpp"
#include "utils/SolutionBase.hpp"
class CheckLevelInOrder : public SolutionBase {
    REGISTER( CheckLevelInOrder )

    inline bool checkDecrease( std::vector<int> const& Levels ) {
        bool res = true;
        for ( size_t i = 0; i < Levels.size() - 1; i++ ) {
            if ( !( Levels[i] > Levels[i + 1] && Levels[i] - Levels[i + 1] <= 3 ) ) {
                return false;
            }
        }
        return res;
    }

    inline bool checkIncrease( std::vector<int> const& Levels ) {
        bool res = true;
        for ( size_t i = 0; i < Levels.size() - 1; i++ ) {
            if ( !( Levels[i] < Levels[i + 1] && Levels[i + 1] - Levels[i] <= 3 ) ) {
                return false;
            }
        }
        return res;
    }

    bool canBeSafeByRemovingOneLevel( std::vector<int> const& Levels ) {
        // Try removing each level and check if the result is safe
        for ( size_t i = 0; i < Levels.size(); ++i ) {
            std::vector<int> newLevels = Levels;
            newLevels.erase( newLevels.begin() + i );

            if ( checkDecrease( newLevels ) || checkIncrease( newLevels ) ) {
                return true;
            }
        }
        return false;
    }
    std::vector<std::vector<int>> LevelLists;

    void readFile() {
        using namespace std;
        LevelLists = BufferedReader( "Day02/input.txt" ).lines().yield() | views::transform( []( const string& line ) {
                         istringstream ss( line );
                         return views::istream<int>( ss ) | ranges::to<std::vector<int>>();
                     } ) |
                     ranges::to<vector<vector<int>>>();
    }

    bool AnalyseInOrder( std::vector<int> const& Levels, bool enableRemoval = false ) {
        if ( Levels.empty() ) {
            return false;
        }
        return checkDecrease( Levels ) || checkIncrease( Levels ) || ( enableRemoval ? canBeSafeByRemovingOneLevel( Levels ) : false );
    }

   public:
    void Solution1() {
        readFile();
        printRes( 1, std::ranges::count_if( LevelLists, [this]( auto const& Levels ) { return AnalyseInOrder( Levels ); } ) );
    }

    void Solution2() {
        printRes( 2, std::ranges::count_if( LevelLists, [this]( auto const& Levels ) { return AnalyseInOrder( Levels, true ); } ) );
    }
};