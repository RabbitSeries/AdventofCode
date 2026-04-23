#include <algorithm>
#include <fstream>
#include <map>
#include <ranges>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "utils/ISolution.hpp"
#include "utils/Streams.hpp"

class PreciseMul : public ISolution {
    REGISTER( PreciseMul )

    std::string memory;

    int parseMul( const std::string& range ) {
        return std::ranges::fold_left( regexStream( R"(mul\((\d+),(\d+)\))", range ) |
                                           std::views::transform( []( std::smatch&& p ) { return stoi( p[1] ) * stoi( p[2] ); } ),
                                       0, std::plus<>{} );
    }

    void readFile() {
        using namespace std;
        ifstream input( "Day03/input.txt", std::ios_base::ate | std::ios_base::binary );
        memory.resize( input.tellg() );
        input.seekg( 0 );
        input.read( memory.data(), memory.size() );
    }

    utils::LazyGenerator<int> EnabledSolve() {
        bool enabled = true;
        // MSVC stack is too small, it will somehow cause stack overflow for (?:.|\s)*?
        for ( std::smatch&& section : regexStream( R"([\s\S]*?(don't\(\)|do\(\)|$))", memory ) ) {
            if ( enabled ) {
                co_yield parseMul( section.str() );
            }
            enabled = section.str().ends_with( "do()" );
        }
    }

   public:
    void Solution1() {
        readFile();
        printRes( 1, parseMul( memory ) );
    }

    void Solution2() {
        printRes( 2, std::ranges::fold_left( EnabledSolve(), 0, std::plus<>{} ) );
    }
};
