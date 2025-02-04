#include <iostream>
#include <cstdlib>

void test3() {
    // Github issue at: <https://github.com/microsoft/vscode-cpptools/issues/13110>
    std::cout << "This line should be cleared." << std::endl;
    // std::cout.flush();
    std::system( "cls" );
    // std::cout << "\033[2J\033[1;1H";
    std::cout << "This line will be the one and only line left." << std::endl;
    // std::cout.flush();
    system( "PAUSE" );
    std::cout << "Count 1. Some random words. Some random words. Some random words. " << std::endl;
    system( "PAUSE" );
    std::cout << "Count 2. Some random words. Some random words. Some random words. " << std::endl;
    system( "PAUSE" );
    std::cout << "Count 3. Some random words. Some random words. Some random words. " << std::endl;
    system( "PAUSE" );
}