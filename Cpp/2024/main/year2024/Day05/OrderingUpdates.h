#include <algorithm>
#include <fstream>
#include <iterator>
#include <memory>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "utils/BufferedReader.hpp"
#include "utils/SolutionBase.hpp"
#include "utils/Stream/RegexStream.hpp"
class OrderingUpdates : public SolutionBase {
    REGISTER( OrderingUpdates )

    bool isCorrectOrder( const std::vector<int>& update ) {
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

    std::unordered_map<int, std::set<int>> rules;

    std::vector<int> consume( const std::string& range ) {
        using namespace std;
        return RegexStream( R"(\d+)", range ).yield() |
               views::transform( []( const std::smatch& m ) { return stoi( m.str() ); } ) |
               ranges::to<vector<int>>();
    }

    void readFile() {
        using namespace std;
        auto input = BufferedReader( "Day05/input.txt" ).lines().yield();
        auto itr = input.begin();
        while ( itr != input.end() ) {
            if ( itr->empty() ) {
                itr++;
                break;
            }
            auto nums = consume( *itr );
            rules[nums[0]].insert( nums[1] );
            itr++;
        }
        while ( itr != input.end() ) {
            updates.emplace_back( consume( *itr ) );
            itr++;
        }
    }

    std::vector<std::vector<int>> updates;

    std::vector<int> reOrdered( std::vector<int> update ) {
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
                    std::swap( update[i], update[j] );
                    break;
                }
            }
        }
        return update;
    }

   public:
    void Solution1() {
        readFile();
        using namespace std;
        printRes( 1, ranges::fold_left( updates |
                                            views::filter( [this]( const vector<int>& update ) {
                                                return isCorrectOrder( update );
                                            } ) |
                                            views::transform( []( const vector<int>& update ) {
                                                return update[update.size() / 2];
                                            } ),
                                        0, plus<>{} ) );
    }

    void Solution2() {
        using namespace std;
        printRes( 2, ranges::fold_left( updates |
                                            views::filter( [this]( const vector<int>& update ) {
                                                return !isCorrectOrder( update );
                                            } ) |
                                            views::transform( [this]( const vector<int>& update ) {
                                                return reOrdered( update );
                                            } ) |
                                            views::transform( []( const vector<int>& update ) {
                                                return update[update.size() / 2];
                                            } ),
                                        0, plus<>{} ) );
    }
};
