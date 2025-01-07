#include  "bits/stdc++.h"
#define BUFFER_SIZE 1024
typedef long long ll;
using namespace std;

const char operators[3] = { '+','*','|' };

vector<pair<ll, vector<ll>>> readFile() {
    vector<pair<ll, vector<ll>>> numList;
    ifstream input( "input.txt" );
    string buf;
    while( getline( input, buf ) ) {
        stringstream ss( buf );
        ll test = 0;
        vector<ll> nums;
        if( ss >> test ) {
            ll tmp = 0;
            if( ss.get() == ':' ) {
                while( ss >> tmp ) {
                    nums.push_back( tmp );
                }
            }
        }
        // numList.insert( { test, nums } );
        numList.emplace_back( test, nums );
    }
    return move( numList );
}


ll parse( vector<ll> nums, vector<char> op ) {
    int i = 0;
    ll res = 0;
    while( i < op.size() ) {
        if( op[i] == '+' ) {
            res = nums[i] + nums[i + 1];
            nums[++i] = res;
        }
        else if( op[i] == '*' ) {
            res = nums[i] * nums[i + 1];
            nums[++i] = res;
        }
        else if( op[i] == '|' ) {
            stringstream numStr( "" );
            string num1( "" ), num2( "" );
            numStr << nums[i];
            numStr >> num1;
            if( numStr.eof() ) numStr.clear();
            else
                cout << "Error Usage" << endl;
            numStr << nums[i + 1];
            numStr >> num2;
            res = stoll( string( num1 + num2 ) );
            nums[++i] = res;
        }
        else {
            cout << "Error situation" << endl;
        }
    }
    return nums.back();
}


inline void parseLine( ll test, vector<ll> const& nums, vector<char>& op, bool& equal, const int options ) {
    if( op.size() == nums.size() - 1 ) {
        ll res = parse( nums, op );
        if( res == test ) {
            equal = true;
            return;
        }
        else {
            if( equal ) cout << "Error Algorithm" << endl;
            return;
        }
    }
    for( int i = 0; i < options && !equal; i++ ) {
        op.push_back( operators[i] );
        parseLine( test, nums, op, equal, options );
        op.pop_back();
    }
    return;
}

ll threadTask( const int  startRow, const int  endRow, const int options, vector<pair<ll, vector<ll>>> numList ) {
    ll res = 0;
    for( int i = startRow; i < endRow; i++ ) {
        auto& [test, nums] = numList[i];
        bool equal = false;
        vector<char> operators;
        parseLine( test, nums, operators, equal, options );
        if( equal ) {
            res += test;
        }
    }
    return res;
}

void Solution1() {
    vector<pair<ll, vector<ll>>> numList = readFile();
    ll res = 0;
    int maxThread = thread::hardware_concurrency();
    int taskPerThread = numList.size() / maxThread;
    vector<future<ll>> taskList;
    for( int i = 0; i < maxThread; i++ ) {
        int startRow = i * taskPerThread;
        int endRow = ( i == maxThread - 1 ) ? numList.size() : ( i + 1 ) * taskPerThread;
        taskList.push_back( async( launch::async, threadTask, startRow, endRow, 2, numList ) );
    }
    for( auto& task : taskList ) {
        res += task.get();
    }
    cout << "Solution 1: " << res << endl;

}

void Solution2() {
    vector<pair<ll, vector<ll>>> numList = readFile();
    ll res = 0;
    int maxThread = thread::hardware_concurrency();
    int taskPerThread = numList.size() / maxThread;
    vector<future<ll>> taskList;
    for( int i = 0; i < maxThread; i++ ) {
        int startRow = i * taskPerThread;
        int endRow = ( i == maxThread - 1 ) ? numList.size() : ( i + 1 ) * taskPerThread;
        taskList.push_back( async( launch::async, threadTask, startRow, endRow, 3, numList ) );
    }
    for( auto& task : taskList ) {
        res += task.get();
    }
    cout << "Solution 2: " << res << endl;
}

int main() {
    auto now = chrono::high_resolution_clock::now();
    Solution1();
    Solution2();
    auto end = chrono::high_resolution_clock::now();
    cout << setprecision( 2 ) << chrono::duration_cast<chrono::milliseconds>( end - now ).count() / 1000 << endl;
    return 0;
}
