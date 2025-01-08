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
    wire( bool _isValid, bool _data ) : isValid( _isValid ), data( _data ) {};
    bool isValid;
    bool data;
    vector<pair<pair<string, string>, string>> outWireList;
};

map<string, wire> inputWireList;
map<string, wire> allOutWireList;

void resetGates() {
    for( auto& w : allOutWireList ) {
        w.second.isValid = false;
        w.second.data = false;
    }
}

long long xOperandVal, yOperandVal, zOperandVal;

void createWire( string wireName, bool isValid = false, bool data = false ) {
    if( inputWireList.count( wireName ) == 0 ) {
        inputWireList[wireName] = wire( isValid, data );
        // TODO Add forward list to support emplace
        // wireId2Wire.push_back( wire( wireName, isValid, data ) );
    }
}

wire& getWire( string wireName ) {
    if( inputWireList.count( wireName ) != 0 ) {
        return inputWireList[wireName];
    }
    else {
        return allOutWireList[wireName];
    }
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
            allOutWireList[outWireName] = inputWireList[outWireName];
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
    for( auto& wireInfo : allOutWireList ) {
        // wireInfo.second = getWire( wireInfo.first ).data;
        if( wireInfo.first[0] == 'z' )
            res = to_string( getWire( getWire( wireInfo.first ) ).data ) + res;
    }
    // sort( zResWire2Id.begin(), zResWire2Id.end(), less<pair<string, bool>>() );
    return stoll( res, nullptr, 2 );
}