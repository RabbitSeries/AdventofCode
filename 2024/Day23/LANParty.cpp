#include <bits/stdc++.h>
using namespace std;
map<string, map<string, bool>> readFile() {
    map<string, map<string, bool>> LANNetwork;
    ifstream input( "input.txt" );
    string buf;
    while( getline( input, buf ) ) {
        regex re( "([a-z]+)-([a-z]+)" );
        smatch m;
        regex_search( buf, m, re );
        if( m.size() == 3 ) {
            string pc1 = m[1], pc2 = m[2];
            LANNetwork[pc1][pc2] = true;
            LANNetwork[pc2][pc1] = true;
        }
    }
    return move( LANNetwork );
}

map<string, string> connections;
mutex connectionsMutex;

inline bool isConnected( string const& Host, string const& ConnectionNextwork, map<string, map<string, bool>> const& LANNetWork ) {
    bool connected = true;
    for( int i = 0; i < ConnectionNextwork.size(); i += 2 ) {
        if( LANNetWork.at( Host ).count( ConnectionNextwork.substr( i, 2 ) ) == 0 ) {
            connected = false;
            return false;
        }
    }
    return connected;
}

inline bool checkUnique( string const& ConnectionNextwork, map<string, string> const& connections ) {
    vector<string> enumeration = { ConnectionNextwork.substr( 0,2 ),ConnectionNextwork.substr( 2,2 ),ConnectionNextwork.substr( 4,2 ) };
    sort( enumeration.begin(), enumeration.end(), less<string>() );
    bool isUnique = true;
    do {
        if( connections.count( enumeration[0] + enumeration[1] + enumeration[2] ) != 0 ) {
            isUnique = false;
            return isUnique;
        }
    } while( next_permutation( enumeration.begin(), enumeration.end() ) );
    return isUnique;
}


int findConnection( map<string, map<string, bool>> const& LANNetWork, string Host, string curConnection = "", int depth = 3 ) {
    // void findConnection( map<string, map<string, bool>> const& LANNetWork, string Host, string & curConnection = "", int depth = 3 ) {
    curConnection += Host;
    if( depth == 1 ) {
        if( connections.count( curConnection ) == 0 && checkUnique( curConnection, connections ) ) {
            lock_guard<mutex> writeConnections( connectionsMutex );
            connections[curConnection] = curConnection.substr( 0, 1 ) + curConnection.substr( 2, 1 ) + curConnection.substr( 4, 1 );
        }
        // curConnection.pop_back();
        // curConnection.pop_back();
        return connections.size();
    }
    auto const& subNetwork = LANNetWork.at( Host );
    for( auto nextPC : subNetwork ) {
        if( curConnection.find( nextPC.first ) == string::npos ) {
            // curConnection += nextPC.first;
            if( isConnected( nextPC.first, curConnection, LANNetWork ) )
                findConnection( LANNetWork, nextPC.first, curConnection, depth - 1 );
            // curConnection.pop_back();
            // curConnection.pop_back();
        }
    }
    return connections.size();
}


void threadTask( int startRow, int endRow, map<string, map<string, bool>> const& LANNetWork ) {
    std::map<std::string, std::map<std::string, bool>>::const_iterator it = LANNetWork.begin(), end = LANNetWork.end();
    // auto it = LANNetWork.begin();
    while( distance( it, LANNetWork.end() ) != endRow - startRow ) {
        it++;
    }
    int left = 0, right = LANNetWork.size() - 1;

    while( it != end ) {
        // findConnection( LANNetWork, ( *it ).first, ( *it ).first );
        findConnection( LANNetWork, ( *it ).first );
        it++;
    }
}

void Solution1() {
    map<string, map<string, bool>> LANNetwork = readFile();
    bool enableMultiThreading = false;
    int maxThread = thread::hardware_concurrency();
    int processPerThread = LANNetwork.size() / maxThread;
    vector<thread> threadList;
    if( enableMultiThreading ) {
        for( int i = 0; i < maxThread; i++ ) {
            int startRow = i * processPerThread;
            int endRow = ( i == maxThread - 1 ) ? LANNetwork.size() : ( i + 1 ) * processPerThread;
            threadList.push_back( thread( threadTask, startRow, endRow, ref( LANNetwork ) ) );
        }
    }
    else {
        threadList.push_back( thread( threadTask, 0, LANNetwork.size(), ref( LANNetwork ) ) );
    }
    for( auto& t : threadList ) {
        t.join();
    }
    int res = 0;
    for( auto const& [connection, Prefix] : connections ) {
        if( Prefix.find( 't' ) != string::npos ) {
            res++;
        }
    }
    cout << "Solution 1: " << res << endl;
}

void Solution2() {
    connections.clear();
    map<string, map<string, bool>> LANNetwork = readFile();
    std::map<std::string, std::map<std::string, bool>>::const_iterator it = LANNetwork.begin(), end = LANNetwork.end();
    size_t res = 0;
    while( it != end ) {
        // cout << "Checking: " << distance( std::map<std::string, std::map<std::string, bool>>::const_iterator( LANNetwork.begin() ), it ) << endl;
        string atom = ( *it ).first;
        if( connections.empty() ) {
            connections.emplace( atom, atom );
        }
        else {
            bool added = false;
            for( auto& [Host, connection] : connections ) {
                if( isConnected( atom, connection, LANNetwork ) ) {
                    connection += atom;
                    // Don't break here.
                    added = true;
                }
            }
            if( !added )
                connections.emplace( atom, atom );
        }
        it++;
    }
    string maxConnection = "";
    for( auto const& [Host, connection] : connections ) {
        if( connection.size() / 2 > res ) {
            maxConnection = connection;
        }
        res = max( connection.size() / 2, res );
    }
    vector<string> passWord;
    for( int i = 0; i < maxConnection.size(); i += 2 ) {
        passWord.push_back( maxConnection.substr( i, 2 ) );
    }
    cout << "Solution 2: ";
    sort( passWord.begin(), passWord.end(), less<string>() );
    cout << passWord[0];
    for_each( passWord.begin() + 1, passWord.end(), []( string host ) {
        cout << "," << host;
    } );
    cout << endl;
    return;
}

int main() {
    auto now = chrono::high_resolution_clock::now();
    Solution1();
    Solution2();
    auto end = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>( end - now ).count() / 1000.0 << setprecision( 2 ) << endl;
    return 0;
}