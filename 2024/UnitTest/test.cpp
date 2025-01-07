#include <iostream>
#include <cstdlib>
int main(){
    // Github issue at: <https://github.com/microsoft/vscode-cpptools/issues/13110>
    std::cout << "This line should be cleared." << std::endl;    
    // std::cout.flush();
    std::system( "cls" );
    std::cout << "This line will be the one and only line left." << std::endl;
    // std::cout.flush();
    
    return 0;
}