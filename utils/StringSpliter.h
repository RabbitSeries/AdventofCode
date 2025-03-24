#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <boost/algorithm/string.hpp>
typedef long long ll;
typedef unsigned long long ull;
using namespace std;

vector<string> split(string const str, string regexString)
{
    vector<string> res;
    if (str.empty())
    {
        return res;
    }
    regex re(regexString);
    sregex_iterator it(str.begin(), str.end(), re), end_it;
    string suffix = "";
    if (distance(it, end_it) > 0)
    {
        while (distance(it, end_it) > 0)
        {
            res.push_back((*it).prefix());
            suffix = (*it).suffix();
            it++;
        }
        res.push_back(suffix);
    }
    return move(res);
}

ll parseLL(string str)
{
    ll val = 0;
    stringstream(str) >> val;
    return val;
}

ull parseULL(string str)
{
    ull val = 0;
    stringstream(str) >> val;
    sscanf(str.c_str(), "%llu", &val);
    return val;
}

string trim(string s)
{
    regex re("^\\s+");
    sregex_iterator it(s.begin(), s.end(), re), end_it;
    if (distance(it, end_it) != 0)
    {
        re = regex("\\b");
        if (distance(it, end_it) != 0)
        {
            it = sregex_iterator(s.begin(), s.end(), re);
            s = s.substr(((*it).prefix().str().length()));
        }
        else
            return "";
    }
    re = regex("\\s+$");
    it = sregex_iterator(s.begin(), s.end(), re);
    if (distance(it, end_it) != 0)
    {
        re = regex("\\b\\s+$");
        it = sregex_iterator(s.begin(), s.end(), re);
        s = s.substr(0, (*it).prefix().str().length());
    }
    return s;
}

int main()
{
    // for( string s : split( "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green", ":" ) ) {
    //     cout << s << endl;
    // }
    vector<string> gameInfo = split("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green", ":");
    int gameId = parseLL(split(gameInfo[0], " ")[1]);
    cout << gameId << endl;
    vector<string> res;
    boost::split(res, "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green", boost::is_any_of(":"));
    for_each(res.begin(), res.end(), [](string str)
             { cout << str << endl; });
}