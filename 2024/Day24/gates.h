#include <bits/stdc++.h>
using namespace std;

bool gateResult( string op, bool operand1, bool operand2 ) {
    if( op == "AND" )
        return  operand1 & operand2;
    else if( op == "OR" )
        return  operand1 | operand2;
    else if( op == "XOR" )
        return  operand1 ^ operand2;
    else {
        cerr << "Error operator" << endl;
    }
    return false;
}

typedef struct wire wire;
typedef struct gate gate;

struct wire {
    wire( string _Name, bool _isValid, bool _data ) : wireName( _Name ), isValid( _isValid ), data( _data ) {};
    string wireName;
    bool isValid;
    bool data;
    vector<pair<pair<string, string>, string>> outWireList;
};

vector<wire> wireId2Wire;

map<string, int> wireName2Id;
map<string, int> allOutWireName2Id;
map<string, int> zResWire2Id;

void resetGates() {
    for( auto& w : wireId2Wire ) {
        if( w.wireName[0] != 'x' && w.wireName[0] != 'y' ) {
            w.isValid = false;
            w.data = false;
        }
    }
}

long long xOperandVal, yOperandVal, zOperandVal;

void createWire( string wireName, bool isValid = false, bool data = false ) {
    if( wireName2Id.count( wireName ) == 0 ) {
        wireName2Id[wireName] = wireId2Wire.size();
        wireId2Wire.push_back( wire( wireName, isValid, data ) );
        // TODO Add forward list to support emplace
    }
}

wire& getWire( string wireName ) {
    return wireId2Wire[wireName2Id[wireName]];
}

void swapWire( string wireName1, string wireName2 ) {
    // swap( wireName2Id[wireName1], wireName2Id[wireName2] );
    swap( getWire( wireName1 ), getWire( wireName2 ) );
}

queue<string> readFile() {
    queue<string> processWireNameQueue;
    ifstream input( "input.txt" );
    // ifstream input( "example.txt" );
    // ifstream input( "example2.txt" );
    // ifstream input( "example3.txt" );
    // ifstream input( "example4.txt" );
    string buf;
    regex initWire( "([xy].+):\\s+([0-9])" );
    regex initGate( "\\b(.+)\\b\\s+(XOR|AND|OR)\\s+\\b(.+)\\b\\s+->\\s+(.+)" );
    while( getline( input, buf ) ) {
        smatch m;
        if( regex_search( buf, m, initWire ) ) {
            createWire( m[1], true, stoi( m[2] ) );
            processWireNameQueue.push( m[1] );
        }
        else if( regex_search( buf, m, initGate ) ) {

            string wire1Name = m[1], wire2Name = m[3], outWireName = m[4];
            createWire( wire1Name );
            createWire( wire2Name );
            createWire( outWireName );
            allOutWireName2Id[outWireName] = wireName2Id[outWireName];
            if( outWireName[0] == 'z' ) {
                zResWire2Id[outWireName] = wireName2Id[outWireName];
            }
            getWire( wire1Name ).outWireList.push_back( { { m[2], wire2Name}, outWireName } );
            getWire( wire2Name ).outWireList.push_back( { { m[2], wire1Name}, outWireName } );

        }
    }
    return move( processWireNameQueue );
}

long long runGates( queue<string> processWireNameQueue ) {
    resetGates();
    map<string, int> inqueTimes;
    while( !processWireNameQueue.empty() ) {
        string curWireName = processWireNameQueue.front();
        wire& curWire = getWire( curWireName );
        processWireNameQueue.pop();
        for( auto gateInfo : curWire.outWireList ) {
            string outWireName = gateInfo.second;
            string nextWireName = gateInfo.first.second;
            wire& outWire = getWire( outWireName );
            wire& nextWire = getWire( nextWireName );
            if( nextWire.isValid ) {
                if( !outWire.isValid ) {
                    outWire.isValid = true;
                    processWireNameQueue.push( outWireName );
                    string operaterName = gateInfo.first.first;
                    outWire.data = gateResult( operaterName, curWire.data, nextWire.data );
                }
                else {
                    // pop;
                }
            }
            else {
                // Stop using the contianer's name;
                inqueTimes[curWireName]++;
                if( inqueTimes[curWireName] > 222 )return 0;
                processWireNameQueue.push( curWireName );
            }
        }
    }
    string res = "";
    for( auto& wireInfo : allOutWireName2Id ) {
        // wireInfo.second = getWire( wireInfo.first ).data;
        if( wireInfo.first[0] == 'z' )
            res = to_string( getWire( getWire( wireInfo.first ).wireName ).data ) + res;
    }
    // sort( zResWire2Id.begin(), zResWire2Id.end(), less<pair<string, bool>>() );
    return stoll( res, nullptr, 2 );
}