#pragma once

#include <ranges>
#include <vector>

#include "Generator.hpp"
template <typename T>
class ReadStream {
   public:
    virtual ~ReadStream() {};
    virtual std::vector<T> toList() {
        return yield() | std::ranges::to<std::vector<T>>();
    }
    virtual utils::Generator<T> yield() = 0;

   private:
};