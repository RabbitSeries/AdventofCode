#include <cstddef>
#include <iostream>
#include <regex>
#include <string>

void show_matches( const std::string& in, const std::string& re ) {
    std::smatch m;
    std::regex_search( in, m, std::regex( re ) );
    if( !m.empty() ) {
        std::cout << "input=[" << in << "], regex=[" << re << "]\n  "
            "prefix=[" << m.prefix() << "]\n  smatch: ";
        for( std::size_t n = 0; n < m.size(); ++n )
            std::cout << "m[" << n << "]=[" << m[n] << "] ";
        std::cout << "\n  suffix=[" << m.suffix() << "]\n";
    }
    else
        std::cout << "input=[" << in << "], regex=[" << re << "]: NO MATCH\n";
}

int main() {
    show_matches( "abcdef", "abc|def" );
    show_matches( "abc", "ab|abc" ); // left Alternative matched first

    // Match of the input against the left Alternative (a) followed
    // by the remained of the regex (c|bc) succeeds, which results
    // in m[1]="a" and m[4]="bc".
    // The skipped Alternatives (ab) and (c) leave their submatches
    // m[3] and m[5] empty.
    show_matches( "abc", "((a)|(ab))((c)|(bc))" );

    show_matches( "abcdef", "" ); // empty regex is a single empty Alternative
    show_matches( "abc", "abc|" ); // left Alternative matched first
    show_matches( "abc", "|abc" ); // left Alternative matched first, leaving abc unmatched

    // greedy match, repeats [a-z] 4 times
    show_matches( "abcdefghi", "a[a-z]{2,4}" );
    // non-greedy match, repeats [a-z] 2 times
    show_matches( "abcdefghi", "a[a-z]{2,4}?" );

    // Choice point ordering for quantifiers results in a match
    // with two repetitions, first matching the substring "aa",
    // second matching the substring "ba", leaving "ac" not matched
    // ("ba" appears in the capture clause m[1])
    show_matches( "aabaac", "(aa|aabaac|ba|b|c)*" );

    // Choice point ordering for quantifiers makes this regex 
    // calculate the greatest common divisor between 10 and 15
    // (the answer is 5, and it populates m[1] with "aaaaa")
    show_matches( "aaaaaaaaaa,aaaaaaaaaaaaaaa", "^(a+)\\1*,\\1+$" );

    // the substring "bbb" does not appear in the capture clause m[4]
    // because it is cleared when the second repetition of the atom
    // (a+)?(b+)?(c) is matching the substring "ac"
    // NOTE: gcc gets this wrong - it does not correctly clear the
    // matches[4] capture group as required by ECMA-262 21.2.2.5.1,
    // and thus incorrectly captures "bbb" for that group.
    show_matches( "zaacbbbcac", "(z)((a+)?(b+)?(c))*" );

    // matches the a at the end of input
    show_matches( "aaa", "a$" );

    // matches the o at the end of the first word
    show_matches( "moo goo gai pan", "o\\b" );

    // the lookahead matches the empty string immediately after the first b
    // this populates m[1] with "aaa" although m[0] is empty
    show_matches( "baaabac", "(?=(a+))" );

    // because backtracking into lookaheads is prohibited, 
    // this matches aba rather than aaaba
    // For the first match the Disjunction fails, making the entire regex moves to "b".
    // Then, a+ matched aaa, and a* matched aaa, b matched b, but \1 aaa fails. And the backtracking to fewer times of repetition in (?=a+) is forhibited, so backtracking to a* won't match 
    // So the entire regex moves to "ba" instead of cusumed by lookahead assertion, backtracking to fewer time of repetition. 
    show_matches( "baaabac", "(?=(a+))a*b\\1" );

    // logical AND via lookahead: this password matches IF it contains
    // at least one lowercase letter
    // AND at least one uppercase letter
    // AND at least one punctuation character
    // AND be at least 6 characters long
    show_matches( "abcdef", "(?=.*[[:lower:]])(?=.*[[:upper:]])(?=.*[[:punct:]]).{6,}" );
    show_matches( "aB,def", "(?=.*[[:lower:]])(?=.*[[:upper:]])(?=.*[[:punct:]]).{6,}" );
    return 0;
}