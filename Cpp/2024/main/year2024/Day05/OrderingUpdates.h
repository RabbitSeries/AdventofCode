#include <bits/stdc++.h>
using namespace std;

#include <utils/SolutionBase.hpp>
class OrderingUpdates : public SolutionBase {
    bool isCorrectOrder( const vector<int>& update, const unordered_map<int, set<int>>& rules ) {
        for ( size_t i = 0; i < update.size(); i++ ) {
            for ( size_t j = 0; j < i; j++ ) {
                if ( rules.count( update[i] ) != 0 ) {
                    if ( rules.at( update[i] ).count( update[j] ) != 0 )
                        return false;
                } else {
                    break;
                }
            }
        }
        return true;
    }

    unordered_map<int, set<int>> rules;

    void readFile() {
        ifstream input = ifstream( "Day05/input.txt" );
        for ( string buf; getline( input, buf ); ) {
            if ( buf.empty() ) {
                break;
            }
            stringstream ss( buf );
            int num1 = 0, num2 = 0;
            char sep = '\0';
            ss >> num1 >> sep >> num2;
            rules[num1].insert( num2 );
        }
        readUpdates( input );
    }
    vector<vector<int>> updates;

    void readUpdates( ifstream& input ) {
        for ( string buf; getline( input, buf ); ) {
            stringstream ss( buf );
            int tmp = 0;
            vector<int> update;
            while ( ss >> tmp ) {
                update.push_back( tmp );
                char comma = '\0';
                if ( ( ss >> comma ) && comma == ',' ) {
                    continue;
                }
            }
            if ( update.size() > 0 )
                updates.push_back( update );
        }
        input.close();
    }

    vector<int> reOrder( vector<int> update, const unordered_map<int, set<int>>& rules ) {
        for ( size_t i = 0; i < update.size(); i++ ) {
            for ( size_t j = i; j < update.size(); j++ ) {
                bool ahead = true;
                for ( size_t k = i; k < update.size(); k++ ) {
                    if ( k == j ) {
                        continue;
                    }
                    if ( rules.count( update[k] ) != 0 && rules.at( update[k] ).count( update[j] ) != 0 ) {
                        ahead = false;
                        break;
                    }
                }
                if ( ahead ) {
                    swap( update[i], update[j] );
                    break;
                }
            }
        }
        return update;
    }

   public:
    void Solution1() {
        readFile();
        int sum = 0;
        for ( const auto& update : updates ) {
            if ( isCorrectOrder( update, rules ) ) {
                int middle = update.size() / 2;
                sum += update[middle];
            }
        }
        printRes( 1, sum );
    }

    void Solution2() {
        int sum = 0;
        for ( vector<int> const& update : updates ) {
            if ( !isCorrectOrder( update, rules ) ) {
                auto res = reOrder( update, rules );
                int middle = res.size() / 2;
                sum += res[middle];
            }
        }
        printRes( 2, sum );
    }
};
