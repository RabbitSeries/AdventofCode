#include <algorithm>
#include <fstream>
#include <iomanip>
#include <queue>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "utils/ISolution.hpp"
class TinkerWithGates : public ISolution {
    REGISTER( TinkerWithGates )

    using Str = std::string;
    bool gateResult( Str const& op, bool operand1, bool operand2 ) {
        if ( op == "AND" ) {
            return operand1 & operand2;
        } else if ( op == "OR" ) {
            return operand1 | operand2;
        } else if ( op == "XOR" ) {
            return operand1 ^ operand2;
        } else {
            return false;
        }
    }
    struct Wire {
        Wire( bool _data = false ) : data( _data ) {};
        bool data;
        bool operator<( Wire const& rhs ) const { return false; }
        std::vector<std::tuple<Str, Str, Str>> outwires;
    };
    std::unordered_map<Str, Wire> WireList;
    std::unordered_map<Str, std::tuple<Str, Str, Str>> WireInputs;

    void readFile() {
        using namespace std;
        stringstream ss;
        ss << ifstream( "Day24/input.txt" ).rdbuf();
        regex wireReg( R"(([xy]\d+):\s+([01]))" );
        regex gateReg( R"(^(\w+)\s+(XOR|AND|OR)\s+(\w+)\s+->\s+(\w+)$)" );
        for ( Str buf; getline( ss, buf ); ) {
            smatch m;
            if ( buf.empty() ) {
                break;
            }
            if ( regex_search( buf, m, wireReg ) ) {
                WireList[m[1]].data = std::stoi( m[2] );
            }
        }
        for ( Str buf; getline( ss, buf ); ) {
            smatch m;
            if ( regex_search( buf, m, gateReg ) ) {
                Str wire1 = m[1], gate = m[2], wire2 = m[3], outputWire = m[4];
                WireInputs[outputWire] = { gate, wire1, wire2 };
                WireList[wire1].outwires.emplace_back( m[2], wire2, outputWire );
                WireList[wire2].outwires.emplace_back( m[2], wire1, outputWire );
            }
        }
    }

    void runGates() {
        std::queue<Str> wireQueue;
        std::unordered_set<Str> validWires;
        for ( auto& [wireName, _] : WireList ) {
            if ( wireName.starts_with( 'x' ) || wireName.starts_with( 'y' ) ) {
                wireQueue.emplace( wireName );
                validWires.emplace( wireName );
            }
        }
        while ( !wireQueue.empty() ) {
            Wire& input1 = WireList[wireQueue.front()];
            wireQueue.pop();
            for ( auto& [gate, input2Name, output] : input1.outwires ) {
                Wire& outWire = WireList[output];
                Wire& input2 = WireList[input2Name];
                if ( validWires.contains( input2Name ) ) {
                    if ( !validWires.contains( output ) ) {
                        validWires.emplace( output );
                        wireQueue.emplace( output );
                        outWire.data = gateResult( gate, input1.data, input2.data );
                    }
                }
            }
        }
    }

    Str findGate( Str const& input1, Str const& input2, Str const& gateType ) {
        for ( auto& wireInfo : WireInputs ) {
            auto& [gate, wire1, wire2] = wireInfo.second;
            if ( gate == gateType ) {
                if ( ( wire1 == input1 && wire2 == input2 ) || ( wire1 == input2 && wire2 == input1 ) ) {
                    return wireInfo.first;
                }
            }
        }
        return "";
    };

   public:
    void Solution1() {
        readFile();
        runGates();
        Str res = "";
        using WireInfo = std::unordered_map<Str, Wire>::value_type;
        std::set zWires = WireList | std::views::filter( []( const WireInfo& info ) {
                              return info.first.starts_with( 'z' );
                          } ) |
                          std::ranges::to<std::set<WireInfo>>();
        for ( auto& [_, wire] : zWires ) {
            res = std::to_string( wire.data ) + res;
        }
        printRes( 1, std::stoull( res, nullptr, 2 ) );
    }

    void Solution2() {
        int CascadingDepth = 0;
        for ( auto const& wireInfo : WireList ) {
            if ( wireInfo.first[0] == 'x' )
                CascadingDepth++;
        }
        std::vector<Str> swapList;
        Str input1 = "x00", input2 = "y00", nextCarrier = "";
        for ( int i = 0; i < CascadingDepth; i++ ) {
            // Add XOR
            Str add1 = findGate( input1, input2, "XOR" );
            // Carrier AND
            Str carrier1 = findGate( input1, input2, "AND" );

            if ( nextCarrier.empty() ) {
                // Possible swap situation 1.
                if ( add1 != "z" + input1.substr( 1 ) ) {
                    swapList.push_back( add1 );
                    swapList.push_back( carrier1 );
                    std::tie( add1, carrier1 ) = { carrier1, add1 };
                }
                // The first adder. No further checking.
                nextCarrier = carrier1;
            } else {
                // Carrier Add
                Str add2 = findGate( nextCarrier, add1, "XOR" );
                if ( add2.empty() ) {
                    // Possible swap situation 2.
                    std::swap( add1, carrier1 );
                    add2 = findGate( nextCarrier, add1, "XOR" );
                    swapList.push_back( add1 );
                    swapList.push_back( carrier1 );
                } else if ( carrier1 == "z" + input1.substr( 1 ) ) {
                    // Possible swap situation 3.
                    std::swap( add2, carrier1 );
                    swapList.push_back( add2 );
                    swapList.push_back( carrier1 );
                }
                // Carrier Add's Carrier
                Str carrier2 = findGate( nextCarrier, add1, "AND" );
                if ( carrier2 == "z" + input1.substr( 1 ) ) {
                    // Possible swap situation 4.
                    std::swap( add2, carrier2 );
                    swapList.push_back( add2 );
                    swapList.push_back( carrier2 );
                }
                // Next Carrier
                nextCarrier = findGate( carrier2, carrier1, "OR" );
                if ( nextCarrier == "z" + input1.substr( 1 ) ) {
                    // Possible swap situation 5.
                    std::swap( add2, nextCarrier );
                    swapList.push_back( add2 );
                    swapList.push_back( nextCarrier );
                }
            }
            std::stringstream nextIndex;
            nextIndex << std::setw( 2 ) << std::setfill( '0' ) << i + 1;
            input1 = "x" + nextIndex.str();
            input2 = "y" + nextIndex.str();
        }
        std::ranges::sort( swapList, std::less<>{} );
        printRes( 2, std::ranges::fold_left_first( swapList, [&]( Str const& cat, Str const& res ) {
                         return cat + "," + res;
                     } ).value_or( "" ) );
    }
};