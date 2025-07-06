#include <cstdio>  // for popen(), pclose()
#include <fstream>
#include <iomanip>  // for setw
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

// constexpr int lang_w = 19, digit_w = 15;
constexpr int lang_w = 19, digit_w = 14;

struct Entry {
    std::string lang;
    int files, blank, comment, code;
    template <typename T, typename... rest>
    static std::string constructRest( T&& first, rest&&... args ) {
        std::ostringstream oss;
        oss << "|" << std::right << std::setw( digit_w ) << std::forward<T>( first ) << constructRest( std::forward<rest>( args )... );
        return oss.str();
    }
    static std::string constructRest() {
        return "";
    }
    template <typename T, typename... rest>
    static std::string constructLine( T&& lang, rest&&... args ) {
        std::ostringstream oss;
        oss << std::left << std::setw( lang_w ) << lang << constructRest( std::forward<rest>( args )... );
        return oss.str();
    }
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
        if ( iter->starts_with( linestartWith ) ) {
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
    std::ifstream ifs( filename, std::ios::in );
    std::vector<std::string> input = splitlines( ifs );
    replaceLines( input, "Language", replaceWith );
    std::ofstream output( filename, std::ios::out | std::ios::trunc );
    for ( const std::string& data : input ) {
        output << data << std::endl;
    }
}

int main() {
#ifdef __linux__
    FILE* pipe = popen( R"(       cloc . --include-ext=h,cc,cpp,hpp,c,java,py,ts,cmake --exclude-dir=build --not-match-d="node_modules|dist|target" 2>exception.log)", "r" );
#else
    FILE* pipe = popen( R"(wsl -e cloc . --include-ext=h,cc,cpp,hpp,c,java,py,ts,cmake --exclude-dir=build --not-match-d="node_modules|dist|target" 2>exception.log)", "r" );
#endif
    if ( !pipe ) {
        std::cerr << "Failed to open popen\n";
        return 1;
    }
    char buffer[4096];
    // Parse table lines
    std::regex re( R"(^(.+?)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+))" );
    std::vector<std::string> res{
        Entry::constructLine( "Language", "files", "blank", "comment", "code" ),
        Entry::constructLine( ":-------", "----:", "----:", "------:", "---:" ) };
    while ( fgets( buffer, sizeof( buffer ), pipe ) ) {
        std::string line( buffer );
        std::smatch m;
        if ( !std::regex_search( line, m, re ) ) {
            continue;  // skip empty lines and cloc line seperators
        }
        res.emplace_back( Entry::constructLine( m[1], m[2], m[3], m[4], m[5] ) );
    }
    if ( int status = pclose( pipe ); status == 0 ) {
        replaceFile( "README.md", "Language", res );
    } else {
        std::cout << "Runing cloc exit " << status << ", " << std::endl
                  << "cloc may have not been installed." << std::endl
                  << "Please run this programm without VS debugger" << std::endl
                  << "See ./exception.log for details." << std::endl;
    }
    return 0;
}