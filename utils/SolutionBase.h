#pragma once
#include <iostream>
#include <sstream>
#include <format>
class SolutionBase {
    static int id;

   protected:
    template <typename T>
    void printRes( int id, T const& res ) {
        using namespace std;
        cout << format( "\tSolution {}: {}\n", id, res );
    }

   public:
    SolutionBase() {
        using namespace std;
        cout << format( "Day {:d}:\n", ++id );
    }
};