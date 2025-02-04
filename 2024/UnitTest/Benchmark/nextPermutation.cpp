#include <iostream>
#include <vector>
#include <algorithm>

void test() {
    std::vector<int> nums = { 3,2,1 };

    // Copy the original state
    std::vector<int> original = nums;

    // Generate next permutation
    if( std::next_permutation( nums.begin(), nums.end() ) ) {
        std::cout << "Next permutation: ";
        for( int num : nums ) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "No next permutation, resetting to the original." << std::endl;
        nums = original;  // Restore original if needed
    }
    std::cout << "Next permutation: ";
    for( int num : nums ) {
        std::cout << num << " ";
    }
}
