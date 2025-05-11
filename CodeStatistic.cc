#include <cstdio>   // for popen(), pclose()
#include <iomanip>  // for setw
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Entry {
    std::string language;
    int files, blank, comment, code;
};

int main() {
    FILE* pipe = popen( "cloc . --include-ext=h,cc,cpp,hpp,c,java,py --exclude-dir=.vscode,.git,compileRuntime,JavaLibs,out,Legacy", "r" );
    if ( !pipe ) {
        std::cerr << "Failed to run cloc\n";
        return 1;
    }

    std::vector<Entry> entries;
    char buffer[4096];
    bool in_table = false;

    while ( fgets( buffer, sizeof( buffer ), pipe ) ) {
        std::string line( buffer );
        std::cout << line;
        if ( !in_table ) {
            if ( line.find( "Language" ) != std::string::npos && line.find( "code" ) != std::string::npos ) {  // Find start of table
                in_table = true;
            }
            continue;
        }
        // Parse table line
        if ( line.find_first_not_of( " -\n\r\t" ) == std::string::npos ) continue;  // skip empty lines and cloc line seperators

        Entry e;
        std::istringstream iss( line );
        std::getline( iss, e.language, ' ' );  // language may have spaces
        std::string temp;
        while ( iss >> temp ) {
            try {
                e.files = std::stoi( temp );
                break;
            } catch ( ... ) {
                e.language += " " + temp;  // it was part of the language name
            }
        }
        iss >> e.blank >> e.comment >> e.code;
        entries.push_back( e );
    }

    pclose( pipe );

    std::cout << "\n\n\nMarkdown:\n\n";
    std::cout << "Language           | files | blank | comment | code\n";
    std::cout << "-------------------|-------|-------|---------|-----\n";
    // Output as Markdown
    for ( const auto& e : entries ) {
        std::cout << std::left << std::setw( 19 ) << e.language << "| "
                  << std::right << std::setw( 5 ) << e.files << " | "
                  << std::setw( 5 ) << e.blank << " | "
                  << std::setw( 7 ) << e.comment << " | "
                  << std::setw( 4 ) << e.code << "\n";
    }
    return 0;
}
