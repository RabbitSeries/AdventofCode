#include <bits/stdc++.h>
using namespace std;
class CheckLevelInOrder : SolutionBase {
    inline bool checkDecrease( vector<int> const& Levels ) {
        bool res = true;
        int i = 0;
        while ( i < Levels.size() - 1 ) {
            if ( !( Levels[i] > Levels[i + 1] && Levels[i] - Levels[i + 1] <= 3 ) ) {
                return false;
            }
            i++;
        }
        return res;
    }

    inline bool checkIncrease( vector<int> const& Levels ) {
        bool res = true;
        int i = 0;
        while ( i < Levels.size() - 1 ) {
            if ( !( Levels[i] < Levels[i + 1] && Levels[i + 1] - Levels[i] <= 3 ) ) {
                return false;
            }
            i++;
        }
        return res;
    }

    bool canBeSafeByRemovingOneLevel( vector<int> const& Levels ) {
        // Try removing each level and check if the result is safe
        for ( int i = 0; i < Levels.size(); ++i ) {
            vector<int> newLevels = Levels;
            newLevels.erase( newLevels.begin() + i );

            if ( checkDecrease( newLevels ) || checkIncrease( newLevels ) ) {
                return true;
            }
        }
        return false;
    }

    bool AnalyseInOrder( ifstream& input, bool enableRemoval ) {
        string buf;
        if ( !getline( input, buf ) )
            return false;
        stringstream lineInput( buf );
        int tmpLevel = 0;
        vector<int> Levels;
        while ( lineInput >> tmpLevel ) {
            Levels.push_back( tmpLevel );
        }
        // Line check
        if ( Levels.empty() ) return false;
        if ( !enableRemoval ) {
            return checkDecrease( Levels ) || checkIncrease( Levels );
        } else {
            return checkDecrease( Levels ) || checkIncrease( Levels ) || canBeSafeByRemovingOneLevel( Levels );
        }
    }

   public:
    void Solution1() {
        ifstream input( "Day02/input.txt" );
        int cnt = 0;
        while ( !input.eof() ) {
            if ( AnalyseInOrder( input, false ) ) {
                cnt++;
            }
        }
        printRes(1,cnt);
    }

    void Solution2() {
        ifstream input( "Day02/input.txt" );
        int cnt = 0;

        while ( !input.eof() ) {
            if ( AnalyseInOrder( input, true ) ) {
                cnt++;
            }
        }
        printRes(2,cnt);
    }
};