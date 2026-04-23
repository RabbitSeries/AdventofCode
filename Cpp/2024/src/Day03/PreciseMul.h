#include <algorithm>
#include <fstream>
#include <map>
#include <ranges>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "utils/ISolution.hpp"
#include "utils/Stream/RegexStream.hpp"

class PreciseMul : public ISolution {
    REGISTER( PreciseMul )

    std::string memory;

    int parseMul( const std::string& range ) {
        return std::ranges::fold_left(
            RegexStream( R"(mul\((\d+),(\d+)\))", range ).yield() | std::views::transform( []( const std::smatch& p ) { return stoi( p[1] ) * stoi( p[2] ); } ),
            0, std::plus<>{} );
    }

    void readFile() {
        using namespace std;
        ifstream input( "Day03/input.txt" );
        memory = ( stringstream() << input.rdbuf() ).str();
    }

    utils::Generator<int> EnabledSolve() {
        bool enabled = true;
        for ( std::smatch const& section : RegexStream( R"(((?:.|\s)*?)(don't\(\)|do\(\)|$))", memory ).yield() ) {
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
