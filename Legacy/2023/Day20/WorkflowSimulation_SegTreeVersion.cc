#include <bits/stdc++.h>

#include "../../../utils/RegexIter.hpp"
using namespace std;
class WorkflowSimulation {
    struct Condition {
        string dest;
        char property;
        function<bool( int, int )> cmp;
        int threshold;
        Condition( string dest, char p = 0, function<bool( int, int )> cp = less{}, int tsh = 0 )
            : dest( dest ),
              property( p ),
              cmp( cp ),
              threshold( tsh ) {}
    };

    struct Part {
        int x, m, a, s;
        map<char, int> prop;
        Part( istringstream&& is ) {
            is >> x >> m >> a >> s;
            prop['x'] = x;
            prop['m'] = m;
            prop['a'] = a;
            prop['s'] = s;
        }
        int operator[]( char c ) const {
            return prop.at( c );
        }
    };

    map<string, vector<Condition>> ruleLists;
    vector<Part const*> acceptedList, rejectedList;
    vector<Part> PartList;

    unordered_map<char, function<bool( int, int )>> Comparator{
        { '<', less<int>{} },
        { '>', greater<int>{} }
        // ,        { '=', equal_to<int>{} } // eh, there is no equal_to case
    };

    generator<string> split( string const match, string pattern ) {
        string suffix;
        regex re( pattern );
        sregex_iterator _begin( match.begin(), match.end(), re ), _end;
        for ( ; _begin != _end; _begin++ ) {
            suffix = move( _begin->suffix() );
            co_yield _begin->prefix();
        }
        if ( !suffix.empty() ) {
            co_yield suffix;
        }
        co_return;
    }

    void readFile() {
        ifstream input( "input.txt" );
        for ( string buf; getline( input, buf ); ) {
            RegexIter workflowMatch( buf, R"(^.+(?=\{.*\}$))" );
            if ( workflowMatch.matched() ) {
                string workflowName = workflowMatch.str();
                buf = RegexIter( buf, R"(\{(.+)\})" ).group( 1 );
                for ( string curCondition : split( buf, R"(,)" ) ) {
                    RegexIter parser( curCondition, R"((\w)([><=])(\d+):(\w+))" );
                    if ( parser.matched() ) {
                        char propery = parser.group( 1 )[0];
                        char cmp = parser.group( 2 )[0];
                        int threshold = stoi( parser.group( 3 ) );
                        string dest = parser.group( 4 );
                        ruleLists[workflowName].emplace_back( dest, propery, Comparator[cmp], threshold );
                    } else {
                        ruleLists[workflowName].emplace_back( curCondition );
                    }
                }
            } else {
                break;
            }
        }
        for ( string buf; getline( input, buf ); ) {
            RegexIter PartMatch( buf, R"(x=(\d+),m=(\d+),a=(\d+),s=(\d+))" );
            vector<string> contents = PartMatch.groups();
            PartList.emplace_back( istringstream( accumulate( contents.begin(), contents.end(), string(), []( string const& init, string const& group ) {
                return init + " " + group;
            } ) ) );
        }
    }
    void Simulate( Part const& p ) {
        string curDest( "in" );
        vector<Condition> curRuleList;
        while ( !curDest.starts_with( "R" ) && !curDest.starts_with( "A" ) ) {
            curRuleList = ruleLists.at( curDest );
            auto it = curRuleList.cbegin();
            for ( ; it != curRuleList.cend(); it++ ) {
                if ( it == curRuleList.cend() - 1 || it->cmp( p[it->property], it->threshold ) ) {
                    curDest = it->dest;
                    break;
                }
            }
        }
        if ( curDest == "R" ) {
            // rejectedList.emplace_back( &p );
        } else {
            acceptedList.emplace_back( &p );
        }
    }
    struct Constraint {
        map<char, set<pair<int, int>>> propConstraints;
        optional<Constraint> joined( Condition const& rhs ) const {
            pair<int, int> rhsItv = getItv( rhs );
            return joinedAt( rhs.property, rhsItv );
        }
        optional<Constraint> excluded( Condition rhs ) const {
            pair<int, int> rhsItv = getItv( rhs );
            if ( rhsItv.first == 1 ) {
                return joinedAt( rhs.property, { rhs.threshold, 4000 } );
            } else {  // if ( rhsItv.second == 4000 )
                return joinedAt( rhs.property, { 1, rhs.threshold } );
            }
            // else {
            //     return joinedAt( rhs.property, { 1, rhs.threshold - 1 } )
            //         .value_or( *this )
            //         .joinedAt( rhs.property, { rhs.threshold + 1, 4000 } );
            // }
        }
        using ull = unsigned long long;
        ull CountProp() const {
            ull res = 1;
            for ( auto& [_, itvList] : propConstraints ) {
                res *= accumulate( itvList.begin(), itvList.end(), 0ll, []( long long init, pair<int, int> const& itv ) {
                    return init - itv.first + itv.second + 1;
                } );
            }
            return res;
        }

