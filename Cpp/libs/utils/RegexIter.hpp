#pragma once
#include<string>
#include<regex>
class RegexIter {
    std::string matchStr, pattern;
    class itr {
        std::string matchStr;
        std::regex pattern;
        std::sregex_iterator iterator;

       public:
        itr() = default;
        itr( std::string_view _matchStr, std::string_view _pattern ) : matchStr( _matchStr ), pattern{ _pattern.data() }, iterator( matchStr.begin(), matchStr.end(), pattern ) {
        }
        const std::string operator*() {
            if ( iterator->size() != 0 )
                return iterator->str();
            return {};
        }
        itr& operator++() {
            iterator++;
            return *this;
        }
        itr& operator++( int ) {
            iterator++;
            return *this;
        }
        const std::smatch* operator->() {
            return iterator.operator->();
        }
        bool operator!=( itr const& rhs ) const {
            // return std::distance( iterator, rhs.iterator ) != 0;
            return iterator != rhs.iterator;
        }
        std::ptrdiff_t count() {
            return std::distance( iterator, std::sregex_iterator() );
        }
    };
    itr _begin, _end;

   public:
    RegexIter( std::string_view _matchStr, std::string_view _pattern )
        : matchStr( _matchStr ),
          pattern( _pattern.data() ),
          _begin( matchStr, pattern ),
          _end{} {}
    std::string str() {
        return _begin->str();
    }
    std::ptrdiff_t matchCount() {
        return _begin.count();
    }
    std::ptrdiff_t groupCount() {
        return _begin->size();
    }
    std::vector<std::string> groups() {
        std::ptrdiff_t size = groupCount();
        std::vector<std::string> totalGroups;
        totalGroups.reserve( size - 1 >= 0 ? size - 1 : 0 );
        for ( int i = 1; i < size; i++ ) {
            totalGroups.emplace_back( move( group( i ) ) );
        }
        return totalGroups;
    }
    std::vector<std::string> findAll() {
        std::vector<std::string> res;
        for ( auto __begin = _begin; __begin != _end; __begin++ ) {
            res.push_back( __begin->str() );
        }
        return res;
    }
    std::string group( size_t group ) {
        return _begin->operator[]( group ).str();
    }
    bool matched() {
        return !_begin->empty();
    }
    itr begin() {
        return _begin;
    }
    itr end() {
        return itr();
    }
};
