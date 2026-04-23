#pragma once

#include <coroutine>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <ranges>
#include <utility>

namespace utils {
    template <class T>
    struct LazyGenerator : public std::ranges::view_interface<LazyGenerator<T>> {
        struct promise_type;
        using handle = std::coroutine_handle<promise_type>;
        handle h{};

        struct promise_type {
            T current{};

            promise_type() {}
            ~promise_type() {}

            LazyGenerator get_return_object() noexcept {  // Called on frame construction
                return LazyGenerator{ handle::from_promise( *this ) };
            }
            std::suspend_always initial_suspend() const noexcept {
                return {};
            }
            std::suspend_always final_suspend() noexcept {
                return {};
            }
            std::suspend_always yield_value( auto&& v ) noexcept {
                current = std::move( v );
                return {};
            }
            void return_void() const noexcept {}
            void unhandled_exception() const {
                std::terminate();
            }
            void await_transform() = delete;
        };
        explicit LazyGenerator( handle h_ ) : h( h_ ) {}
        LazyGenerator( const LazyGenerator& ) = delete;
        // For viewable_range<LazyGenerator<T>&&> -> ... || std::movable<utils::LazyGenerator<T>&&> && ...;
        LazyGenerator( LazyGenerator&& o ) noexcept : h( std::exchange( o.h, {} ) ) {
        }
        LazyGenerator& operator=( LazyGenerator other ) {
            std::swap( other.h, this->h );
        }
        ~LazyGenerator() {
            if ( h ) h.destroy();
        }
        struct iterator {
            // Adapter for std::ranges, std::input_range
            using difference_type = std::ptrdiff_t;
            using value_type = T;

            handle h{};

            iterator() = default;
            iterator( handle h_ ) : h( h_ ) {}

            iterator& operator++() {
                h.resume();
                return *this;
            }
            void operator++( int ) {
                this->operator++();
            }
            T&& operator*() const {
                return std::move( h.promise().current );
            }
            T* operator->() const {
                return std::addressof( h.promise().current );
            }
            bool operator==( std::default_sentinel_t ) const { return h.done(); }
        };

        iterator begin() {
            if ( h ) h.resume();
            return iterator{ h };
        }
        std::default_sentinel_t end() const { return {}; }
    };
}  // namespace utils
