#include "3bitMachine.h"
/**
 * 1. Double Quotes ("header.h")
Search Order:
The compiler first looks for the file in the same directory as the source file (or in the directory specified by the include path for the project).
If not found, it then searches the standard include directories.
Use Case:
Typically used for user-defined header files or project-specific files.
2. Angle Brackets (<header.h>)
Search Order:
The compiler skips the local directory and directly searches the standard include directories.
Use Case:
Typically used for standard library headers or headers from external libraries.
 */

int main() {
    regex re( "[0-9]+" );
    vector<int> instruction;
    FILE* input( fopen( "input.txt", "r" ) );
    char buf[1024] = "\0";
    long long tra = 0, trb = 0, trc = 0;
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
    // tra = 7;
    InitProgram( tra, 0, 0 );
    while( pc < instruction.size() ) {
        int opcode = instruction[pc];
        int operand = instruction[pc + 1];
        operation( opcode, operand );
    }
    cout << outBuf.substr(1) << endl;
    return 0;
}
