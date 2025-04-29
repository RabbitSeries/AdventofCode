#include <bits/stdc++.h>
using namespace std;
#include "../../utils/SolutionBase.h"
class LANParty : public SolutionBase {
    void readFile() {
        ifstream input( "Day23/input.txt" );
        for ( string buf; getline( input, buf ); ) {
            regex re( "([a-z]+)-([a-z]+)" );
            smatch m;
            regex_search( buf, m, re );
            if ( m.size() == 3 ) {
                string pc1 = m[1], pc2 = m[2];
                LANNetwork[pc1].insert( pc2 );
                LANNetwork[pc2].insert( pc1 );
            }
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
            if ( connections.empty() ) {
                connections.emplace_back( set( { atom } ) );
            } else {
                bool added = false;
                for ( auto& connection : connections ) {
                    if ( isConnected( atom, connection ) ) {
                        connection.insert( atom );
                        // Don't break here.
                        added = true;
                    }
                }
                if ( !added )
                    connections.emplace_back( set( { atom } ) );
            }
        }
        set<string> passWord = *ranges::max_element( connections.begin(), connections.end(), {}, []( decltype( connections )::value_type const& conn ) {
            return conn.size();
        } );
        printRes( 2, accumulate( ++passWord.begin(), passWord.end(), *passWord.begin(), []( string const& init, string const& host ) {
                      return init + "," + host;
                  } ) );
        return;
    }
};