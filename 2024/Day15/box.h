#include "bits/stdc++.h"
#include <termios.h>
using namespace std;
#define CELLWALL -1
#define CELLEMPTY -2
#define BOT -3
#define BUF 4096
typedef struct pos {
    pos() {};
    pos( int a, int b ) : x( a ), y( b ) {}
    int x, y;
}pos;
typedef struct boxPos {
    boxPos() {}
    boxPos( pos a, pos b ) : l( a ), r( b ) {}
    pos l, r;
}bos;
typedef long long ll;
void enableRawMode() {
    termios term;
    tcgetattr( STDIN_FILENO, &term );          // Obtain terminal setting
    term.c_lflag &= ~( ICANON | ECHO );        // Shut down standard IO and echo
    tcsetattr( STDIN_FILENO, TCSANOW, &term ); // immediate effect
}

void disableRawMode() {
    termios term;
    tcgetattr( STDIN_FILENO, &term );
    term.c_lflag |= ( ICANON | ECHO );         // restore standard IO and echo
    tcsetattr( STDIN_FILENO, TCSANOW, &term );
}

char getMoveMent() {
    char c;
    while( 1 ) {
        c = getchar();
        if( c == '\033' ) { // Detext escape character
            getchar();     // jump '['
            switch( getchar() ) { // read the third character of direction key
            case 'A': return '^';
            case 'B': return 'v';
            case 'C': return '>';
            case 'D': return '<';
            default: std::cout << "Unknown key" << std::endl; break;
            }
        }
        else if( c == 27 ) { // ESC pressed
            return 0;
        }
        else {
            // std::cout << "Normal key: " << c << std::endl;
            continue;
        }
    }
}

void readMap( vector<vector<int>>& arcade, vector<boxPos>& id2BoxPos, pos& start ) {
    ifstream input( "input.txt" );
    string buf = "\0";
    while( getline( input, buf ) ) {
        vector<int> row;
        for( char c : buf ) {
            if( c != '\n' && c != '\0' ) {
                switch( c ) {
                case '#':
                    row.push_back( CELLWALL );
                    row.push_back( CELLWALL );
                    break;
                case '.':
                    row.push_back( CELLEMPTY );
                    row.push_back( CELLEMPTY );
                    break;
                case 'O':
                    row.push_back( id2BoxPos.size() );
                    row.push_back( id2BoxPos.size() );
                    id2BoxPos.push_back( boxPos( pos( arcade.size(), row.size() - 2 ), pos( arcade.size(), row.size() - 1 ) ) );
                    break;
                case '@':
                    start = pos( arcade.size(), row.size() );
                    row.push_back( CELLEMPTY );
                    row.push_back( CELLEMPTY );
                    break;
                default:
                    break;
                }
            }
        }
        arcade.push_back( row );
    }
}

void printGUI( vector<vector<int>> const acrade, pos const curPos ) {
    system( "clear" );
    vector<vector<int>> GUI = acrade;
    GUI[curPos.x][curPos.y] = BOT;
    for( auto line : GUI ) {
        bool first = true;
        for( auto id : line ) {
            if( id >= 0 ) {
                // cout << 'O';
                if( first ) {
                    first = false;
                    cout << "[";
                }
                else {
                    first = true;
                    cout << "]";
                }
            }
            else if( id == CELLWALL ) {
                cout << '#';
            }
            else if( id == CELLEMPTY ) {
                cout << ' ';
            }
            else {
                cout << '@';
            }
        }
        cout << endl;
    }
    return;
}

pos getNextPos( char const c, pos const curPos ) {
    pos nextPos;
    switch( c ) {
    case '^':
        nextPos = pos( curPos.x - 1, curPos.y );
        break;
    case '>':
        nextPos = pos( curPos.x, curPos.y + 1 );
        break;
    case 'v':
        nextPos = pos( curPos.x + 1, curPos.y );
        break;
    case '<':
        nextPos = pos( curPos.x, curPos.y - 1 );
        break;
    default:
        cout << "Error situation." << endl;
        break;
    }
    return nextPos;
}

boxPos getBoxPosAtPos( vector<boxPos> const id2BoxPos, vector<vector<int>> const pos2Id, pos const curPos ) {
    return id2BoxPos[pos2Id[curPos.x][curPos.y]];
}

bool isBox( vector<vector<int>> const pos2Id, pos point ) {
    return pos2Id[point.x][point.y] >= 0;
}
bool isWALL( vector<vector<int>> const pos2Id, pos point ) {
    return pos2Id[point.x][point.y] == CELLWALL;
}
bool isEmpty( vector<vector<int>> const pos2Id, pos point ) {
    return pos2Id[point.x][point.y] == CELLEMPTY;
}

ll sumCoordinates( vector<boxPos> const id2BoxPos, vector<vector<int>> const acrade ) {
    ll res = 0;
    for( auto boxPos : id2BoxPos ) {
        res += 100 * boxPos.l.x + boxPos.l.y;
    }
    return res;
}

int countWall( vector<vector<int>> const acrade ) {
    int wallCnt = 0;
    for( int i = 0; i < acrade.size(); i++ ) {
        for( int j = 0; j < acrade[i].size(); j++ ) {
            if( acrade[i][j] == CELLWALL ) {
                // cout << "Pos" << i << "," << j << endl;
                wallCnt++;
            }

        }

    }
    return wallCnt;
}
