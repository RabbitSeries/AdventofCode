#include "Day01/PairAndAdd.h"
#include "Day02/CheckLevelInOrder.h"
#include "Day03/PreciseMul.h"
#include "Day04/CeresSearch.h"
#include "Day05/OrderingUpdates.h"
#include "Day06/PatrolRoute.h"
#include "Day07/Calibration.h"
#include "Day08/AntinodeHarmonic.h"
#include "Day09/DiskCompact.h"
#include "Day10/TrailScore.h"
#include "Day11/TwinkleStones.h"
#include "Day12/AsciiGraph.h"
#include "Day13/CoinOptimize.h"
#include "Day14/RobotPatrol.h"
#include "Day15/PushBox.h"
#include "Day16/MazeDijkstra.h"
#include "Day17/TenaryMachine.h"
#include "Day18/RAMRun.h"
#include "Day19/LinenLayout.h"
#include "Day20/RaceCondition.h"
#include "Day21/CascadingRemote.h"
#include "Day22/MonkeyMarket.h"
#include "Day23/LANParty.h"
#include "Day24/TinkerWithGates.h"
#include "Day25/KeyPair.h"

int main() {
    using solutionStat = std::tuple<int64_t, int, std::string>;
    int64_t t_cost = 0;

    std::priority_queue<solutionStat, std::vector<solutionStat>, std::less<>> pq;

    std::cout << "Registred solutions: " << SolutionRegistry::registry.size() << std::endl;
    for ( int i = 0; auto& [solutionName, instance] : SolutionRegistry::registry ) {
        std::cout << "Day " << ++i << ": " << solutionName << std::endl;

        auto startTime = std::chrono::high_resolution_clock::now();
        ImplPtr solution;
        std::string info = std::string( solutionName );
        try {
            // create and run solution
            solution = instance();
            solution->Solution1();
            solution->Solution2();
        } catch ( const std::exception& ex ) {
            std::cerr << "Exception while running Day " << i << " (" << solutionName << "): "
                      << ex.what() << std::endl;
            info += " [EX]";
        } catch ( ... ) {
            std::cerr << "Unknown exception while running Day " << i << " (" << solutionName << ")." << std::endl;
            info += " [EX_UNKNOWN]";
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::microseconds>( endTime - startTime ).count();
        pq.emplace( delta, i, info );
        t_cost += delta;

        try {
            solution.reset();
        } catch ( const std::exception& ex ) {
            std::cerr << "Exception thrown from destructor of Day " << i << " (" << solutionName << "): "
                      << ex.what() << std::endl;
        } catch ( ... ) {
            std::cerr << "Unknown exception thrown from destructor of Day " << i << " (" << solutionName << ")." << std::endl;
        }
    }

    std::cerr << "---\t\t\t\t\t\tFinished\t\t\t\t\t\t---" << std::endl;
    std::cerr << "---\t\t\t\t\t\tFinished\t\t\t\t\t\t---" << std::endl;
    std::cerr << "---\t\t\t\t\t\tFinished\t\t\t\t\t\t---" << std::endl;
    std::cerr << "Problem time t_cost ranking:" << std::endl
              << std::endl;

    for ( int i = 0; !pq.empty(); ) {
        auto [c_cost, solutionId, info] = pq.top();
        pq.pop();
        std::cerr << ++i << ". Day " << solutionId << " " << info << ":" << std::endl
                  << std::fixed << std::setprecision( 6 ) << std::right << std::setw( 15 ) << std::setfill( ' ' )
                  << c_cost / 1000000.0 << " seconds." << std::endl;
    }
    std::cerr << "Total cost: " << ( t_cost / 1000000.0 ) << " seconds." << std::endl;
    return 0;
}
