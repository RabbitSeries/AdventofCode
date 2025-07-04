#include <bits/stdc++.h>
using namespace std;
#include <utils/SolutionBase.hpp>
class LANParty : public SolutionBase {
    void readFile() {
        stringstream input;
        input << ifstream( "Day23/input.txt" ).rdbuf();
        for ( string buf; getline( input, buf ); ) {
            LANNetwork[buf.substr( 0, 2 )].emplace( buf.substr( 3 ) );
            LANNetwork[buf.substr( 3 )].emplace( buf.substr( 0, 2 ) );
        }
        return;
    }

    bool isConnected( string const& Host, set<string> const& ConnectionNetwork ) {
        for ( auto const& conn : ConnectionNetwork ) {
            if ( !LANNetwork.at( Host ).contains( conn ) ) {
                return false;
            }
        }
        return true;
    }
    map<string, set<string>> LANNetwork;

   public:
    void Solution1() {
        readFile();
        unordered_set<string> partySet;
        partySet.reserve( LANNetwork.size() );
        for ( auto& [netA, _] : LANNetwork ) {
            for ( auto& netB : LANNetwork.at( netA ) ) {
                for ( auto& netC : LANNetwork.at( netB ) ) {
                    if ( LANNetwork.at( netA ).contains( netC ) && ( netA.starts_with( 't' ) || netB.starts_with( 't' ) || netC.starts_with( 't' ) ) ) {
                        set party( { netA, netB, netC } );
                        partySet.insert( accumulate( party.begin(), party.end(), string() ) );
                    }
                }
            }
        }
        printRes( 1, partySet.size() );
    }
    void Solution2() {
        vector<set<string>> connections;
        for ( auto& [atom, _] : LANNetwork ) {
            for ( auto& connection : connections ) {
                if ( isConnected( atom, connection ) ) {
                    connection.insert( atom );  // Don't break here.
                    continue;
                }
            }
            connections.emplace_back( set( { atom } ) );
        }
        set<string> passWord = move( const_cast<set<string>&>( *ranges::max_element( connections.begin(), connections.end(), {}, []( decltype( connections )::value_type const& conn ) {
            return conn.size();
        } ) ) );
        printRes( 2, accumulate( ++passWord.begin(), passWord.end(), *passWord.begin(), []( string const& init, string const& host ) {
                      return init + "," + host;
                  } ) );
        return;
    }
};