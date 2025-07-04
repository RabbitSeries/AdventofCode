#include <cstdio>  // for popen(), pclose()
#include <fstream>
#include <iomanip>  // for setw
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Entry {
    std::string language;
    int files, blank, comment, code;
};

std::vector<std::string> splitlines( std::ifstream& ifs ) {
    std::stringstream ss;
    std::vector<std::string> res;
    ss << ifs.rdbuf();
    for ( std::string buf; getline( ss, buf ); ) {
        res.emplace_back( move( buf ) );
    }
    return res;
}

void replaceLines( std::vector<std::string>& input, const char* linestartWith, std::vector<std::string> const& replaceWith ) {
    auto iter = input.begin();
    for ( ; iter != input.end(); iter++ ) {
        if ( ( *iter ).starts_with( linestartWith ) ) {
            break;
        }
    }
    auto backwardlines = iter - input.begin();
    input.resize( backwardlines + replaceWith.size(), "" );
    iter = input.begin() + backwardlines;
    for ( auto& line : replaceWith ) {
        *iter++ = line;
    }
}

void replaceFile( const char* filename, const char* linestartWith, std::vector<std::string> const& replaceWith ) {
    std::ifstream ifs( filename );
    std::vector<std::string> input = splitlines( ifs );
    replaceLines( input, "Language", replaceWith );
    std::ofstream output( filename );
    for ( const std::string& data : input ) {
        output << data << std::endl;
    }
}

int main() {
#ifdef __linux__
    FILE* pipe = popen( R"(cloc . --include-ext=h,cc,cpp,hpp,c,java,py,js,ts,mjs --exclude-dir=.vscode,build --not-match-d="node_modules|dist|target" 2>exception.log)", "r" );
#else
    FILE* pipe = popen( R"(wsl -e cloc . --include-ext=h,cc,cpp,hpp,c,java,py,js,ts,mjs --exclude-dir=.vscode,build --not-match-d="node_modules|dist|target" 2>exception.log)", "r" );
#endif
    if ( !pipe ) {
        std::cerr << "Failed to open popen\n";
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
    if ( int status = pclose( pipe ); status == 0 ) {
        std::vector<std::string> res;
        res.emplace_back( "Language           | files | blank | comment | code" );
        res.emplace_back( "-------------------|-------|-------|---------|-----" );
        // Output as Markdown
        for ( const auto& e : entries ) {
            std::ostringstream ss;
            ss << std::left << std::setw( 19 ) << e.language << "| "
               << std::right << std::setw( 5 ) << e.files << " | "
               << std::setw( 5 ) << e.blank << " | "
               << std::setw( 7 ) << e.comment << " | "
               << std::setw( 4 ) << e.code << "";
            res.emplace_back( ss.str() );
        }
        replaceFile( "README.md", "Language", res );
    } else {
        std::cout << "Runing cloc exit " << status << ", " << std::endl
                  << "cloc may have not been installed." << std::endl
                  << "Please run this programm in root/Administrator mode." << std::endl
                  << "See ./exception.log for details." << std::endl;
    }
    return 0;
}
