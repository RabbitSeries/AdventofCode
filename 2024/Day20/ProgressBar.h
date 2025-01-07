#include <iostream>
#include <iomanip> // for std::fixed and std::setprecision
#include <thread>  // for std::this_thread::sleep_for
#include <chrono>  // for std::chrono::milliseconds
#include <cstdlib>
inline void showProgressBar( int processCnt, int pathCnt ) {
    static int processCntDelta = 0;
    if( processCnt > processCntDelta + 2000 || processCnt == pathCnt ) {
#if defined(_WIN32) || defined(_WIN64)
        // std::system( "PAUSE" );  // For Windows
        std::system( "cls" );  // For Windows
#else
        std::system( "clear" );  // For Linux or macOS
#endif
        processCntDelta = processCnt;
    }
    else {
        return;
    }
    const int barWidth = 50; 
    double progress = static_cast<double>( processCnt ) / pathCnt;


    std::cout << "\rProcessing " << processCnt << " / " << pathCnt << " [";
    int pos = static_cast<int>( barWidth * progress );
    for( int i = 0; i < barWidth; ++i ) {
        if( i < pos )
            std::cout << "="; 
        else if( i == pos )
            std::cout << ">";
        else
            std::cout << " "; 
    }
    std::cout << "] " << std::fixed << std::setprecision( 2 ) << ( progress * 100.0 ) << "%" << std::endl;
}
