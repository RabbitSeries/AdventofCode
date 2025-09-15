#include <algorithm>
#include <fstream>
#include <map>
#include <ranges>
#include <vector>

#include "utils/ISolution.hpp"
class PushBox : public ISolution {
    REGISTER( PushBox )

    // Simple data member is able to initialize
    const int CELLWALL = -1;
    const int CELLEMPTY = -2;

    struct Point2D {
        Point2D() {}
        Point2D( int a, int b, int _id ) : x( a ), y( b ), id{ _id } {}
        int x{}, y{}, id{ -1 };
    };

    int dx[4]{ -1, 0, 1, 0 };
    int dy[4]{ 0, 1, 0, -1 };
    std::map<char, int> dirs{ { '^', 0 },
                              { '>', 1 },
                              { 'v', 2 },
                              { '<', 3 } };

    using Grid = std::vector<std::vector<int>>;

    Point2D next( char c, Point2D const& curPos, Grid const& grid ) {
        int dir = dirs.at( c );
        int nx = curPos.x + dx[dir], ny = curPos.y + dy[dir];
        return { nx, ny, grid[nx][ny] };
    }

    bool isWALL( Point2D const& point ) {
        return point.id == CELLWALL;
    }
    bool isEmpty( Point2D const& point ) {
        return point.id == CELLEMPTY;
    }
    bool isBox( Point2D const& point ) {
        return point.id >= 0;
    }

    bool pushBox( Point2D curPos, char c ) {
        Point2D nextPos = next( c, curPos, BoxMap );
        if ( isEmpty( nextPos ) || ( isBox( nextPos ) && pushBox( nextPos, c ) ) ) {
            BoxMap[nextPos.x][nextPos.y] = curPos.id;
            BoxMap[curPos.x][curPos.y] = CELLEMPTY;
            BoxList[curPos.id] = { nextPos.x, nextPos.y, curPos.id };
            return true;
        }
        return false;
    }

    int sumCoordinates() {
        return std::ranges::fold_left( BoxList | std::views::transform( []( Point2D const& b ) {
                                           return 100 * b.x + b.y;
                                       } ),
                                       0, std::plus<>{} );
    }
    int sumWideBoxes() {
        return std::ranges::fold_left( WideBoxList | std::views::transform( []( WideBox const& b ) {
                                           return 100 * b.l.x + b.l.y;
                                       } ),
                                       0, std::plus<>{} );
    }
    std::string control;
    std::vector<Point2D> BoxList;
    Grid BoxMap;
    Point2D start;

    void buildData() {
        std::ifstream input( "Day15/input.txt" );
        for ( std::string buf; std::getline( input, buf ) && !buf.empty(); ) {
            std::vector<int> row;
            for ( char c : buf ) {
                if ( c == '#' ) {
                    row.push_back( CELLWALL );
                } else if ( c == '.' ) {
                    row.push_back( CELLEMPTY );
                } else if ( c == 'O' ) {
                    row.push_back( BoxList.size() );
                    BoxList.emplace_back( BoxMap.size(), row.size() - 1, BoxList.size() );
                } else if ( c == '@' ) {
                    row.push_back( CELLEMPTY );
                    start = Point2D( BoxMap.size(), row.size() - 1, -1 );
                }
            }
            BoxMap.emplace_back( std::move( row ) );
        }
        for ( std::string buf; std::getline( input, buf ); ) {
            control += buf;
        }
        for ( auto& line : BoxMap ) {
            WideBoxMap.push_back( {} );
            for ( int cell : line ) {
                WideBoxMap.rbegin()->insert( WideBoxMap.rbegin()->end(), { cell, cell } );
            }
        }
        for ( size_t i = 0; i < BoxList.size(); i++ ) {
            Point2D& box = BoxList[i];
            WideBoxList.emplace_back( box.x, box.y * 2, box.x, box.y * 2 + 1, i );
        }
    }

    struct WideBox {
        WideBox( int lx, int ly, int rx, int ry, int _id ) : l{ lx, ly, _id }, r{ rx, ry, _id }, id{ _id } {}
        WideBox( Point2D a, Point2D b, int _id ) : l( std::move( a ) ), r( std::move( b ) ), id{ _id } {}
        Point2D l, r;
        int id;
    };

