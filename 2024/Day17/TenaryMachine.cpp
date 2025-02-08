#include "TenaryMachine.h"

using namespace data;

int TenaryMachine::pc = 0;
ull TenaryMachine::ra = 0, TenaryMachine::rb = 0, TenaryMachine::rc = 0;
string TenaryMachine::outBuf = "";

void ( *( TenaryMachine::fun[8] ) )( int operand ) = { &adv,bxl,bst,jnz,bxc ,out,bdv,cdv };