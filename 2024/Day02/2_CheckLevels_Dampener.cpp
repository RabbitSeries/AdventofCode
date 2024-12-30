#include <algorithm>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

int checkDecrease(int start, vector<int>& Levels) {
    int i = start;
    while (i < Levels.size() - 1 && Levels[i] > Levels[i + 1] && Levels[i] - Levels[i + 1] <= 3) {
        i++;
    }
    return i;
}

int checkIncrease(int start, vector<int>& Levels) {
    int i = start;
    while (i < Levels.size() - 1 && Levels[i] < Levels[i + 1] && Levels[i + 1] - Levels[i] <= 3) {
        i++;
    }
    return i;
}

bool isSafe(vector<int>& Levels) {
    // Check if the report is already safe without removing any level
    int i = 0;
    if (checkDecrease(0, Levels) == Levels.size() - 1) return true;
    if (checkIncrease(0, Levels) == Levels.size() - 1) return true;
    
    return false;
}

bool canBeSafeByRemovingOneLevel(vector<int>& Levels) {
    // Try removing each level and check if the result is safe
    for (int i = 0; i < Levels.size(); ++i) {
        vector<int> newLevels = Levels;
        newLevels.erase(newLevels.begin() + i);
        
        if (isSafe(newLevels)) {
            return true;
        }
    }
    return false;
}

bool AnalyseLine(FILE* input) {
    char cline[1 << 10 + 1] = { '\0' };
    if (!fgets(cline, 1 << 10, input)) return false;
    
    string line(cline);
    stringstream lineInput(line);
    int tmpLevel = 0;
    vector<int> Levels;
    while (lineInput >> tmpLevel) {
        Levels.push_back(tmpLevel);
    }
    
    if (Levels.empty()) return false;

    // Check if the report is safe or can be made safe by removing one level
    if (isSafe(Levels) || canBeSafeByRemovingOneLevel(Levels)) {
        return true;
    }
    return false;
}

int main() {
    FILE* input = fopen("input.txt", "r");
    int cnt = 0;
    
    while (!feof(input)) {
        if (AnalyseLine(input)) {
            cnt++;
        }
    }
    
    cout << cnt;
    fclose(input);
    return 0;
}
