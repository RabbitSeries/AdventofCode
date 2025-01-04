#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

// Function to count arrangements using dynamic programming
int countArrangementsDP(const std::vector<std::string>& patterns, const std::string& design) {
    int n = design.size();
    std::vector<int> dp(n + 1, 0);
    dp[0] = 1; // Base case: one way to form an empty design

    for (int i = 0; i <= n; ++i) {
        if (dp[i] == 0) continue; // Skip unreachable states

        for (const auto& pattern : patterns) {
            int patternSize = pattern.size();
            if (i + patternSize <= n && design.substr(i, patternSize) == pattern) {
                dp[i + patternSize] += dp[i];
            }
        }
    }

    return dp[n];
}

int main() {
    // Open input files
    FILE* keyInput = fopen("keys.txt", "r");
    FILE* designInput = fopen("input.txt", "r");

    if (!keyInput || !designInput) {
        std::cerr << "Error: Could not open input files!" << std::endl;
        return 1;
    }

    // Read towel patterns from keys.txt
    std::vector<std::string> patterns;
    char buffer[256];
    while (fscanf(keyInput, "%s", buffer) != EOF) {
        patterns.emplace_back(buffer);
    }
    fclose(keyInput);

    // Read designs from input.txt
    std::vector<std::string> designs;
    while (fscanf(designInput, "%s", buffer) != EOF) {
        designs.emplace_back(buffer);
    }
    fclose(designInput);

    // Compute the number of arrangements for each design
    int totalWays = 0;
    for (const auto& design : designs) {
        int ways = countArrangementsDP(patterns, design);
        std::cout << design << ": " << ways << " ways" << std::endl;
        totalWays += ways;
    }

    std::cout << "Total distinct arrangements: " << totalWays << std::endl;

    return 0;
}
