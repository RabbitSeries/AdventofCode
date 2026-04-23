#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

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

struct SolutionRegistry {
    using ImplPtr = std::unique_ptr<ISolution>;
    struct Entry {
        int day;
        std::string_view name;
        std::function<ImplPtr()> factory;
        bool operator>( const Entry& rhs ) const {
            return day > rhs.day;
        }
    };

    static inline std::priority_queue registry = std::priority_queue( std::greater<>{}, std::vector<Entry>{} );
    static inline std::mutex mtx{};
};

template <typename T>
inline consteval std::string_view type_name() {
// consteval should not contain std::cout or any runtime-only expressions
#ifdef __clang__
    std::string_view p = __PRETTY_FUNCTION__;
    return p.substr( p.find( '=' ) + 2, p.rfind( ']' ) - p.find( '=' ) - 2 );
#elif defined( __GNUC__ )
    std::string_view p = __PRETTY_FUNCTION__;
    // string_view's constructor is constexpr
    // std::string_view type_name() [with T = classSignature; std::string_view = std::basic_string_view<char>]
    return p.substr( p.find( '=' ) + 2, p.rfind( ';' ) - p.find( '=' ) - 2 );
#elif defined( _MSC_VER )
    std::string_view p = __FUNCSIG__;
    std::string_view tArg = p.substr( p.rfind( "<" ) + 1, p.rfind( ">" ) - p.rfind( "<" ) - 1 );
    if ( tArg.starts_with( "class " ) ) {
        tArg = tArg.substr( 6 );
    }
    return tArg;
#else
    return "UnknownType";
#endif
}

template <typename DerivedSolution>
class Register {
    static int parse_day_from_file( std::string_view path ) {
        std::regex re( R"(Day(\d+))" );
        std::cmatch m;
        if ( std::regex_search( path.data(), m, re ) ) {
            return std::stoi( m[1] );
        }
        return -1;
    }

    static SolutionRegistry::ImplPtr solutionFactory() {
        return std::make_unique<DerivedSolution>();
    }

   public:
    explicit inline Register( std::string_view sourceFile ) {
        std::lock_guard regLock( SolutionRegistry::mtx );
        if ( int day = parse_day_from_file( std::move( sourceFile ) ); day != -1 ) {
            SolutionRegistry::registry.emplace( SolutionRegistry::Entry{ day, type_name<DerivedSolution>(), &Register::solutionFactory } );
        }
    }
};

#define REGISTER( CLASSNAME ) \
    static inline Register<CLASSNAME> _reg{ __FILE__ };
