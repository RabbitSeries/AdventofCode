#include <iostream>

#include "stream.hpp"
using namespace std;
int main() {
    stream<string> t{ "Hello", "World" };
    cout << t.join( " "s );
}