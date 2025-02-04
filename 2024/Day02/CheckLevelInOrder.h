#include <bits/stdc++.h>
using namespace std;
class CheckLevelInOrder {
    inline bool checkDecrease( vector<int> const& Levels ) {
        bool res = true;
        int i = 0;
        while( i < Levels.size() - 1 ) {
            if( !( Levels[i] > Levels[i + 1] && Levels[i] - Levels[i + 1] <= 3 ) ) {
                return false;
            }
            i++;
        }
        return res;
    }

    inline bool checkIncrease( vector<int> const& Levels ) {
        bool res = true;
        int i = 0;
        while( i < Levels.size() - 1 ) {
            if( !( Levels[i] < Levels[i + 1] && Levels[i + 1] - Levels[i] <= 3 ) ) {
                return false;
            }
            i++;
        }
        return res;
    }

    bool canBeSafeByRemovingOneLevel( vector<int> const& Levels ) {
        // Try removing each level and check if the result is safe
        for( int i = 0; i < Levels.size(); ++i ) {
            vector<int> newLevels = Levels;
            newLevels.erase( newLevels.begin() + i );

            if( checkDecrease( newLevels ) || checkIncrease( newLevels ) ) {
                return true;
            }
        }
        return false;
    }

    bool AnalyseInOrder( ifstream& input, bool enableRemoval ) {
        // // char cline[1 << 10 + 1] = { '\0' };
        // // If fgets read a empty line, it won't set cline to "\0", thus remain ramdom value (if cline is two big, there is a chance that cline is allocated at a rather later position of the stack, and the space is not realocated to other usage, and because of which cline may also be reallocated to the same position because there might be such long free space ahead ) for cline.
        // The above is not right, the problem exists at the buffer size is not matched.
        // However, Always innitialize your datasets.
        // if( !fgets( cline, 1 << 10, input ) )
        string buf;
        if( !getline( input, buf ) )
            return false;
        stringstream lineInput( buf );
        int tmpLevel = 0;
        vector<int> Levels;
        while( lineInput >> tmpLevel ) {
            Levels.push_back( tmpLevel );
        }
        // Line check
        if( Levels.empty() ) return false;
        if( !enableRemoval ) {
            return checkDecrease( Levels ) || checkIncrease( Levels );
        }
        else {
            return checkDecrease( Levels ) || checkIncrease( Levels ) || canBeSafeByRemovingOneLevel( Levels );
        }
    }

public:
    void Solution1() {
        ifstream input( "Day02/input.txt" );
        int cnt = 0;
        while( !input.eof() ) {
            if( AnalyseInOrder( input, false ) ) {
                cnt++;
            }
        }
        cout << "Solution 1: " << cnt << endl;
    }

    void Solution2() {
        ifstream input( "Day02/input.txt" );
        int cnt = 0;

        while( !input.eof() ) {
            if( AnalyseInOrder( input, true ) ) {
                cnt++;
            }
        }

        cout << "Solution 2: " << cnt << endl;
        return;
    }

};