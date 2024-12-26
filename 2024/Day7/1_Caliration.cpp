#include  "bits/stdc++.h"
#define BUFFER_SIZE 1024
typedef long long ll;
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
    return;
}

int main() {
    FILE* input = fopen( "input.txt", "r" );
    char linebuf[BUFFER_SIZE + 1] = { '\0' };
    ll res = 0;
    while( !feof( input ) && fgets( linebuf, BUFFER_SIZE, input ) ) {
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
    cout << res;
}

