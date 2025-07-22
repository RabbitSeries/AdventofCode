#include <string>
#include <vector>
#include <queue>

using namespace std;
// #include <utils/SolutionBase.hpp>
class TenaryMachine : public SolutionBase {
	REGISTER( TenaryMachine )

    typedef unsigned long long ull;
    size_t pc = 0;
    ull ra = 0, rb = 0, rc = 0;
    string outBuf = "";

    void ( TenaryMachine::* fun[8] )( int operand ) = { &TenaryMachine::adv, &TenaryMachine::bxl, &TenaryMachine::bst, &TenaryMachine::jnz, &TenaryMachine::bxc, &TenaryMachine::out, &TenaryMachine::bdv, &TenaryMachine::cdv };

    // It turns out that only opcode 0 will change the value of ra, and the example of this problem always use literal num to change its value. Within one round ra is only changed once, and was all the way devided to zero.
    // out = out + "," + to_string( ( ( ( ( modulo ) ^ 7 ) ^ ( tmp / ( unsigned long long )( powf128( 2, ( modulo ) ^ 7 ) ) ) ) ^ 7 ) % 8 );
    void adv( int operand ) {
#ifdef UNIX
        ull denominator = (ull)powf128( 2, combo( operand ) );
#else
        ull denominator = (ull)powl( 2, combo( operand ) );
#endif
        ra = ra / denominator;
        return;
    }

    void bxl( int operand ) {
        rb = ( ( rb ) ^ ull( operand ) );
        return;
    }

    void bst( int operand ) {
        rb = combo( operand ) % 8;
        return;
    }

    void jnz( int operand ) {
        if ( ra == 0 ) {
            pc += 2;
            return;
        } else {
            pc = operand;
        }
    }

    void bxc( int operand ) {
        rb = ( ( rb ) ^ ( rc ) );
    }

    void out( int operand ) {
        outBuf += "," + to_string( combo( operand ) % 8 );
    }

    void bdv( int operand ) {
        ull numerator = ra;
        ull denominator = (ull)pow( 2, combo( operand ) );
        rb = numerator / denominator;
    }

    void cdv( int operand ) {
        ull numerator = ra;
        ull denominator = (ull)pow( 2, combo( operand ) );
        rc = numerator / denominator;
    }

    ull combo( int operand ) {
        if ( operand <= 3 ) {
            return (ull)operand;
        }
        switch ( operand ) {
            case 4:
                return ra;
            case 5:
                return rb;
            case 6:
                return rc;
            default:
                cerr << "invild" << endl;
                break;
        }
        return 0;
    }

    void InitProgram( ull a, ull b, ull c ) {
        pc = 0;
        ra = a;
        rb = b;
        rc = c;
        outBuf.clear();
    }

    void operation( int opcode, int operand ) {
        ( this->*( fun[opcode] ) )( operand );
        if ( fun[opcode] != &TenaryMachine::jnz ) {
            pc += 2;
        }
    }
    vector<int> instruction;
    size_t tra = 0, trb = 0, trc = 0;
    string program = "";

    void readFile() {
        regex re( "[0-9]+" );
        ifstream input( "Day17/input.txt" );
        for ( string buf; getline( input, buf ); ) {
            sregex_iterator it( buf.begin(), buf.end(), re ), end_it;
            if ( distance( it, end_it ) == 1 ) {
                if ( buf.find( "A" ) != string::npos ) {
                    tra = stoi( ( *it ).str() );
                } else if ( buf.find( "B" ) != string::npos ) {
                    rb = trb = stoi( ( *it ).str() );
                } else {
                    rc = trc = stoi( ( *it ).str() );
                }
            } else {
                while ( it != end_it ) {
                    instruction.push_back( stoi( ( *it ).str() ) );
                    program += "," + ( *it ).str();
                    it++;
                }
            }
        }
    }

   public:
    void Solution1() {
        readFile();
        InitProgram( tra, 0, 0 );
        while ( pc < instruction.size() ) {
            int opcode = instruction[pc];
            int operand = instruction[pc + 1];
            operation( opcode, operand );
        }
        printRes( 1, outBuf.substr( 1 ) );
    }

    void Solution2() {
        InitProgram( tra, 0, 0 );
        ull p = 1;
        // Yeah, I nailed it.
        priority_queue<ull, vector<ull>, greater<>> processQue;
        processQue.push( p );
        // int filterdTimes = 0;
        while ( !processQue.empty() ) {
            ull searchBase = processQue.top();
            processQue.pop();
            int i = 0;
            bool flag = false;
            // This boundary is determined by the constant literal value after opcode 0: 0,3, 1<<3 = 8.
            for ( ; i < 8; i++ ) {
                ull curRA = searchBase + i;
                InitProgram( curRA, 0, 0 );
                // while( pc < instruction.size() ) {
                //     int opcode = instruction[pc];
                //     int operand = instruction[pc + 1];
                //     operation( opcode, operand );
                // }
                ull copy = curRA;
                do {
                    outBuf = outBuf + "," + to_string( ( ( ( ( copy % 8 ) ^ 7 ) ^ ( copy / (ull)( pow( 2, ( copy % 8 ) ^ 7 ) ) ) ) ^ 7 ) % 8 );
                    copy /= 8;
                } while ( copy != 0 );

                if ( outBuf.size() <= program.size() && outBuf == program.substr( program.size() - outBuf.size(), outBuf.size() ) ) {
                    processQue.push( curRA << 3 );
                    flag = true;
                    // cout << outBuf << endl;
                    if ( outBuf == program ) {
                        printRes( 2, curRA );
                        // cout << "Found register A: " << curRA << endl;
                        // cout << (ull)( 0xFFFFFFFFFFFFFFFFull ) << endl;
                        // Break here instead.
                        return;
                    }
                    // Don't break me!
                    // break;
                    // Filtered, times 71, value 2140870108847336
                    // There are multiple choice in 0-7 that can reach the current result, but some of them is invalid.
                }
            }
            if ( i == 8 && !flag ) {
                // cout << "Filtered, times " << ++filterdTimes << ", value " << searchBase << endl;
            }
        }
    }
};