    std::vector<WideBox> WideBoxList;
    Grid WideBoxMap;

    WideBox getWideBox( Point2D const& curPos ) {
        return WideBoxList[WideBoxMap[curPos.x][curPos.y]];
    }

    bool pushWideBox( std::vector<WideBox> collisions, char c ) {
        std::vector<WideBox> nextLevel;
        for ( WideBox const& b : collisions ) {
            Point2D lNext = next( c, b.l, WideBoxMap ), rNext = next( c, b.r, WideBoxMap );
            if ( isWALL( lNext ) || isWALL( rNext ) ) {
                return false;
            }
            if ( ( c == '<' || c != '>' ) && isBox( lNext ) ) {
                nextLevel.emplace_back( getWideBox( lNext ) );
            }
            if ( ( c == '>' || c != '<' ) && isBox( rNext ) && rNext.id != lNext.id ) {
                nextLevel.emplace_back( getWideBox( rNext ) );
            }
        }
        if ( nextLevel.empty() || pushWideBox( std::move( nextLevel ), c ) ) {
            for ( auto& b : collisions ) {
                Point2D lNext = next( c, b.l, WideBoxMap ), rNext = next( c, b.r, WideBoxMap );
                if ( c == '<' || c != '>' ) {
                    WideBoxMap[lNext.x][lNext.y] = b.id;
                    WideBoxMap[b.r.x][b.r.y] = CELLEMPTY;
                }
                if ( c == '>' || c != '<' ) {
                    WideBoxMap[rNext.x][rNext.y] = b.id;
                    WideBoxMap[b.l.x][b.l.y] = CELLEMPTY;
                }
                WideBoxList[b.id] = WideBox( lNext, rNext, b.id );
            }
            return true;
        }
        return false;
    }

   public:
    void Solution1() {
        buildData();
        auto curPos = start;
        // PrintGrid( BoxMap, curPos );
        for ( char c : control ) {
            Point2D nextPos = next( c, curPos, BoxMap );
            if ( isEmpty( nextPos ) || ( isBox( nextPos ) && pushBox( nextPos, c ) ) ) {
                curPos = nextPos;
                // PrintGrid( BoxMap, curPos );
            }
        }
        printRes( 1, sumCoordinates() );
    }
    void Solution2() {
        Point2D curPos( start.x, start.y * 2, -1 );
        // PrintGrid( WideBoxMap, curPos );
        for ( char c : control ) {
            Point2D nextPos = next( c, curPos, WideBoxMap );
            if ( isEmpty( nextPos ) ) {
                curPos = nextPos;
                // PrintGrid( WideBoxMap, curPos );
            } else if ( isBox( nextPos ) ) {
                WideBox nextBox = WideBoxList[nextPos.id];
                if ( pushWideBox( { nextBox }, c ) ) {
                    curPos = nextPos;
                    // PrintGrid( WideBoxMap, curPos );
                }
            }
        }
        printRes( 2, sumWideBoxes() );
    }

   private:
    // void PrintGrid( Grid const& grid, Point2D const& bot ) {
    //     std::ofstream output( "Day15/output.txt" );
    //     for ( size_t i = 0; i < grid.size(); i++ ) {
    //         for ( size_t j = 0; j < grid[i].size(); j++ ) {
    //             if ( i == bot.x && j == bot.y ) {
    //                 output << "@";
    //             } else {
    //                 output << ( grid[i][j] >= 0 ? 'O' : grid[i][j] == CELLEMPTY ? ' '
    //                                                                             : '#' );
    //             }
    //         }
    //         output << std::endl;
    //     }
    //     return;
    // }
    // std::generator<char> readControlls() {
    //     std::cout << "Commands:\n";
    //     for ( std::string buf; getline( std::cin, buf ); ) {
    //         if ( buf.empty() ) {
    //             continue;
    //         }
    //         co_yield buf[0];
    //     }
    // }
    // std::generator<char> readControlls() {
    //     std::cout << "Commands:\n";
    //     int i = 0;
    //     for ( char c : control ) {
    //         std::cout << "Cmd: " << ( ++i ) << "/" << control.size();
    //         std::cout << c << std::endl;
    //         co_yield c;
    //     }
    // }
};
