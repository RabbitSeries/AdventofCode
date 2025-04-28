#include "bits/stdc++.h"
using namespace std;
#include "../../utils/SolutionBase.h"
class Calibration : public SolutionBase {
    using ull = unsigned long long;
    void readFile() {
        ifstream input( "Day07/input.txt" );
        for ( string buf; getline( input, buf ); ) {
            stringstream ss( buf );
            ull test = 0;
            vector<ull> nums;
            if ( ss >> test ) {
                if ( ss.get() == ':' ) {
                    copy( istream_iterator<ull>( ss ), istream_iterator<ull>(), back_inserter( nums ) );
                }
            }
            numList.emplace_back( test, move( nums ) );
        }
        input.close();
    }
    size_t dfsParse( size_t test, vector<ull> const& nums, const size_t curPos, const size_t curRes, const int options ) {
        if ( curPos == 0 ) return ( curRes == 0 ? test : 0 );
        if ( curRes >= nums[curPos - 1] ) {
            if ( dfsParse( test, nums, curPos - 1, curRes - nums[curPos - 1], options ) == test )
                return test;
            if ( ( curRes % nums[curPos - 1] ) == 0 && dfsParse( test, nums, curPos - 1, curRes / nums[curPos - 1], options ) == test )
                return test;
            if ( options == 3 ) {
                string curStr = to_string( curRes );
                string formerStr = to_string( nums[curPos - 1] );
                if ( curStr.ends_with( formerStr ) ) {
                    size_t pos = curStr.find_last_of( formerStr ), nextRes = 0;
                    if ( pos != 0 ) {
                        nextRes = stoull( curStr.substr( 0, pos ) );
                    }
                    if ( dfsParse( test, nums, curPos - 1, nextRes, options ) == test ) {
                        return test;
                    }
                }
            }
        }
        return 0;
    }
    size_t parseWith( const int options ) {
        size_t res = 0;
        for ( auto& [test, nums] : numList ) {
            res += dfsParse( test, nums, nums.size(), test, options );
        }
        return res;
    }
    vector<pair<ull, vector<ull>>> numList;

   public:
    void Solution1() {
        readFile();
        printRes( 1, parseWith( 2 ) );
    }

    void Solution2() {
        printRes( 2, parseWith( 3 ) );
    }
};