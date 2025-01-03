#include "3bitMachine.h"

int main() {
    regex re( "[0-9]+" );
    vector<int> instruction;
    FILE* input( fopen( "input.txt", "r" ) );
    char buf[1024] = "\0";
    ull tra = 0, trb = 0, trc = 0;
    string program = "";
    while( !feof( input ) && fgets( buf, 1024, input ) ) {
        string pattern( buf );
        sregex_iterator it( pattern.begin(), pattern.end(), re ), end_it;
        if( distance( it, end_it ) == 1 ) {
            if( pattern.find( "A" ) != string::npos ) {
                tra = stoi( ( *it ).str() );
            }
            else if( pattern.find( "B" ) != string::npos ) {
                rb = trb = stoi( ( *it ).str() );
            }
            else {
                rc = trc = stoi( ( *it ).str() );
            }
        }
        else {
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
                outBuf = outBuf + "," + to_string( ( ( ( ( copy % 8 ) ^ 7 ) ^ ( copy / ( unsigned long long )( powf128( 2, ( copy % 8 ) ^ 7 ) ) ) ) ^ 7 ) % 8 );
                copy /= 8;
            } while( copy != 0 );

            if( outBuf.size() <= program.size() && outBuf == program.substr( program.size() - outBuf.size(), outBuf.size() ) ) {
                processQue.push( curRA << 3 );
                flag = true;
                cout << outBuf << endl;
                if( outBuf == program ) {
                    cout << "Found register A :" << curRA << endl;
                    // return 0;
                }
                // Don't break me!
                // break;
                // There are multiple choice in 0-7 that can reach the current result, but some of them is invalid.
            }
        }
        if( i == 8 && !flag ) {
            cout << "Filtered times " << ++filterdTimes << ": " << searchBase << endl;
        }
    }
    return 0;
}


// Nice try though.
// long long getNextRa( long long permutate, bitset<3> key ) {
//     string permutateLower4 = bitset<4>( permutate & 0xF ).to_string();
//     string permutateBits = bitset<122>( permutate ).to_string();
//     string permutateHigher = permutateBits.substr( 0, permutateBits.size() - 4 );
//     bitset<3> alignPattern = key;
//     alignPattern[0] = alignPattern[0] ^ true;
//     alignPattern[2] = alignPattern[2] ^ true;
//     string keystr = key.to_string();
//     string suffix = alignPattern.to_string();
//     long long returnVal = stoll( permutateHigher + keystr + permutateLower4 + suffix );
//     return returnVal;
// }