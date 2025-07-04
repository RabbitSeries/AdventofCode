#pragma once
#include <iostream>
#include <sstream>
class SolutionBase {
    static int id;

   protected:
    template <typename... Args>
    void printRes( int id, Args&&... args ) {
        using namespace std;
        ( ( cout << "\tSolution " << id << ": " ) << ... << forward<Args>( args ) ) << endl;  // fold expression
    }

   public:
    SolutionBase() {
        using namespace std;
        cout << "Day " << ++id << ":\n";
    }
};