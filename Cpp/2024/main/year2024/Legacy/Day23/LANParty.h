#include <algorithm>
#include <fstream>
#include <functional>
#include <generator>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "utils/ISolution.hpp"
class LANParty : public ISolution {
    REGISTER( LANParty )
    void readFile() {
        std::ifstream input( "Day23/input.txt" );
        for ( std::string buf; getline( input, buf ); ) {
            std::regex re( "([a-z]+)-([a-z]+)" );
            std::smatch m;
            if ( std::regex_search( buf, m, re ) ) {
                std::string pc1 = m[1], pc2 = m[2];
                LANNetwork[pc1].emplace( pc2 );
                LANNetwork[pc2].emplace( pc1 );
            }
        }
    }
    std::map<std::string, std::set<std::string>> LANNetwork;
    // Some kind of https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm#Without_pivoting, but slow!!!!
    std::generator<std::set<std::string>> BronKerbosch1( std::set<std::string> nextR, std::set<std::string> P, std::set<std::string> X ) {
        if ( P.empty() && X.empty() ) {
            co_yield nextR;
        }
        while ( !P.empty() ) {
            std::vector<std::string> nextP, nextX;
            std::string v = *P.begin();
            std::set_intersection( LANNetwork.at( v ).begin(), LANNetwork.at( v ).end(), P.begin(), P.end(), back_inserter( nextP ) );
            std::set_intersection( LANNetwork.at( v ).begin(), LANNetwork.at( v ).end(), X.begin(), X.end(), back_inserter( nextX ) );
            nextR.insert( *P.begin() );
            for ( const std::set<std::string>& res : BronKerbosch1( nextR, std::set( std::from_range_t{}, P ), std::set( std::from_range_t{}, nextX ) ) ) {
                co_yield res;
            }
            P.erase( P.begin() );
            X.insert( v );
        }
    }

   public:
    void Solution1() {
        readFile();
        std::unordered_set<std::string> partyset;
        for ( auto& [netA, _] : LANNetwork ) {
            for ( auto& netB : LANNetwork.at( netA ) ) {
                for ( auto& netC : LANNetwork.at( netB ) ) {
                    if ( LANNetwork.at( netA ).contains( netC ) && ( netA.substr( 0, 1 ) + netB.substr( 0, 1 ) + netC.substr( 0, 1 ) ).find( 't' ) != std::string::npos ) {
                        std::set party( { netA, netB, netC } );
                        partyset.insert( std::ranges::fold_left( party, std::string(), std::plus<>{} ) );
                    }
                }
            }
        }
        printRes( 1, partyset.size() );
    }
    void Solution2() {
        std::set<std::string> P( std::from_range_t{}, LANNetwork | std::views::keys );
        std::set<std::pair<int, std::string>, std::greater<>> clique;
        for ( std::set<std::string> cliq : BronKerbosch1( {}, P, {} ) ) {
            clique.emplace( cliq.size(), std::ranges::fold_left( cliq, "", []( std::string const& add, std::string const& v ) { return add.empty() ? v : ( add + "," + v ); } ) );
        }
        printRes( 2, clique.begin()->second );
    }
};
