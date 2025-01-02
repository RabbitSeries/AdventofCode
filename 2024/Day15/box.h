#include "bits/stdc++.h"
#include <termios.h>
using namespace std;
#define WALL -1
#define EMPTY -2
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
    tcgetattr( STDIN_FILENO, &term );          // 获取当前终端设置
    term.c_lflag &= ~( ICANON | ECHO );        // 关闭规范模式和回显
    tcsetattr( STDIN_FILENO, TCSANOW, &term ); // 立即生效
}

void disableRawMode() {
    termios term;
    tcgetattr( STDIN_FILENO, &term );
    term.c_lflag |= ( ICANON | ECHO );         // 恢复规范模式和回显
    tcsetattr( STDIN_FILENO, TCSANOW, &term );
}

char getMoveMent() {
    char c;
    while( 1 ) {
        c = getchar();
        if( c == '\033' ) { // 检测转义序列（ESC 键）
            getchar();     // 跳过 '['
            switch( getchar() ) { // 读取方向键的第三个字符
            case 'A': return '^';
            case 'B': return 'v';
            case 'C': return '>';
            case 'D': return '<';
            default: std::cout << "Unknown key" << std::endl; break;
            }
        }
        else if( c == 27 ) { // ESC 键单独按下
            return 0;
        }
        else {
            // std::cout << "Normal key: " << c << std::endl;
            continue;
        }
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
