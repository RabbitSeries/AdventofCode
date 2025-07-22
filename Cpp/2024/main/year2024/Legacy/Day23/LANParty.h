#include <bits/stdc++.h>
using namespace std;
#include <utils/SolutionBase.hpp>
class LANParty : public SolutionBase {
	static inline Register<LANParty> _reg{};

    void readFile() {
        ifstream input( "Day23/input.txt" );
        for ( string buf; getline( input, buf ); ) {
            regex re( "([a-z]+)-([a-z]+)" );
            smatch m;
            regex_search( buf, m, re );
            if ( m.size() == 3 ) {
                string pc1 = m[1], pc2 = m[2];
                LANNetwork[pc1].emplace( pc2 );
                LANNetwork[pc2].emplace( pc1 );
            }
        }
        input.close();
    }
    map<string, set<string>> LANNetwork;
    generator<set<string>> BronKerbosch1( set<string> R, set<string> P, set<string> X ) {
        if ( P.empty() && X.empty() ) {
            co_yield R;
        }
        while ( !P.empty() ) {
            set nextR = R;
            vector<string> nextP, nextX;
            string v = *P.begin();
            set_intersection( LANNetwork.at( v ).begin(), LANNetwork.at( v ).end(), P.begin(), P.end(), back_inserter( nextP ) );
            set_intersection( LANNetwork.at( v ).begin(), LANNetwork.at( v ).end(), X.begin(), X.end(), back_inserter( nextX ) );
            nextR.insert( *P.begin() );
            for ( set<string> res : BronKerbosch1( nextR, set( nextP.begin(), nextP.end() ), set( nextX.begin(), nextX.end() ) ) ) {
                co_yield res;
            }
            P.erase( P.begin() );
            X.insert( v );
        }
    }
    bool isConnected( string const& Host, string const& ConnectionNetwork, map<string, set<string>> const& LANNetWork ) {
        bool connected = true;
        for ( size_t i = 0; i < ConnectionNetwork.size(); i += 2 ) {
            if ( LANNetWork.at( Host ).count( ConnectionNetwork.substr( i, 2 ) ) == 0 ) {
                connected = false;
                return false;
            }
        }
        return connected;
    }

    bool checkUnique( string const& ConnectionNetwork, map<string, string> const& connections ) {
        vector<string> enumeration = { ConnectionNetwork.substr( 0, 2 ), ConnectionNetwork.substr( 2, 2 ), ConnectionNetwork.substr( 4, 2 ) };
        sort( enumeration.begin(), enumeration.end() );
        bool isUnique = true;
        do {
            if ( connections.count( enumeration[0] + enumeration[1] + enumeration[2] ) != 0 ) {
                isUnique = false;
                return isUnique;
            }
        } while ( next_permutation( enumeration.begin(), enumeration.end() ) );
        return isUnique;
    }
    int findConnection( map<string, string>& connections, mutex& connectionsMutex, map<string, set<string>> const& LANNetWork, string Host, string curConnection = "", int depth = 3 ) {
        // void findConnection( map<string, map<string, bool>> const& LANNetWork, string Host, string & curConnection = "", int depth = 3 ) {
        curConnection += Host;
        if ( depth == 1 ) {
            if ( connections.count( curConnection ) == 0 && checkUnique( curConnection, connections ) ) {
                lock_guard<mutex> writeConnections( connectionsMutex );
                connections[curConnection] = curConnection.substr( 0, 1 ) + curConnection.substr( 2, 1 ) + curConnection.substr( 4, 1 );
            }
            // curConnection.pop_back();
            // curConnection.pop_back();
            return connections.size();
        }
        auto const& subNetwork = LANNetWork.at( Host );
        for ( auto const& nextPC : subNetwork ) {
            if ( curConnection.find( nextPC ) == string::npos ) {
                // curConnection += nextPC.first;
                if ( isConnected( nextPC, curConnection, LANNetWork ) )
                    findConnection( connections, connectionsMutex, LANNetWork, nextPC, curConnection, depth - 1 );
                // curConnection.pop_back();
                // curConnection.pop_back();
            }
        }
        return connections.size();
    }

    void threadTask( int startRow, int endRow, map<string, string>& connections, mutex& connectionsMutex, map<string, set<string>> const& LANNetWork ) {
        auto it = LANNetWork.cbegin(), end = LANNetWork.cend();
        // auto it = LANNetWork.begin();
        while ( distance( it, LANNetWork.end() ) != endRow - startRow ) {
            it++;
        }
        // int left = 0, right = LANNetWork.size() - 1;

        while ( it != end ) {
            // findConnection( LANNetWork, ( *it ).first, ( *it ).first );
            findConnection( connections, connectionsMutex, LANNetWork, ( *it ).first );
            it++;
        }
    }

   public:
    void Solution1() {
        readFile();
        unordered_set<string> partySet;
        for ( auto& [netA, _] : LANNetwork ) {
            for ( auto& netB : LANNetwork.at( netA ) ) {
                for ( auto& netC : LANNetwork.at( netB ) ) {
                    if ( LANNetwork.at( netA ).contains( netC ) && ( netA.substr( 0, 1 ) + netB.substr( 0, 1 ) + netC.substr( 0, 1 ) ).find( 't' ) != string::npos ) {
                        set party( { netA, netB, netC } );
                        partySet.insert( accumulate( party.begin(), party.end(), string() ) );
                    }
                }
            }
        }
        printRes( 1, partySet.size() );
    }
    // Some kind of https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm#Without_pivoting
    void Solution2() {
        set<string> P;
        for ( auto& [k, _] : LANNetwork ) P.insert( k );
        set<pair<int, string>, greater<>> clique;
        for ( set<string> cliq : BronKerbosch1( {}, P, {} ) ) {
            clique.emplace( cliq.size(), accumulate(
                                             ++cliq.begin(),
                                             cliq.end(),
                                             *cliq.begin(),
                                             []( string const& init, string const& v ) {
                                                 return init + "," + v;
                                             } ) );
        }
        printRes( 2, clique.begin()->second );
    }
};