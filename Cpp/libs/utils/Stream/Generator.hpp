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
    struct Generator : public std::ranges::view_interface<Generator<T>> {
        struct promise_type;
        using handle = std::coroutine_handle<promise_type>;
        handle h{};

        struct promise_type {
            T current{};

            promise_type() {}
            ~promise_type() {}

            Generator get_return_object() noexcept {  // Called on frame construction
                return Generator{ handle::from_promise( *this ) };
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
        explicit Generator( handle h_ ) : h( h_ ) {}
        Generator( const Generator& ) = delete;
        // For viewable_range<Generator<T>&&> -> ... || std::movable<utils::Generator<T>&&> && ...;
        Generator( Generator&& o ) noexcept : h( std::exchange( o.h, {} ) ) {
        }
        Generator& operator=( Generator other ) {
            std::swap( other.h, this->h );
        }
        ~Generator() {
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
            const T& operator*() const {
                return h.promise().current;
            }
            const T* operator->() const {
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
