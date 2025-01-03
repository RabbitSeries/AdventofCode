#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;

static int pc = 0;
static ull ra = 0, rb = 0, rc = 0;
string outBuf = "";

ull combo( int operand ) {
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

void adv( int operand ) {
    ull denominator = (ull)powf128( 2, combo( operand ) );
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
    if( ra == 0 ) {
        pc += 2;
        return;
    }
    else {
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

void ( *( fun[8] ) )( int operand ) = { &adv,bxl,bst,jnz,bxc ,out,bdv,cdv };

void operation( int opcode, int operand ) {
    fun[opcode]( operand );
    if( fun[opcode] != jnz ) {
        pc += 2;
    }
}