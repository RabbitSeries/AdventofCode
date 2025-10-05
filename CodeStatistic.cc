#include <algorithm>
#include <cstdio>  // for popen(), pclose()
#include <fstream>
#include <generator>
#include <iomanip>  // for setw
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
// constexpr int lang_w = 19, digit_w = 15;
constexpr int lang_w = 19, digit_w = 14;

namespace Entry {
    template <typename T, typename... rest>
    static std::string constructLine( T&& lang, rest&&... args ) {
        std::ostringstream oss;
        oss << std::left << std::setw( lang_w ) << lang;
        ( ..., ( oss << "|" << std::right << std::setw( digit_w ) << std::forward<rest>( args ) ) );
        return oss.str();
    }
};  // namespace Entry

std::generator<std::string> splitlines( std::ifstream& ifs ) {
    for ( std::string buf; getline( ifs, buf ); ) {
        co_yield buf;
    }
    co_return;
}

void replaceFile( const char* filename, const char* linestartWith, std::vector<std::string> const& replaceWith ) {
    std::ifstream ifs( filename, std::ios::in );
    std::stringstream output_buffer;
    for ( auto&& line : splitlines( ifs ) ) {
        if ( line.starts_with( linestartWith ) ) {
            for ( auto& line : replaceWith ) {
                output_buffer << line << std::endl;
            }
            std::ofstream( filename, std::ios::out | std::ios::trunc ) << output_buffer.str();
            break;
        }
        output_buffer << line << std::endl;
    }
}

int main() {
    // There is a bug for resolving special extension name of CMakeLists.txt in perl script:
    // https://github.com/AlDanial/cloc/blob/dfaa4618ab7057bebb9e9dbe093f5d56d5fc13ab/Unix/cloc#L2664-L2672
#ifdef __linux__
    FILE* pipe = popen( R"(       cloc . --include-lang="C/C++ Header,C++,CMake,TypeScript,Java,Python,Kotlin" --exclude-dir=build --not-match-d="node_modules|dist|target" 2>exception.log)", "r" );
#else
    // FILE* pipe = popen( R"(wsl -e cloc . --include-ext=h,cc,cpp,hpp,c,java,py,ts,cmake,CMakeLists.txt" --exclude-dir=build --not-match-d="node_modules|dist|target" 2>exception.log)", "r" );
    FILE* pipe = popen( R"(wsl -e cloc . --include-lang="C/C++ Header,C++,CMake,TypeScript,Java,Python,Kotlin" --exclude-dir=build --not-match-d="node_modules|dist|target" 2>exception.log)", "r" );
#endif
    if ( !pipe ) {
        std::cerr << "Failed to open popen\n";
        return 1;
    }
    char buffer[4096];
    // Parse table lines
    std::regex re( R"(^\b(.+?)((?:\s+\d+){4}))" );
    std::vector<std::string> res{
        Entry::constructLine( "Language", "files", "blank", "comment", "code" ),
        Entry::constructLine( ":-------", "----:", "----:", "------:", "---:" ) };
    while ( std::fgets( buffer, sizeof( buffer ), pipe ) ) {
        std::string line( buffer );
        std::smatch m;
        if ( std::regex_search( line, m, re ) ) {
            int files, blank, comment, code;
            std::istringstream( m[2] ) >> files >> blank >> comment >> code;
            res.emplace_back( Entry::constructLine( m[1], files, blank, comment, code ) );
        }
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