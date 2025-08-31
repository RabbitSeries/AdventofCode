#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <memory>
// #include <mutex>
#include <sstream>
#include <string_view>
#include <type_traits>

template <class T>
concept is_ostream_insertable = requires { std::cout << std::declval<T>(); };
// template <typename T>
class ISolution {
    // static inline Register<KeyPair> _reg{}; // template static member's code won't be generated if it is never accessed outside definition
    // but non-template class's template member will be generated
   public:
    virtual void Solution1() = 0;
    virtual void Solution2() = 0;
    virtual ~ISolution() = default;

   protected:
    template <typename... Args>
    // requires requires { ( std::cout << ... << std::declval<Args>() ); }
        requires( is_ostream_insertable<Args> && ... )
    void printRes( int part, Args&&... args ) {
        ( ( std::cout << "\tSolution " << part << ": " ) << ... << std::forward<Args>( args ) ) << std::endl;  // fold expression
    }
};

using ImplPtr = std::unique_ptr<ISolution>;

struct SolutionRegistry {
    static inline std::vector<std::pair<std::string_view, std::function<ImplPtr()>>> registry{};
    // static inline std::mutex mtx{};
};

template <typename T>
consteval std::string_view type_name() {
// consteval should not contain std::cout or any runtime-only expressions
#if defined( __clang__ )
    std::string_view p = __PRETTY_FUNCTION__;
    return p.substr( p.find( '=' ) + 2, p.rfind( ']' ) - p.find( '=' ) - 2 );
#elif defined( __GNUC__ )
    std::string_view p = __PRETTY_FUNCTION__;
    // string_view's constructor is constexpr
    // std::string_view type_name() [with T = classSignature; std::string_view = std::basic_string_view<char>]
    return p.substr( p.find( '=' ) + 2, p.rfind( ';' ) - p.find( '=' ) - 2 );
#else
    return "UnknownType";
#endif
}

template <typename DerivedSolution>
class Register {
   public:
    Register() {
        // std::lock_guard regLock( SolutionRegistry::mtx ); I only have one translation unit, no need for locks
        SolutionRegistry::registry.emplace_back( type_name<DerivedSolution>(), []() -> ImplPtr {
            return std::make_unique<DerivedSolution>();
        } );
    }
};

#define REGISTER( CLASSNAME ) \
    static inline Register<CLASSNAME> _reg{};
