#include <cmath>
#include <fstream>
#include <functional>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "utils/ISolution.hpp"
class TenaryMachine : public ISolution {
    REGISTER( TenaryMachine )

    using ull = unsigned long long;
    std::size_t pc = 0;
    ull ra = 0, rb = 0, rc = 0;
    std::string outBuf = "";
    std::vector<ull> instruction;
    std::size_t init_ra = 0, init_rb = 0, init_rc = 0;
    std::string program = "";
    void InitRa( ull a ) {
        pc = 0;
        ra = a;
        rb = 0;
        rc = 0;
        outBuf.clear();
    }
    void ( TenaryMachine::* fun[8] )( ull operand ) = { &TenaryMachine::adv, &TenaryMachine::bxl, &TenaryMachine::bst, &TenaryMachine::jnz,
                                                        &TenaryMachine::bxc, &TenaryMachine::out, &TenaryMachine::bdv, &TenaryMachine::cdv };
    // It turns out that only opcode 0 will change the value of ra, and the example of this problem always use literal num to change its value. Within one round ra is only changed once, and was all the way devided to zero.
    // out = out + "," + to_string( ( ( ( ( modulo ) ^ 7 ) ^ ( tmp / ( unsigned long long )( powf128( 2, ( modulo ) ^ 7 ) ) ) ) ^ 7 ) % 8 );
    void adv( ull operand ) {
        ra = ra >> combo( operand );
    }
    void bxl( ull operand ) {
        rb = rb ^ operand;
    }
    void bst( ull operand ) {
        rb = combo( operand ) % 8;
    }
    void jnz( ull operand ) {
        pc = operand;
    }
    void bxc( ull operand ) {
        rb = rb ^ rc;
    }
    void out( ull operand ) {
        outBuf += "," + std::to_string( combo( operand ) % 8 );
    }
    void bdv( ull operand ) {
        rb = ra >> combo( operand );
    }
    void cdv( ull operand ) {
        rc = ra >> combo( operand );
    }
    ull combo( ull operand ) {
        if ( operand <= 3 ) {
            return operand;
        }
        switch ( operand ) {
            case 4:
                return ra;
            case 5:
                return rb;
        }
        return rc;  // case 6:
    }
    void operation( int opcode, int operand ) {
        if ( fun[opcode] != &TenaryMachine::jnz || ra != 0 ) {
            ( this->*( fun[opcode] ) )( operand );
        }
        if ( fun[opcode] != &TenaryMachine::jnz || ra == 0 ) {
            pc += 2;
        }
    }
    void readFile() {
        std::regex re( "[0-9]+" );
        std::ifstream input( "Day17/input.txt" );
        for ( std::string buf; std::getline( input, buf ); ) {
            std::sregex_iterator it( buf.begin(), buf.end(), re ), end_it;
            if ( distance( it, end_it ) == 1 ) {
                if ( buf.contains( "A" ) ) {
                    init_ra = std::stoul( it->str() );
                } else if ( buf.contains( "B" ) ) {
                    init_rb = std::stoul( it->str() );
                } else {
                    init_rc = std::stoul( it->str() );
                }
            } else {
                while ( it != end_it ) {
                    instruction.push_back( std::stoul( it->str() ) );
                    program += "," + it->str();
                    it++;
                }
            }
        }
    }

   public:
    void Solution1() {
        readFile();
        InitRa( init_ra );
        while ( pc < instruction.size() ) {
            operation( instruction[pc], instruction[pc + 1] );
        }
        printRes( 1, outBuf.substr( 1 ) );
    }

    void Solution2() {
        std::priority_queue<ull, std::vector<ull>, std::greater<>> pq;
        pq.push( 0 );
        std::set<ull> visited( { 0 } );
        while ( !pq.empty() ) {
            ull searchBase = pq.top();
            pq.pop();
            // use 000 ~ 111 to fill last 3 bit
            for ( int i : std::views::iota( searchBase == 0 ? 1 : 0, 8 ) ) {
                ull curRA = searchBase + i;
                InitRa( curRA );
                while ( pc < instruction.size() ) {
                    operation( instruction[pc], instruction[pc + 1] );
                }
                if ( outBuf.size() <= program.size() && program.ends_with( outBuf ) ) {
                    if ( outBuf.size() == program.size() ) {
                        printRes( 2, curRA );
                        return;
                    }
                    pq.push( curRA << 3 );
                }
            }
        }
    }
};