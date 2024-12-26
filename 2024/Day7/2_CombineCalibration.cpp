#include  "bits/stdc++.h"
#include <sstream>
#define BUFFER_SIZE 1024
typedef long long ll;
// typedef stringstream ss; // This gets error type
// typedef basic_stringstream<char> ss;
using namespace std;
ll eval( vector<ll> nums, vector<char> op ) {
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
            stringstream num1,num2;
            // There is seriout problem with stream implementation on linux machine.
            num1 << nums[i];
            num2 << nums[i + 1];
            res = stoll( string( num1.str() + num2.str() ) );
            nums[++i] = res;
        }
        else {
            cout << "Error situation" << endl;
        }
    }
    return nums.back();
}
void parseLine( ll test, vector<ll> nums, vector<char> op, bool& equal ) {
    if( op.size() == nums.size() - 1 ) {
        ll res = eval( nums, op );
        if( res == test ) {
            equal = true;
            return;
        }
        else {
            if( equal ) cout << "Error Algorithm" << endl;
            return;
        }
    }
    if( !equal ) {
        op.push_back( '+' );
        parseLine( test, nums, op, equal );
        op.pop_back();
    }
    if( !equal ) {
        op.push_back( '*' );
        parseLine( test, nums, op, equal );
        op.pop_back();
    }
    if( !equal ) {
        op.push_back( '|' );
        parseLine( test, nums, op, equal );
        op.pop_back();
    }
    return;
}

int main() {
    FILE* input = fopen( "input.txt", "r" );
    char linebuf[BUFFER_SIZE + 1] = { '\0' };
    ll res = 0;
    int currentLine = 0;
    while( !feof( input ) && fgets( linebuf, BUFFER_SIZE, input ) ) {
        cout << "Processing " << currentLine++ << endl;
        stringstream ss( linebuf );
        ll test = 0;
        vector<ll> nums;
        if( ss >> test ) {
            ll tmp = 0; char cono = '\0';
            ss >> cono;
            while( ss >> tmp ) {
                nums.push_back( tmp );
            }
        }
        if( !nums.empty() ) {
            bool equal = false;
            parseLine( test, nums, vector<char>(), equal );
            if( equal ) {
                res += test;
            }
        }
    }
    cout << res << endl;
}

