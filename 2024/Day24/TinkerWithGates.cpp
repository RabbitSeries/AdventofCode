#include "gates.h"
void Solution1() {
    queue<string> processWireNameQueue = readFile();
    long long solutionRes = runGates( processWireNameQueue );
    cout << "Solution 1: " << solutionRes << endl;
}

int res = 0;
int rows = 0;
vector<string> swapNames;
void swapGates( queue<string>& processWireNameQueue, map < string, pair<bool, string>>& swapped, bool& found, int depth = 4 ) {
    if( depth == 0 ) {
        if( runGates( processWireNameQueue ) == zOperandVal ) {
            for( auto wireInfo : swapped ) {
                if( wireInfo.second.first ) {
                    swapNames.push_back( wireInfo.first );
                }
            }
            found = true;
        }
        return;
    }
    for( auto wireInfo : allOutWireList ) {
        string wireName = wireInfo.first;
        if( !swapped[wireName].first ) {
            swapped[wireName].first = true;
            for( auto nextWire : allOutWireList ) {
                string nextWireName = nextWire.first;
                if( nextWireName != wireName && !swapped[nextWireName].first ) {
                    swapped[nextWireName].first = true;
                    swapWire( wireName, nextWireName );
                    swapGates( processWireNameQueue, swapped, found, depth - 1 );
                    if( found ) {
                        return;
                    }
                    swapWire( wireName, nextWireName );
                    swapped[nextWireName].first = false;
                    swapped[wireName].second = wireName;
                    swapped[nextWireName].second = nextWireName;
                }
            }
            swapped[wireName].first = false;
        }
        cout << ++res << "/" << 1ull * rows * rows * rows * rows << endl;
    }
}

void Solution2() {
    queue<string> processWireNameQueue = readFile();
    string xOperand = "", yOperand = "";
    for( auto wire : inputWireList ) {
        if( wire.first[0] == 'x' ) {
            xOperand = to_string( getWire( wire.first ).data ) + xOperand;
        }
        else if( wire.first[0] == 'y' ) {
            yOperand = to_string( getWire( wire.first ).data ) + yOperand;
        }
    }
    xOperandVal = stoll( xOperand, nullptr, 2 ), yOperandVal = stoll( yOperand, nullptr, 2 );
    zOperandVal = xOperandVal & yOperandVal;
    map <string, pair<bool, string>> swapped;
    for( auto outWireInfo : allOutWireList ) {
        rows++;
        swapped[outWireInfo.first] = { false,outWireInfo.first };
    }
    bool found = false;
    swapGates( processWireNameQueue, swapped, found );
    sort( swapNames.begin(), swapNames.end(), less<string>() );
    for_each( swapNames.begin(), swapNames.end(), []( string name ) {
        cout << name << " ";
    } );
    cout << endl;
    cout << "Found: " << res << endl;
}

int main() {
    auto time1 = chrono::high_resolution_clock::now();
    // Solution1();
    auto time2 = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>( time2 - time1 ).count() / 1000.0 << setprecision( 2 ) << endl;
    Solution2();
    auto time3 = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>( time3 - time2 ).count() / 1000.0 << setprecision( 2 ) << endl;
    return 0;
}