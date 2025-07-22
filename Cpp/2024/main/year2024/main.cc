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
    using solutionStat = tuple<int64_t, int, string_view>;
    int64_t t_cost = 0;

    priority_queue<solutionStat, vector<solutionStat>, less<>> pq;

    cout << "Registred solutions: " << SolutionRegistry::registry.size() << endl;
    for ( int i = 0; auto& [solutionName, instance] : SolutionRegistry::registry ) {
        cout << "Day " << ++i << ": " << solutionName << endl;

        auto nowTime = chrono::high_resolution_clock::now();
        ImplPtr solution = instance();
        solution->Solution1();
        solution->Solution2();
        auto endTime = chrono::high_resolution_clock::now();

        auto delta = chrono::duration_cast<chrono::microseconds>( endTime - nowTime ).count();
        pq.emplace( delta, i, solutionName );
        t_cost += delta;
    }
    cerr << "---\t\t\t\t\t\tFinished\t\t\t\t\t\t---" << endl;
    cerr << "---\t\t\t\t\t\tFinished\t\t\t\t\t\t---" << endl;
    cerr << "---\t\t\t\t\t\tFinished\t\t\t\t\t\t---" << endl;
    cerr << "Problem time t_cost ranking:" << endl
         << endl;
    for ( int i = 0; !pq.empty(); ) {
        auto [c_cost, solutionId, info] = pq.top();
        pq.pop();
        cerr << ++i << ". Day " << solutionId << " " << info << ":" << endl
             << fixed << setprecision( 6 ) << right << setw( 15 ) << setfill( ' ' ) << c_cost / 1000000.0
             << " seconds." << endl;
    }
    cerr << "Total cost: " << ( t_cost / 1000000.0 ) << " seconds." << endl;
    return 0;
}
