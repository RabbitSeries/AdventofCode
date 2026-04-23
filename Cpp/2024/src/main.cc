#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <queue>
#include <tuple>
#include <vector>

#include "utils/ISolution.hpp"
int main() {
    using solutionStat = std::tuple<int64_t, int, std::string>;
    int64_t t_cost = 0;

    std::priority_queue<solutionStat, std::vector<solutionStat>, std::less<>> runtime;
    auto& registry = SolutionRegistry::registry;
    std::cout << "Registred solutions: " << registry.size() << std::endl;
    for ( int i = 0; !registry.empty(); registry.pop(), i++ ) {
        const auto& entry = registry.top();
        const auto solutionDay = entry.day ? entry.day : ( i + 1 );
        const auto& solutionName = entry.name;
        const auto& factory = entry.factory;
        std::cout << "Day " << solutionDay << ": " << solutionName << std::endl;

        auto startTime = std::chrono::high_resolution_clock::now();
        SolutionRegistry::ImplPtr solution;
        std::string info = std::string( solutionName );
        try {
            // create and run solution
            solution = factory();
            solution->Solution1();
            solution->Solution2();
        } catch ( const std::exception& ex ) {
            std::cerr << "Exception while running Day " << solutionDay << " (" << solutionName << "): "
                      << ex.what() << std::endl;
            info += " [EX]";
        } catch ( ... ) {
            std::cerr << "Unknown exception while running Day " << solutionDay << " (" << solutionName << ")." << std::endl;
            info += " [EX_UNKNOWN]";
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::microseconds>( endTime - startTime ).count();
        runtime.emplace( delta, solutionDay, info );
        t_cost += delta;

        try {
            solution.reset();
        } catch ( const std::exception& ex ) {
            std::cerr << "Exception thrown from destructor of Day " << solutionDay << " (" << solutionName << "): "
                      << ex.what() << std::endl;
        } catch ( ... ) {
            std::cerr << "Unknown exception thrown from destructor of Day " << solutionDay << " (" << solutionName << ")." << std::endl;
        }
    }

    std::cerr << "---\t\t\t\t\t\tFinished\t\t\t\t\t\t---" << std::endl;
    std::cerr << "---\t\t\t\t\t\tFinished\t\t\t\t\t\t---" << std::endl;
    std::cerr << "---\t\t\t\t\t\tFinished\t\t\t\t\t\t---" << std::endl;
    std::cerr << "Problem time t_cost ranking:" << std::endl
              << std::endl;

    for ( int i = 1; !runtime.empty(); i++, runtime.pop() ) {
        const auto& [c_cost, solutionId, info] = runtime.top();
        std::cerr << i << ". Day " << solutionId << " " << info << ":" << std::endl
                  << std::fixed << std::setprecision( 6 ) << std::right << std::setw( 15 ) << std::setfill( ' ' )
                  << c_cost / 1000000.0 << " seconds." << std::endl;
    }
    std::cerr << "Total cost: " << ( t_cost / 1000000.0 ) << " seconds." << std::endl;
    return 0;
}
