#include <bits/stdc++.h>

using namespace std;
#include <utils/SolutionBase.hpp>
class TinkerWithGates : public SolutionBase {
	REGISTER( TinkerWithGates )

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
        vector<tuple<string, string, string>> outWireList;
    };

    map<string, wire> wireList;
    map<string, tuple<string, string, string>> outWireList;

    void readFile() {
        ifstream input( "Day24/input.txt" );
        stringstream ss;
        ss << input.rdbuf();
        regex initWire( "([xy].+):\\s+([0-9])" );
        regex initGate( "\\b(.+)\\b\\s+(XOR|AND|OR)\\s+\\b(.+)\\b\\s+->\\s+(.+)" );
        for ( string buf; getline( ss, buf ); ) {
            smatch m;
            if ( regex_search( buf, m, initWire ) ) {
                wireList[m[1]].data = stoi( m[2] );
                wireList[m[1]].isValid = true;
            } else if ( regex_search( buf, m, initGate ) ) {
                string wire1Name = m[1], gateType = m[2], wire2Name = m[3], outWireName = m[4];
                vector<string> inputSort{ wire1Name, wire2Name };
                sort( inputSort.begin(), inputSort.end(), less<string>() );
                outWireList[outWireName] = { gateType, inputSort[0], inputSort[1] };
                wireList[wire1Name].outWireList.emplace_back( m[2], wire2Name, outWireName );
                wireList[wire2Name].outWireList.emplace_back( m[2], wire1Name, outWireName );
            }
        }
    }

    tuple<vector<string>, bool, bool> runGates( queue<string> wireQueue, map<string, wire>& wireList, bool staged = false, int curID = 0 ) {
        tuple<vector<string>, bool, bool> curProcessList;
        while ( !wireQueue.empty() ) {
            string curWireName = wireQueue.front();
            wire& curWire = wireList[curWireName];
            wireQueue.pop();
            if ( staged ) {
                get<0>( curProcessList ).push_back( curWireName );
                if ( curWireName[0] == 'z' ) {
                    get<1>( curProcessList ) = curWire.data;
                }
                if ( curID == 0 && get<0>( curProcessList ).size() == 4 ) {
                    return curProcessList;
                } else if ( curID != 0 && get<0>( curProcessList ).size() == 8 ) {
                    return curProcessList;
                }
            }
            for ( auto& gateInfo : curWire.outWireList ) {
                string outWireName = get<2>( gateInfo );
                string nextWireName = get<1>( gateInfo );
                wire& outWire = wireList[outWireName];
                wire& nextWire = wireList[nextWireName];
                if ( nextWire.isValid ) {
                    if ( !outWire.isValid ) {
                        outWire.isValid = true;
                        wireQueue.push( outWireName );
                        string operaterName = get<0>( gateInfo );
                        outWire.data = gateResult( operaterName, curWire.data, nextWire.data );
                    }
                }
            }
        }
        return curProcessList;
    }

   public:
    void Solution1() {
        readFile();
        queue<string> wireQueue;
        for ( auto& wire : wireList ) {
            if ( wire.first[0] == 'x' || wire.first[0] == 'y' ) {
                wireQueue.push( wire.first );
            }
        }
        runGates( wireQueue, wireList );
        string res = "";
        for ( auto& wireInfo : wireList ) {
            if ( wireInfo.first[0] == 'z' )
                res = to_string( wireInfo.second.data ) + res;
        }
        printRes( 1, stoll( res, nullptr, 2 ) );
    }

    string findGate( string input1, string input2, string gateType ) {
        for ( auto& wireInfo : outWireList ) {
            auto& outWireInfo = wireInfo.second;
            if ( get<0>( outWireInfo ) == gateType ) {
                if ( ( get<1>( outWireInfo ) == input1 && get<2>( outWireInfo ) == input2 ) || ( get<1>( outWireInfo ) == input2 && get<2>( outWireInfo ) == input1 ) ) {
                    return wireInfo.first;
                }
            }
        }
        return "";
    };

    void Solution2() {
        int CascadingDepth = 0;
        for ( auto const& wireInfo : wireList ) {
            if ( wireInfo.first[0] == 'x' )
                CascadingDepth++;
        }
        vector<string> swapList;
        string input1 = "x00", input2 = "y00", nextCarrier = "";
        for ( int i = 0; i < CascadingDepth; i++ ) {
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
        printRes( 2, accumulate( swapList.begin() + 1, swapList.end(), swapList[0], [&]( string const& init, string const& res ) {
                      return init + "," + res;
                  } ) );
    }
};