#include "RegexIter.hpp"
int main() {
    for ( int id = 0; auto const& match : RegexIter( "abcd", R"(\w)" ) ) {
        std::cout << "Match " << id++ << ": " << match << std::endl;
    }
}