abcdef ab|abc
abc ab|abc
abc ((a)|(ab))((c)|(bc))

```cpp
// search and replace.
// m[0] is the entire match, while m[1-5] is the submatch within.
zaacbbbcac (z)((a+)?(b+)?(c))* m[1]=$1 m[2]=$2 m[3]=$3 m[4]=$4 m[5]=$5
```

zaacbbbcac (z)((a+)?(b+)?(c))*z z
zaacbbbcac (z)((a+)?(b+)?(c))*? z z
zaacbbbcac (z)((a+)?(b+)?(c))+ z z
zaacbbbcac (z)((a+)?(b+)?(c))+? z z
abcdefghi a[a-z]{2,4}?

abc a(b?)
abc a(b??)
