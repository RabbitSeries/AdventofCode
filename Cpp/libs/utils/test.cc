#include <chrono>
#include <filesystem>
#include <fstream>
#include <generator>
#include <iostream>
#include <sstream>

#include "utils/BufferedReader.hpp"

auto now() {
    return std::chrono::high_resolution_clock::now();
}

auto begin = now();

int64_t delta() {
    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>( now() - begin ).count();
    begin = now();
    return delta;
}
std::generator<std::string> yield() {
    std::stringstream ss;
    std::filesystem::path p( "Day22/input.txt" );
    if ( p.empty() || !std::filesystem::exists( p ) || !p.has_filename() ) {
        throw std::runtime_error( "File not found" );
    }
    ss << std::ifstream( p ).rdbuf();
    for ( std::string line; std::getline( ss, line ); ) {
        co_yield std::move( line );
    }
}
utils::Generator<std::string> yield_gen() {
    std::stringstream ss;
    std::filesystem::path p( "Day22/input.txt" );
    if ( p.empty() || !std::filesystem::exists( p ) || !p.has_filename() ) {
        throw std::runtime_error( "File not found" );
    }
    ss << std::ifstream( p ).rdbuf();
    for ( std::string line; std::getline( ss, line ); ) {
        co_yield std::move( line );
    }
}

int main() {
    begin = now();
    for ( int i = 0; i < 1000; i++ ) {
        std::stringstream ss;
        ss << std::ifstream( "Day22/input.txt" ).rdbuf();
        std::vector<std::string> lines;
        for ( std::string line; std::getline( ss, line ); ) {
            lines.emplace_back( std::move( line ) );
        }
    }
    std::cout << delta() << std::endl;
    for ( int i = 0; i < 1000; i++ ) {
        std::vector<std::string> lines = yield() | std::ranges::to<std::vector<std::string>>();
    }
    std::cout << delta() << std::endl;
    for ( int i = 0; i < 1000; i++ ) {
        auto read = BufferedReader( "Day22/input.txt" ).lines().toList();
    }
    std::cout << delta() << std::endl;
    for ( int i = 0; i < 1000; i++ ) {
        std::vector<std::string> lines = yield_gen() | std::ranges::to<std::vector<std::string>>();
    }
    std::cout << delta() << std::endl;
}
