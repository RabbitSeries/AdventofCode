#pragma once

#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>

#include "LazyGenerator.hpp"

template <class T>
inline std::vector<T> toList( utils::LazyGenerator<T> generator ) {
    return generator | std::ranges::to<std::vector<T>>();
}

inline utils::LazyGenerator<std::smatch> regexStream( std::string_view pattern, const std::string& range ) {
    std::regex re( pattern.data() );
    for ( std::sregex_iterator itr( range.begin(), range.end(), re ), endItr; itr != endItr; itr++ ) {
        co_yield *itr;
    }
}

inline utils::LazyGenerator<std::string> fileLinesStream( const char* file ) {
    std::filesystem::path p( file );
    if ( !std::filesystem::exists( p ) || !p.has_filename() ) {
        throw std::runtime_error( "File not found" );
    }
    std::ifstream fstream( p );
    std::string buf;
    while ( std::getline( fstream, buf ) ) {
        co_yield std::move( buf );
    }
}
