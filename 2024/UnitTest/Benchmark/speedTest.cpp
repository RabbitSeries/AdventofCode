#include "iostream"
#include "chrono"
typedef unsigned long long ull;
void  test2() {
    auto cur = std::chrono::high_resolution_clock::now();
    ull sum = 0;
    for( size_t i = 0; i < 0x7FFFFFFFull; i++ ) {
        sum += i;

    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>( end - cur ).count() / 1000 << std::endl;
}