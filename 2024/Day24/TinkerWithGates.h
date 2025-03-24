#include <bits/stdc++.h>

using namespace std;
class TinkerWithGates {
    bool gateResult( string op, bool operand1, bool operand2 ) {
        if ( op == "AND" )
            return operand1 & operand2;
        else if ( op == "OR" )
            return operand1 | operand2;
        else if ( op == "XOR" )
            return operand1 ^ operand2;
        else {
            cerr << "Error operator" << endl;
        }
        return false;
    }

    struct wire {
        wire() {};
        wire( bool _isValid, bool _data ) : isValid( _isValid ), data( _data ) {};
        //! Bool is not one bit, it is one entire byte, always init data member!!!!!!!
        bool isValid = false;
        bool data = false;
        vector<pair<pair<string, string>, string>> outWireList;
    };

    void resetGates( map<string, wire>& wireList ) {
        for ( auto& w : wireList ) {
            if ( w.first[0] != 'x' && w.first[0] != 'y' ) {
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
        ifstream input( "Day24/input.txt" );
        regex initWire( "([xy].+):\\s+([0-9])" );
        regex initGate( "\\b(.+)\\b\\s+(XOR|AND|OR)\\s+\\b(.+)\\b\\s+->\\s+(.+)" );
        for ( string buf; getline( input, buf ); ) {
            smatch m;
            if ( regex_search( buf, m, initWire ) ) {
                wireList[m[1]].data = stoi( m[2] );
                wireList[m[1]].isValid = true;
            } else if ( regex_search( buf, m, initGate ) ) {
                string wire1Name = m[1], gateType = m[2], wire2Name = m[3], outWireName = m[4];
                vector<string> inputSort{ wire1Name, wire2Name };
                sort( inputSort.begin(), inputSort.end(), less<string>() );
                outWireList[outWireName] = { gateType, inputSort[0], inputSort[1] };
                wireList[wire1Name].outWireList.push_back( { { m[2], wire2Name }, outWireName } );
                wireList[wire2Name].outWireList.push_back( { { m[2], wire1Name }, outWireName } );
            }
        }
    }

    tuple<vector<string>, bool, bool> runGates( queue<string> wireQueue, map<string, wire>& wireList, bool staged = false, int curID = 0 ) {
        resetGates( wireList );
        bool addFound = false, carrierFound = false;
        tuple<vector<string>, bool, bool> curProcessList;
        while ( !wireQueue.empty() ) {
            string curWireName = wireQueue.front();
            wire& curWire = wireList[curWireName];
            wireQueue.pop();
            if ( staged ) {
                get<0>( curProcessList ).push_back( curWireName );
                if ( curWireName[0] == 'z' ) {
                    addFound = true;
                    get<1>( curProcessList ) = curWire.data;
                }
                if ( curID == 0 && get<0>( curProcessList ).size() == 4 ) {
                    return curProcessList;
                } else if ( curID != 0 && get<0>( curProcessList ).size() == 8 ) {
                    return curProcessList;
                }
            }
            for ( auto gateInfo : curWire.outWireList ) {
                string outWireName = gateInfo.second;
                string nextWireName = gateInfo.first.second;
                wire& outWire = wireList[outWireName];
                wire& nextWire = wireList[nextWireName];
                if ( nextWire.isValid ) {
                    if ( !outWire.isValid ) {
                        outWire.isValid = true;
                        wireQueue.push( outWireName );
                        string operaterName = gateInfo.first.first;
                        outWire.data = gateResult( operaterName, curWire.data, nextWire.data );
                    }
                }
            }
        }
        return curProcessList;
    }

    // TODO Someone tells me to permutate by level.
    void Solution2_Permutate() {
        map<string, wire> wireList;
        map<string, tuple<string, string, string>> outWireList;
        readFile( wireList, outWireList );
        map<string, string> binaryDigits;
        for ( auto wireInfo : wireList ) {
            if ( wireInfo.first[0] == 'x' )
                binaryDigits[wireInfo.first] = 'y' + wireInfo.first.substr( 1 );
        }
        // This queue
        queue<string> wireQueue;
        wireQueue.push( "x00" );
        resetGates( wireList );
        string input1 = "x00", input2 = "y00", carrier = "";
        tuple<vector<string>, bool, bool> curProcessList;
        vector<string> swapList;

        // while( stoi( input1.substr( 1 ) ) < binaryDigits.size() ) {
        //     curProcessList = runGates( wireQueue, wireList, true );
        //     bool res = carrier.empty() ? ( wireList[input1].data ^ wireList[input2].data ) : ( wireList[input1].data ^ wireList[input2].data ^ wireList[carrier].data );

        //     if( res != curWire.data )
        //         permutateAndSort( curProcessList, swapList, wireList, carrier, stoi( input1.substr( 1 ) ) );
        // }
    }

   public:
    void Solution1() {
        map<string, wire> wireList;
        map<string, tuple<string, string, string>> outWireList;
        readFile( wireList, outWireList );
        // This queue
        queue<string> wireQueue;
        for ( auto wire : wireList ) {
            if ( wire.first[0] == 'x' || wire.first[0] == 'y' ) {
                wireQueue.push( wire.first );
            }
        }
        runGates( wireQueue, wireList );
        string res = "";
        for ( auto wireInfo : wireList ) {
            if ( wireInfo.first[0] == 'z' )
                res = to_string( wireInfo.second.data ) + res;
        }
        cout << "-" << res << "-" << endl;
        cout << "Solution 1: " << stoll( res, nullptr, 2 ) << endl;
    }

    void Solution2() {
        map<string, wire> wireList;
        map<string, tuple<string, string, string>> outWireList;
        readFile( wireList, outWireList );
        map<string, string> binaryDigits;
        vector<string> swapList;
        for ( auto wireInfo : wireList ) {
            if ( wireInfo.first[0] == 'x' )
                binaryDigits[wireInfo.first] = 'y' + wireInfo.first.substr( 1 );
        }
        auto findGate = [&]( string input1, string input2, string gateType ) -> string {
            for ( auto wireInfo : outWireList ) {
                auto outWireInfo = wireInfo.second;
                if ( get<0>( outWireInfo ) == gateType ) {
                    if ( ( get<1>( outWireInfo ) == input1 && get<2>( outWireInfo ) == input2 ) || ( get<1>( outWireInfo ) == input2 && get<2>( outWireInfo ) == input1 ) ) {
                        return wireInfo.first;
                    }
                }
            }
            return "";
        };
        string input1 = "x00", input2 = "y00", nextCarrier = "";
        for ( int i = 0; i < binaryDigits.size(); i++ ) {
            // Add XOR
            string add1 = findGate( input1, input2, "XOR" );
            // Carrier AND
            string carrier1 = findGate( input1, input2, "AND" );

            if ( nextCarrier.empty() ) {
                // Possible swap situation 1.
                if ( add1 != "z" + input1.substr( 1 ) ) {
                    swapList.push_back( add1 );
                    swapList.push_back( carrier1 );
                    tie( add1, carrier1 ) = { carrier1, add1 };
                }
                // The first adder. No further checking.
                nextCarrier = carrier1;
            } else {
                // Carrier Add
                string add2 = findGate( nextCarrier, add1, "XOR" );
                if ( add2.empty() ) {
                    // Possible swap situation 2.
                    swap( add1, carrier1 );
                    // tie( add1, carrier1 ) = { carrier1, add1 };
                    add2 = findGate( nextCarrier, add1, "XOR" );
                    swapList.push_back( add1 );
                    swapList.push_back( carrier1 );
                } else if ( carrier1 == "z" + input1.substr( 1 ) ) {
                    // Possible swap situation 3.
                    swap( add2, carrier1 );
                    swapList.push_back( add2 );
                    swapList.push_back( carrier1 );
                }
                // Carrier Add's Carrier
                string carrier2 = findGate( nextCarrier, add1, "AND" );
                if ( carrier2 == "z" + input1.substr( 1 ) ) {
                    // Possible swap situation 4.
                    swap( add2, carrier2 );
                    swapList.push_back( add2 );
                    swapList.push_back( carrier2 );
                }
                // Next Carrier
                nextCarrier = findGate( carrier2, carrier1, "OR" );
                if ( nextCarrier == "z" + input1.substr( 1 ) ) {
                    // Possible swap situation 5.
                    swap( add2, nextCarrier );
                    swapList.push_back( add2 );
                    swapList.push_back( nextCarrier );
                }
            }
            stringstream nextIndex;
            nextIndex << setw( 2 ) << setfill( '0' ) << i + 1;
            input1 = "x" + nextIndex.str();
            input2 = "y" + nextIndex.str();
        }
        sort( swapList.begin(), swapList.end(), less<string>() );
        cout << swapList[0];
        for_each( swapList.begin() + 1, swapList.end(), []( string res ) {
            cout << "," << res;
        } );
        cout << endl;
    }
};