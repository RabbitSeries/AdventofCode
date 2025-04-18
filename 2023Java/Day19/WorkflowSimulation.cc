#include <bits/stdc++.h>

#include "../../utils/RegexIter.hpp"
using namespace std;
class WorkflowSimulation {
    struct condition {
        string destination;
        char property;
        function<bool( int, int )> cmp;
        int threshold;
        condition( string dest, char p = 0, function<bool( int, int )> cp = less{}, int tsh = 0 )
            : destination( dest ),
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

    map<string, vector<condition>> ruleLists;
    vector<Part const*> acceptedList, rejectedList;
    vector<Part> PartList;

    unordered_map<char, function<bool( int, int )>> comparator{
        { '<', less<int>{} },
        { '>', greater<int>{} },
        { '=', equal_to<int>{} } };

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
                        string destination = parser.group( 4 );
                        ruleLists[workflowName].emplace_back( destination, propery, comparator[cmp], threshold );
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
        vector<condition> curRuleList;
        while ( curDest != "R" && curDest != "A" ) {
            curRuleList = ruleLists.at( curDest );
            auto it = curRuleList.cbegin();
            for ( ; it != curRuleList.cend(); it++ ) {
                if ( it == curRuleList.cend() - 1 || it->cmp( p[it->property], it->threshold ) ) {
                    curDest = it->destination;
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
        cout << "Solution 1: " << res;
    }
};
int main() {
    WorkflowSimulation Day19;
    Day19.Solution1();
}