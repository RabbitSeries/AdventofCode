#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;
#define buffer 1024
bool isCorrectOrder( const vector<int>& update, const vector<vector<int>>& rules ) {
    for( int i = 0; i < update.size(); i++ ) {
        for( int j = 0; j < i; j++ ) {
            for( int after : rules[update[i]] ) {
                if( after == update[j] ) {
                    return false;
                }
            }
        }
    }
    return true;
}

int main() {
    vector<vector<int>> rules( 100 );
    vector<vector<int>> updates;
    vector<vector<int>> correctUpdates;
    // Read the rules and updates from the input file
    // ...
    FILE* RuleInput = fopen( "RuleInput.txt", "r" );
    FILE* UpdatesInput = fopen( "UpdatesInput.txt", "r" );

    char linebuf[buffer + 1] = { '\0' };
    while( !feof( RuleInput ) && fgets( linebuf, buffer, RuleInput ) ) {
        stringstream ss( linebuf );
        int num1 = 0, num2 = 0; char sep = '\0';
        ss >> num1 >> sep >> num2;
        rules[num1].push_back( num2 );
    }
    linebuf[0] = '\0';
    while( !feof( UpdatesInput ) && fgets( linebuf, buffer, UpdatesInput ) ) {
        stringstream ss( linebuf );
        int tmp = 0;
        vector<int> update;
        while( ss >> tmp ) {
            update.push_back( tmp );
            char comma = '\0';
            if( ( ss >> comma ) && comma == ',' ) {
                continue;
            }
        }
        if( update.size() > 0 )
            updates.push_back( update );
    }
    for( const auto& update : updates ) {
        if( isCorrectOrder( update, rules ) ) {
            int middle = update.size() / 2;
        }
        else {
            correctUpdates.push_back( update );
        }
    }
    int sum = 0;


    cout << "The sum of the middle page numbers of the correctly reordered updates is: " << sum << endl;

    return 0;
}