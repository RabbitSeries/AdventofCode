
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
template <typename _Tp, typename _C = std::vector<_Tp>>
class stream {
    _C c;

   public:
    stream( std::initializer_list<_Tp> vec ) : c{ vec } {
    }
    std::string join( std::string s ) {
        using std::operator""s;
        return reduce( ""s, [=]( std::string const& init, _Tp const& elem ) {
            return ( std::ostringstream() << init << s << elem ).str();
        } );
    }
    template <typename _BinaryPred>
    _Tp reduce( _Tp init, _BinaryPred func ) {
        return std::accumulate( ++std::begin( c ), std::end( c ), *std::begin( c ), func );
    }
};