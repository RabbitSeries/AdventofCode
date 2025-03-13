#include <bits/stdc++.h>
using namespace std;
int main() {
    regex re0( "(\\w)\\s(\\d+)\\s\\(#([\\d\\w]+)\\)" );
    // Raw string must be equipped with integrate brackets.
    regex re( R"((\w)\s(\d+)\s\(#([\d\w]+)\))" );
    smatch m;
    match_results<string::const_iterator> m_r;
    string buf{ "hello" };
    regex_search( buf, re );
    regex_search( buf, m, re );
    regex_search( (string::const_iterator)buf.begin(), (string::const_iterator)buf.end(), m, re );
    regex_search( (string::const_iterator)buf.begin(), (string::const_iterator)buf.end(), m_r, re );
    regex_search( buf, m_r, re );
    regex_search( buf.begin(), buf.end(), re );
    cmatch cm;
    regex_search( "const char", re );
    regex_search( "const char", cm, re );
    wcmatch wcm;
    wregex Lre( L"ABS" );
    regex_search( L"ABCDEF", wcm, Lre );
    string const str{ "const char" };
    regex_search( str.begin(), str.end(), m, re );
    return 0;
}