#pragma once

#include <regex>
#include <string>

#include "./ReadStream.hpp"
class RegexStream : public ReadStream<std::smatch> {
    // public inherit to expose toList() api
    std::string Pattern;
    std::string Range;

   public:
    RegexStream( const char* pattern, const std::string& range ) : Pattern( pattern ), Range( range ) {}
    utils::Generator<std::smatch> yield() {
        return yield( Pattern, Range );
    }
    utils::Generator<std::smatch> yield( std::string Pattern, std::string Range ) {
        std::regex re( Pattern );
        for ( std::sregex_iterator itr = std::sregex_iterator( Range.begin(), Range.end(), re ), endItr; itr != endItr; itr++ ) {
            co_yield *itr;
        }
    }
    ~RegexStream() {}
};
