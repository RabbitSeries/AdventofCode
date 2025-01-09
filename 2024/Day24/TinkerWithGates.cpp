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

struct wire {
    wire() {};
    wire( bool _isValid, bool _data ) : isValid( _isValid ), data( _data ) {};
    bool isValid;
    bool data;
    vector<pair<pair<string, string>, string>> outWireList;
};

void resetGates( map<string, wire>& wireList ) {
    for( auto& w : wireList ) {
        if( w.first[0] != 'x' && w.first[0] != 'y' ) {
            wireList[w.first].isValid = false;
            wireList[w.first].data = false;
        }
    }
}

void readFile( map<string, wire>& wireList, map<string, tuple<string, string, string>>& outWireList ) {
    // ifstream input( "example.txt" );
    // ifstream input( "example2.txt" );
    // ifstream input( "example3.txt" );
    // ifstream input( "example4.txt" );
    ifstream input( "input.txt" );
    string buf;
    regex initWire( "([xy].+):\\s+([0-9])" );
    regex initGate( "\\b(.+)\\b\\s+(XOR|AND|OR)\\s+\\b(.+)\\b\\s+->\\s+(.+)" );
    while( getline( input, buf ) ) {
        smatch m;
        if( regex_search( buf, m, initWire ) ) {
            wireList[m[1]].data = stoi( m[2] );
            wireList[m[1]].isValid = true;
        }
        else if( regex_search( buf, m, initGate ) ) {
            string wire1Name = m[1], gateType = m[2], wire2Name = m[3], outWireName = m[4];
            vector<string> inputSort{ wire1Name,wire2Name };
            sort( inputSort.begin(), inputSort.end(), less<string>() );
            outWireList[outWireName] = { gateType,inputSort[0],inputSort[1] };
            wireList[wire1Name].outWireList.push_back( { { m[2], wire2Name}, outWireName } );
            wireList[wire2Name].outWireList.push_back( { { m[2], wire1Name}, outWireName } );
        }
    }
}

void runGates( map<string, wire>& wireList ) {
    resetGates( wireList );
    queue<string> wireQueue;
    for( auto wire : wireList ) {
        if( wire.first[0] == 'x' || wire.first[0] == 'y' ) {
            wireQueue.push( wire.first );
        }
    }
    while( !wireQueue.empty() ) {
        string curWireName = wireQueue.front();
        wire& curWire = wireList[curWireName];
        wireQueue.pop();
        for( auto gateInfo : curWire.outWireList ) {
            string outWireName = gateInfo.second;
            string nextWireName = gateInfo.first.second;
            wire& outWire = wireList[outWireName];
            wire& nextWire = wireList[nextWireName];
            if( nextWire.isValid ) {
                if( !outWire.isValid ) {
                    outWire.isValid = true;
                    wireQueue.push( outWireName );
                    string operaterName = gateInfo.first.first;
                    outWire.data = gateResult( operaterName, curWire.data, nextWire.data );
                }
            }
        }
    }
    return;
}

void Solution1() {
    map<string, wire> wireList; map<string, tuple<string, string, string>> outWireList;
    readFile( wireList, outWireList );
    runGates( wireList );
    string res = "";
    for( auto& wireInfo : wireList ) {
        if( wireInfo.first[0] == 'z' )
            res = to_string( wireInfo.second.data ) + res;
    }
    cout << "Solution 1: " << stoll( res, nullptr, 2 ) << endl;
}

void Solution2() {
    map<string, wire> wireList; map<string, tuple<string, string, string>> outWireList;
    readFile( wireList, outWireList );
    map<string, bool> swappedGateOutWire;
    // Start from x00,y00 search for its AND (carrier), XOR (Add) gate
    ofstream output( "mermaid.md", ios::out );
    output <<
        "---\ntitle: Node\n---\nflowchart TD\n";
    vector<string> xOutList;
    vector<string> yOutList;
    vector<string> otherOutList;
    for( auto wire : outWireList ) {
        auto [gateType, inWire1, inWire2] = wire.second;
        if( inWire1[0] == 'x' )
            xOutList.push_back( inWire1 + "-->" + wire.first + "\n" );
        else if( inWire1[0] == 'y' )
            yOutList.push_back( inWire1 + "-->" + wire.first + "\n" );
        else
            otherOutList.push_back( inWire1 + "-->" + wire.first + "\n" );
        if( inWire2[0] == 'x' )
            xOutList.push_back( inWire2 + "-->" + wire.first + "\n" );
        else if( inWire2[0] == 'y' )
            yOutList.push_back( inWire2 + "-->" + wire.first + "\n" );
        else
            otherOutList.push_back( inWire2 + "-->" + wire.first + "\n" );
        // output << inWire1 << "-->" << wire.first << endl;
        // output << inWire2 << "-->" << wire.first << endl;
        // output << gateType + wire.first << "-->" << wire.first << endl;
    }
    sort( xOutList.begin(), xOutList.end(), less<string>() );
    sort( yOutList.begin(), yOutList.end(), less<string>() );
    sort( otherOutList.begin(), otherOutList.end(), less<string>() );
    for( auto line : xOutList ) {
        output << line;
    }
    for( auto line : yOutList ) {
        output << line;
    }
    for( auto line : otherOutList ) {
        output << line;
    }
}

int main() {
    auto time1 = chrono::high_resolution_clock::now();
    Solution1();
    auto time2 = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>( time2 - time1 ).count() / 1000.0 << setprecision( 2 ) << endl;
    Solution2();
    auto time3 = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>( time3 - time2 ).count() / 1000.0 << setprecision( 2 ) << endl;
    return 0;
}