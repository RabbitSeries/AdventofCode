#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string_view>

// template <typename T>
class SolutionBase {
    // static inline Register<KeyPair> _reg{}; // template static member's code won't be generated if it is never accessed outside definition
    // but non-template class's template member will be generated
   public:
    virtual void Solution1() = 0;
    virtual void Solution2() = 0;

   protected:
    template <typename... Args>
    void printRes( int part, Args&&... args ) {
        using namespace std;
        ( ( cout << "\tSolution " << part << ": " ) << ... << forward<Args>( args ) ) << endl;  // fold expression
    }
};

using ImplPtr = std::unique_ptr<SolutionBase>;

struct SolutionRegistry {
    static inline std::vector<std::pair<std::string_view, std::function<ImplPtr()>>> registry{};
    static inline std::mutex mtx{};
};

template <typename T>
consteval std::string_view type_name() {
#if defined( __clang__ )
    std::string_view p = __PRETTY_FUNCTION__;
    return p.substr( p.find( '=' ) + 2, p.rfind( ']' ) - p.find( '=' ) - 2 );
#elif defined( __GNUC__ )
    std::string_view p = __PRETTY_FUNCTION__;
    return p.substr( p.find( '=' ) + 2, p.rfind( ';' ) - p.find( '=' ) - 2 );
#else
    return "UnknownType";
#endif
}

template <typename DerivedSolution>
class Register {
   public:
    Register() {
        SolutionRegistry::registry.emplace_back( type_name<DerivedSolution>(), []() -> ImplPtr {
            return std::make_unique<DerivedSolution>();
        } );
    }
};

#define REGISTER( CLASSNAME ) \
    static inline Register<CLASSNAME> _reg{};
