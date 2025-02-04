#include "Day06/PatrolRoute.h"

int SolutionStart = 0;
int SolutionTarget = 0;

auto nowTime = chrono::high_resolution_clock::now();
auto endTime = chrono::high_resolution_clock::now();

void printProcess() {
    cout
        << "Day " << SolutionStart << " problem time cost:" << endl
        << fixed << setprecision( 6 ) << right << setw( 15 ) << setfill( ' ' )
        << chrono::duration_cast<chrono::microseconds>( endTime - nowTime ).count() / 1000000.0
        << " seconds." << endl;
    if( SolutionStart != SolutionTarget ) {
        cout << "Cotinuing to next problem in 3 seconds." << endl;
        this_thread::sleep_for( chrono::seconds( 3 ) );
    }
}

int main() {
    cout << "Cotinuing to next problem in 3 seconds." << endl;
    PatrolRoute Day06 = PatrolRoute();
    Day06.Solution1();
    Day06.Solution2();
    return 0;
}