       private:
        optional<Constraint> joinedAt( char prop, pair<int, int> const& rhsItv ) const {
            auto const curItvSet = propConstraints.at( prop );
            auto itr = curItvSet.begin();
            while ( itr != curItvSet.end() && rhsItv.first > itr->second ) {
                itr++;
            }
            set<pair<int, int>> joinedConstraint;
            for ( ; itr != curItvSet.end(); itr++ ) {
                int l = max( itr->first, rhsItv.first ), r = min( itr->second, rhsItv.second );
                if ( l > r ) {
                    break;
                }
                joinedConstraint.emplace( l, r );
            }
            if ( !joinedConstraint.empty() ) {
                return ReplacedAt( prop, move( joinedConstraint ) );
            }
            return nullopt;
        }
        Constraint ReplacedAt( char const& prop, set<pair<int, int>>&& itv ) const {
            auto tmp = *this;
            tmp.propConstraints[prop] = move( itv );
            return tmp;
        }
        pair<int, int> getItv( Condition const& rhs ) const {
            if ( rhs.cmp( 0, rhs.threshold ) ) {  // cmp is less
                return { 1, rhs.threshold - 1 };
            } else if ( rhs.cmp( 4001, rhs.threshold ) ) {  // cmp is greater
                return { rhs.threshold + 1, 4000 };
            } else
                return { rhs.threshold, rhs.threshold };  // cmp is equal_to
        }
    };

    generator<Constraint> BFSPathList() {
        queue<tuple<string, Constraint>> q{
            {
                {
                    "in"s,  // string
                    {
                        { { 'x', { { 1, 4000 } } },
                          { 'm', { { 1, 4000 } } },
                          { 'a', { { 1, 4000 } } },
                          { 's', { { 1, 4000 } } } }  // map
                    }  // Constriant
                }  // tuple
            }  // deque
        };
        while ( !q.empty() ) {
            auto [curWkflN, curCstr] = move( const_cast<tuple<string, Constraint>&>( q.front() ) );
            q.pop();
            if ( curWkflN == "A" ) {
                co_yield curCstr;
                continue;
            }
            if ( ruleLists.contains( curWkflN ) ) {
                auto const& CdtList = ruleLists.at( curWkflN );
                auto itr = CdtList.cbegin();
                optional<Constraint> flowCstr = curCstr;
                for ( ; itr != CdtList.cend() - 1 && flowCstr.has_value(); itr++ ) {
                    auto nCstr = flowCstr.value().joined( *itr );
                    if ( nCstr.has_value() ) {
                        q.emplace( itr->dest, move( nCstr.value() ) );
                    }
                    flowCstr = move( flowCstr.value().excluded( *itr ) );
                }
                if ( itr == CdtList.cend() - 1 && flowCstr.has_value() )
                    q.emplace( itr->dest, move( flowCstr.value() ) );
            }
        }
        co_return;
    }

   public:
    void Solution1() {
        readFile();
        for ( Part const& p : PartList ) {
            Simulate( p );
        }
        unsigned long long res = 0;
        for ( auto& p : acceptedList ) {
            res += accumulate( p->prop.cbegin(), p->prop.cend(), 0ull, []( unsigned long long init, pair<const char, int> const& prop ) {
                return init + prop.second;
            } );
        }
        cout << "Solution 1: " << res << endl;
    }

    void Solution2() {
        using ull = unsigned long long;
        ull res = 0;
        for ( auto constraint : BFSPathList() ) {
            res += constraint.CountProp();
        }
        cout << "Solution 2: " << res << endl;
    }
};
int main() {
    WorkflowSimulation Day19;
    Day19.Solution1();
    Day19.Solution2();
}