#include <bits/stdc++.h>
using namespace std;

namespace data {
    typedef unsigned long long ull;
};

class TenaryMachine {
    typedef unsigned long long ull;
    static int pc;
    static ull ra, rb, rc;

    // It turns out that only opcode 0 will change the value of ra, and the example of this problem always use literal num to change its value. Within one round ra is only changed once, and was all the way devided to zero.
    // out = out + "," + to_string( ( ( ( ( modulo ) ^ 7 ) ^ ( tmp / ( unsigned long long )( powf128( 2, ( modulo ) ^ 7 ) ) ) ) ^ 7 ) % 8 );
    static void adv( int operand ) {
#ifdef UNIX
        ull denominator = (ull)powf128( 2, combo( operand ) );
#else
        ull denominator = (ull)powl( 2, combo( operand ) );
#endif
        ra = ra / denominator;
        return;
    }

    static void bxl( int operand ) {
        rb = ( ( rb ) ^ ull( operand ) );
        return;
    }

    static void bst( int operand ) {
        rb = combo( operand ) % 8;
        return;
    }

    static void jnz( int operand ) {
        if( ra == 0 ) {
            pc += 2;
            return;
        } else {
            pc = operand;
        }
    }

    static void bxc( int operand ) {
        rb = ( ( rb ) ^ ( rc ) );
    }

    static void out( int operand ) {
        outBuf += "," + to_string( combo( operand ) % 8 );
    }

    static void bdv( int operand ) {
        ull numerator = ra;
        ull denominator = (ull)pow( 2, combo( operand ) );
        rb = numerator / denominator;
    }

    static void cdv( int operand ) {
        ull numerator = ra;
        ull denominator = (ull)pow( 2, combo( operand ) );
        rc = numerator / denominator;
    }

    static string outBuf;

    static ull combo( int operand ) {
        if( operand <= 3 ) {
            return (ull)operand;
        }
        switch( operand ) {
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

    static void ( *( fun[8] ) )( int operand );

    static void operation( int opcode, int operand ) {
        fun[opcode]( operand );
        if( fun[opcode] != jnz ) {
            pc += 2;
        }
    }
public:
    void Solution1() {
        regex re( "[0-9]+" );
        vector<int> instruction;
        FILE* input( fopen( "Day17/input.txt", "r" ) );
        char buf[1024] = "\0";
        long long tra = 0, trb = 0, trc = 0;
        string program = "";
        while( !feof( input ) && fgets( buf, 1024, input ) ) {
            string pattern( buf );
            sregex_iterator it( pattern.begin(), pattern.end(), re ), end_it;
            if( distance( it, end_it ) == 1 ) {
                if( pattern.find( "A" ) != string::npos ) {
                    tra = stoi( ( *it ).str() );
                } else if( pattern.find( "B" ) != string::npos ) {
                    rb = trb = stoi( ( *it ).str() );
                } else {
                    rc = trc = stoi( ( *it ).str() );
                }
            } else {
                while( it != end_it ) {
                    instruction.push_back( stoi( ( *it ).str() ) );
                    program += "," + ( *it ).str();
                    it++;
                }
            }
        }
        // tra = 7;
        InitProgram( tra, 0, 0 );
        while( pc < instruction.size() ) {
            int opcode = instruction[pc];
            int operand = instruction[pc + 1];
            operation( opcode, operand );
        }
        cout << "Solution 1: " << outBuf.substr( 1 ) << endl;
    }

    void Solution2() {
        regex re( "[0-9]+" );
        vector<int> instruction;
        // FILE* input( fopen( "inputExample.txt", "r" ) );
        FILE* input( fopen( "Day17/input.txt", "r" ) );
        char buf[1024] = "\0";
        ull tra = 0, trb = 0, trc = 0;
        string program = "";
        while( !feof( input ) && fgets( buf, 1024, input ) ) {
            string pattern( buf );
            sregex_iterator it( pattern.begin(), pattern.end(), re ), end_it;
            if( distance( it, end_it ) == 1 ) {
                if( pattern.find( "A" ) != string::npos ) {
                    tra = stoi( ( *it ).str() );
                } else if( pattern.find( "B" ) != string::npos ) {
                    rb = trb = stoi( ( *it ).str() );
                } else {
                    rc = trc = stoi( ( *it ).str() );
                }
            } else {
                while( it != end_it ) {
                    instruction.push_back( stoi( ( *it ).str() ) );
                    program += "," + ( *it ).str();
                    it++;
                }
            }
        }
        ull  p = 1;
        // Yeah, I nailed it.
        priority_queue<ull, vector<ull>, greater<>> processQue;
        processQue.push( p );
        int filterdTimes = 0;
        while( !processQue.empty() ) {
            ull searchBase = processQue.top();
            processQue.pop();
            int i = 0;
            bool flag = false;
            // This boundary is determined by the constant literal value after opcode 0: 0,3, 1<<3 = 8.
            for( ; i < 8; i++ ) {
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
                } while( copy != 0 );

                if( outBuf.size() <= program.size() && outBuf == program.substr( program.size() - outBuf.size(), outBuf.size() ) ) {
                    processQue.push( curRA << 3 );
                    flag = true;
                    // cout << outBuf << endl;
                    if( outBuf == program ) {
                        cout << "Found register A: " << curRA << endl;
                        cout << (ull)( 0xFFFFFFFFFFFFFFFFull ) << endl;
                        // Break here instead.
                    }
                    // Don't break me!
                    // break;
                    // Filtered, times 71, value 2140870108847336
                    // There are multiple choice in 0-7 that can reach the current result, but some of them is invalid.
                }
            }
            if( i == 8 && !flag ) {
                cout << "Filtered, times " << ++filterdTimes << ", value " << searchBase << endl;
            }
        }
    }
};