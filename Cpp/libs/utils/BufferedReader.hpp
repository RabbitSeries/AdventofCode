#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "./Stream/Generator.hpp"
#include "./Stream/ReadStream.hpp"
class BufferedReader {
    std::filesystem::path p;
    std::stringstream ss;

    class LineStream : public ReadStream<std::string> {
        std::stringstream ss;

       public:
        explicit LineStream( BufferedReader&& reader );
        utils::Generator<std::string> yield();
    };
    friend class LineStream;

   public:
    BufferedReader( std::filesystem::path path ) : p{ std::move( path ) } {
        if ( p.empty() || !std::filesystem::exists( p ) || !p.has_filename() ) {
            throw std::runtime_error( "File not found" );
        }
        ss << std::ifstream( p ).rdbuf();
    }

    LineStream lines() {
        return LineStream( std::move( *this ) );
    }
};

BufferedReader::LineStream::LineStream( BufferedReader&& reader ) : ss( std::move( reader.ss ) ) {}

utils::Generator<std::string> BufferedReader::LineStream::yield() {
    for ( std::string buf; std::getline( ss, buf ); ) {
        co_yield buf;
    }
}