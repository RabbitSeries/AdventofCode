#include <bits/stdc++.h>
using namespace std;
class CheckLevelInOrder : public SolutionBase {
	REGISTER( CheckLevelInOrder )

    inline bool checkDecrease( vector<int> const& Levels ) {
        bool res = true;
        for ( size_t i = 0; i < Levels.size() - 1; i++ ) {
            if ( !( Levels[i] > Levels[i + 1] && Levels[i] - Levels[i + 1] <= 3 ) ) {
                return false;
            }
        }
        return res;
    }

    inline bool checkIncrease( vector<int> const& Levels ) {
        bool res = true;
        for ( size_t i = 0; i < Levels.size() - 1; i++ ) {
            if ( !( Levels[i] < Levels[i + 1] && Levels[i + 1] - Levels[i] <= 3 ) ) {
                return false;
            }
        }
        return res;
    }

    bool canBeSafeByRemovingOneLevel( vector<int> const& Levels ) {
        // Try removing each level and check if the result is safe
        for ( size_t i = 0; i < Levels.size(); ++i ) {
            vector<int> newLevels = Levels;
            newLevels.erase( newLevels.begin() + i );

            if ( checkDecrease( newLevels ) || checkIncrease( newLevels ) ) {
                return true;
            }
        }
        return false;
    }
    vector<vector<int>> LevelLists;

    void readFile() {
        ifstream input( "Day02/input.txt" );
        vector<int> Levels;
        for ( string buf; getline( input, buf ); ) {
            istringstream ss( buf );
            copy( istream_iterator<int>( ss ), istream_iterator<int>(), back_inserter( Levels ) );
            LevelLists.emplace_back( move( Levels ) );
            Levels.clear();
        }
        input.close();
    }
    bool AnalyseInOrder( vector<int>& Levels, bool enableRemoval ) {
        if ( Levels.empty() ) return false;
        if ( !enableRemoval ) {
            return checkDecrease( Levels ) || checkIncrease( Levels );
        } else {
            return checkDecrease( Levels ) || checkIncrease( Levels ) || canBeSafeByRemovingOneLevel( Levels );
        }
    }

   public:
    void Solution1() {
        readFile();
        int cnt = 0;
        for ( auto& Levels : LevelLists ) {
            if ( AnalyseInOrder( Levels, false ) ) {
                cnt++;
            }
        }
        printRes( 1, cnt );
    }

    void Solution2() {
        int cnt = 0;
        for ( auto& Levels : LevelLists ) {
            if ( AnalyseInOrder( Levels, true ) ) {
                cnt++;
            }
        }
        printRes( 2, cnt );
    }
};