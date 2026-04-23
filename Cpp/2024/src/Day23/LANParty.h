#include <algorithm>
#include <fstream>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>

#include "utils/BufferedReader.hpp"
#include "utils/ISolution.hpp"
class LANParty : public ISolution {
    REGISTER( LANParty )

    void readFile() {
        using namespace std;
        for ( const string& buf : BufferedReader( "Day23/input.txt" ).lines().yield() ) {
            LANNetwork[buf.substr( 0, 2 )].emplace( buf.substr( 3 ) );
            LANNetwork[buf.substr( 3 )].emplace( buf.substr( 0, 2 ) );
        }
        return;
    }

    bool isConnected( std::string const& Host, std::set<std::string> const& ConnectionNetwork ) {
        for ( auto const& conn : ConnectionNetwork ) {
            if ( !LANNetwork.at( Host ).contains( conn ) ) {
                return false;
            }
        }
        return true;
    }
    std::map<std::string, std::set<std::string>> LANNetwork;

   public:
    void Solution1() {
        using namespace std;
        readFile();
        unordered_set<string> partySet;
        partySet.reserve( LANNetwork.size() );
        for ( auto& [netA, _] : LANNetwork ) {
            for ( auto& netB : LANNetwork.at( netA ) ) {
                for ( auto& netC : LANNetwork.at( netB ) ) {
                    if ( LANNetwork.at( netA ).contains( netC ) && ( netA.starts_with( 't' ) || netB.starts_with( 't' ) || netC.starts_with( 't' ) ) ) {
                        set party( { netA, netB, netC } );
                        partySet.insert( ranges::fold_left( party, "", plus<>{} ) );
                    }
                }
            }
        }
        printRes( 1, partySet.size() );
    }
    void Solution2() {
        using namespace std;
        vector<set<string>> connections;
        for ( auto& [atom, _] : LANNetwork ) {
            for ( auto& connection : connections ) {
                if ( isConnected( atom, connection ) ) {
                    connection.insert( atom );  // Don't break here.
                }
            }
            connections.emplace_back( set{ atom } );
        }
        set<string>& passWord = *ranges::max_element( connections, {}, []( const set<string>& conn ) {
            return conn.size();
        } );
        printRes( 2, ranges::fold_left( passWord, "", []( string const& acc, string const& host ) {
                      return acc.empty() ? host : ( acc + "," + host );
                  } ) );
    }
};