#include "bits/stdc++.h"
using namespace std;
#include "../../utils/SolutionBase.h"
class Calibration : public SolutionBase {
    typedef long long ll;
    static const char operators[3];

    vector<pair<ll, vector<ll>>> readFile() {
        vector<pair<ll, vector<ll>>> numList;
        ifstream input( "Day07/input.txt" );
        string buf;
        for ( string buf; getline( input, buf ); ) {
            stringstream ss( buf );
            ll test = 0;
            vector<ll> nums;
            if ( ss >> test ) {
                ll tmp = 0;
                if ( ss.get() == ':' ) {
                    while ( ss >> tmp ) {
                        nums.push_back( tmp );
                    }
                }
            }
            // numList.insert( { test, nums } );
            numList.emplace_back( test, nums );
        }
        return numList;
    }

    static ll parse( vector<ll> nums, vector<char> op ) {
        size_t i = 0;
        ll res = 0;
        while ( i < op.size() ) {
            if ( op[i] == '+' ) {
                res = nums[i] + nums[i + 1];
                nums[++i] = res;
            } else if ( op[i] == '*' ) {
                res = nums[i] * nums[i + 1];
                nums[++i] = res;
            } else if ( op[i] == '|' ) {
                stringstream numStr( "" );
                string num1( "" ), num2( "" );
                numStr << nums[i];
                numStr >> num1;
                if ( numStr.eof() )
                    numStr.clear();
                else
                    cout << "Error Usage" << endl;
                numStr << nums[i + 1];
                numStr >> num2;
                res = stoll( string( num1 + num2 ) );
                nums[++i] = res;
            } else {
                cout << "Error situation" << endl;
            }
        }
        return nums.back();
    }
    // legacy version
    static inline void parseLine( ll test, vector<ll> const& nums, vector<char>& op, bool& equal, const int options ) {
        if ( op.size() == nums.size() - 1 ) {
            ll res = parse( nums, op );
            if ( res == test ) {
                equal = true;
                return;
            } else {
                if ( equal ) cout << "Error Algorithm" << endl;
                return;
            }
        }
        for ( int i = 0; i < options && !equal; i++ ) {
            op.push_back( operators[i] );
            parseLine( test, nums, op, equal, options );
            op.pop_back();
        }
        return;
    }

    static inline size_t dfsParse( size_t test, vector<ll> const& nums, const size_t curPos, const size_t curRes, const int options ) {
        if ( curPos == nums.size() - 1 ) return ( curRes == test ? test : 0 );
        vector<size_t> nextRes{ curRes + nums[curPos + 1], curRes * nums[curPos + 1], stoull( to_string( curRes ) + to_string( nums[curPos + 1] ) ) };
        for ( int i = 0; i < options; i++ ) {
            if ( nextRes[i] <= test && dfsParse( test, nums, curPos + 1, nextRes[i], options ) == test ) {
                return test;
            }
        }
        return 0;
    }

    static atomic<int> rowCnt;
    static mutex progress;
    static void progressIncreament() {
        lock_guard<mutex> lock{ progress };
        rowCnt++;
    }

    static size_t threadTask( const int startRow, const int endRow, const int options, vector<pair<ll, vector<ll>>> numList ) {
        size_t res = 0;
        for ( int i = startRow; i < endRow; i++ ) {
            progressIncreament();
            auto& [test, nums] = numList[i];
            // bool equal = false;
            // vector<char> operators;
            // parseLine( test, nums, operators, equal, options );
            // if ( equal ) {
            //     res += test;
            // }
            res += dfsParse( test, nums, 0, nums[0], options );
        }
        return res;
    }

   public:
    void Solution1() {
        vector<pair<ll, vector<ll>>> numList = readFile();
        size_t res = 0;
        int maxThread = thread::hardware_concurrency();
        int taskPerThread = numList.size() / maxThread;
        vector<future<size_t>> taskList;
        for ( int i = 0; i < maxThread; i++ ) {
            int startRow = i * taskPerThread;
            int endRow = ( i == maxThread - 1 ) ? numList.size() : ( i + 1 ) * taskPerThread;
            taskList.push_back( async( launch::async, threadTask, startRow, endRow, 2, numList ) );
        }
        for ( auto& task : taskList ) {
            res += task.get();
        }
        printRes(1,res);
    }

    void Solution2() {
        rowCnt = 0;
        vector<pair<ll, vector<ll>>> numList = readFile();
        size_t res = 0;
        int maxThread = thread::hardware_concurrency();
        int taskPerThread = numList.size() / maxThread;
        vector<future<size_t>> taskList;
        for ( int i = 0; i < maxThread; i++ ) {
            int startRow = i * taskPerThread;
            int endRow = ( i == maxThread - 1 ) ? numList.size() : ( i + 1 ) * taskPerThread;
            taskList.push_back( async( launch::async, threadTask, startRow, endRow, 3, numList ) );
        }
        // int lastChecked = 0, curChecked = 0, total = numList.size();
        // while ( lastChecked < total ) {
        //     curChecked = rowCnt.load();
        //     if ( curChecked - lastChecked > 10 || curChecked == total ) {
        //         lastChecked = curChecked;
        //         std::cout << "\033[2J\033[1;1H";
        //         cout << "Progress - Part 2" << endl;
        //         cout << "Positions checked: " << lastChecked << "/" << total << " (" << ( lastChecked * 100.0 / total ) << "%)" << endl;
        //     }
        // }
        for ( auto& task : taskList ) {
            res += task.get();
        }
        printRes(2,res);
    }
};
