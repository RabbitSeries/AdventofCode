#include <cstddef>
#include <iostream>
#include <regex>
#include <string>
 
void show_matches(const std::string& in, const std::string& re)
{
    std::smatch m;
    std::regex_search(in, m, std::regex(re));
    if (!m.empty())
    {
        std::cout << "input=[" << in << "], regex=[" << re << "]\n  "
                     "prefix=[" << m.prefix() << "]\n  smatch: ";
        for (std::size_t n = 0; n < m.size(); ++n)
            std::cout << "m[" << n << "]=[" << m[n] << "] ";
        std::cout << "\n  suffix=[" << m.suffix() << "]\n";
    }
    else
        std::cout << "input=[" << in << "], regex=[" << re << "]: NO MATCH\n";
}
 
int main()
{
    // greedy match, repeats [a-z] 4 times
    show_matches("abcdefghi", "a[a-z]{2,4}");
    // non-greedy match, repeats [a-z] 2 times
    show_matches("abcdefghi", "a[a-z]{2,4}?");
 
    // Choice point ordering for quantifiers results in a match
    // with two repetitions, first matching the substring "aa",
    // second matching the substring "ba", leaving "ac" not matched
    // ("ba" appears in the capture clause m[1])
    show_matches("aabaac", "(aa|aabaac|ba|b|c)*");
 
    // Choice point ordering for quantifiers makes this regex 
    // calculate the greatest common divisor between 10 and 15
    // (the answer is 5, and it populates m[1] with "aaaaa")
    show_matches("aaaaaaaaaa,aaaaaaaaaaaaaaa", "^(a+)\\1*,\\1+$");
 
    // the substring "bbb" does not appear in the capture clause m[4]
    // because it is cleared when the second repetition of the atom
    // (a+)?(b+)?(c) is matching the substring "ac"
    // NOTE: gcc gets this wrong - it does not correctly clear the
    // matches[4] capture group as required by ECMA-262 21.2.2.5.1,
    // and thus incorrectly captures "bbb" for that group.
    show_matches("zaacbbbcac", "(z)((a+)?(b+)?(c))*");
}