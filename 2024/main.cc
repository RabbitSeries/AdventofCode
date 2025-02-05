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
#include "Day17/3bitMachine.h"
#include "Day18/RAMRun.h"
#include "Day19/LinenLayout.h"
#include "Day20/RaceCondition.h"
#include "Day21/CascadingRemote.h"
#include "Day22/MonkeyMarket.h"
#include "Day23/LANParty.h"
#include "Day24/TinkerWithGates.h"
#include "Day25/KeyPair.h"

int SolutionStart = 14;
int SolutionTarget = 25;

auto nowTime = chrono::high_resolution_clock::now();
auto endTime = chrono::high_resolution_clock::now();

void printProcess() {
    endTime = chrono::high_resolution_clock::now();
    cout
        << "Day " << SolutionStart << " problem time cost:" << endl
        << fixed << setprecision( 6 ) << right << setw( 15 ) << setfill( ' ' )
        << chrono::duration_cast<chrono::microseconds>( endTime - nowTime ).count() / 1000000.0
        << " seconds." << endl;
    if( SolutionStart != SolutionTarget ) {
        // cout << "Cotinuing to next problem" << endl;
        cout << "Cotinuing to next problem in 1 seconds." << endl;
        this_thread::sleep_for( chrono::seconds( 1 ) );
    }
    SolutionStart++;
    nowTime = chrono::high_resolution_clock::now();
}

int main() {
    PairAndAdd Day01 = PairAndAdd();
    Day01.Solution1();
    Day01.Solution2();
    printProcess();
    CheckLevelInOrder Day02 = CheckLevelInOrder();
    Day02.Solution1();
    Day02.Solution2();
    printProcess();
    PreciseMul Day03 = PreciseMul();
    Day03.Solution1();
    Day03.Solution2();
    printProcess();
    CeresSearch Day04 = CeresSearch();
    Day04.Solution1();
    Day04.Solution2();
    printProcess();
    OrderingUpdates Day05 = OrderingUpdates();
    Day05.Solution1();
    Day05.Solution2();
    printProcess();
    PatrolRoute Day06 = PatrolRoute();
    Day06.Solution1();
    Day06.Solution2();
    printProcess();
    Calibration Day07 = Calibration();
    Day07.Solution1();
    Day07.Solution2();
    printProcess();
    AntinodeHarmonic Day08 = AntinodeHarmonic();
    Day08.Solution1();
    Day08.Solution2();
    printProcess();
    DiskCompact Day09 = DiskCompact();
    Day09.Solution1();
    Day09.Solution2();
    printProcess();
    TrailScore Day10 = TrailScore();
    Day10.Solution1();
    Day10.Solution2();
    printProcess();
    TwinkleStones Day11 = TwinkleStones();
    Day11.Solution1();
    Day11.Solution2();
    printProcess();
    AsciiGraph Day12 = AsciiGraph();
    Day12.Solution1();
    Day12.Solution2();
    printProcess();
    CoinOptimize Day13 = CoinOptimize();
    Day13.Solution1();
    Day13.Solution2();
    printProcess();
    RobotPatrol Day14 = RobotPatrol();
    Day14.Solution1();
    // Easter egg
    // Day14.Solution2();
    printProcess();
    PushBox Day15 = PushBox();
    Day15.Solution1();
    Day15.Solution2();
    printProcess();
    MazeDijkstra Day16 = MazeDijkstra();
    Day16.Solution();
    printProcess();
    TenaryMachine Day17 = TenaryMachine();
    Day17.Solution1();
    Day17.Solution2();
    printProcess();
    RAMRun Day18 = RAMRun();
    Day18.Solution1();
    Day18.Solution2();
    printProcess();
    LinenLayout Day19 = LinenLayout();
    Day19.Solution1();
    Day19.Solution2();
    printProcess();
    RaceCondition Day20 = RaceCondition();
    Day20.Solution1();
    Day20.Solution2();
    printProcess();
    CascadingRemote Day21 = CascadingRemote();
    Day21.Solution1();
    Day21.Solution2();
    printProcess();
    MonkeyMarket Day22 = MonkeyMarket();
    Day22.Solution1();
    Day22.Solution2();
    printProcess();
    LANParty Day23 = LANParty();
    Day23.Solution1();
    Day23.Solution2();
    printProcess();
    TinkerWithGates Day24 = TinkerWithGates();
    Day24.Solution1();
    Day24.Solution2();
    printProcess();
    KeyPair Day25 = KeyPair();
    Day25.Solution1();
    Day25.Solution2();
    printProcess();
    return 0;
}
