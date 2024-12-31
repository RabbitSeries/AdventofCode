#include "bits/stdc++.h"
#include <termios.h>
using namespace std;
#define WALL -1
#define EMPTY -2
#define BOT -3
#define BUF 1024
typedef struct pos {
    pos() {};
    pos( int a, int b ) : x( a ), y( b ) {}
    int x, y;
}pos;

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

void printGUI( vector<vector<int>> const pos2Id, pos const curPos ) {
    system( "clear" );
    vector<vector<int>> GUI = pos2Id;
    GUI[curPos.x][curPos.y] = BOT;
    for( auto line : GUI ) {
        for( auto id : line ) {
            if( id >= 0 )
                cout << 'O';
            else if( id == WALL ) {
                cout << '#';
            }
            else if( id == EMPTY ) {
                cout << ' ';
            }
            else
                cout << '@';
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

bool isBox( vector<vector<int>> const& pos2Id, pos point ) {
    return pos2Id[point.x][point.y] >= 0;
}

void pushBox( vector<pos>& id2Pos, vector<vector<int>>& pos2Id, pos& curPos, char control ) {
    // printGUI( pos2Id, curPos );
    stack<pos> boxPath;
    pos nextPos = getNextPos( control, curPos );
    while( isBox( pos2Id, nextPos ) ) {
        boxPath.push( nextPos );
        nextPos = getNextPos( control, nextPos );
    }
    if( pos2Id[nextPos.x][nextPos.y] == EMPTY ) {
        while( !boxPath.empty() ) {
            // printGUI( pos2Id, curPos );
            pos preBox = boxPath.top();
            boxPath.pop();
            int preBoxId = pos2Id[preBox.x][preBox.y];
            id2Pos[preBoxId] = nextPos;
            pos2Id[nextPos.x][nextPos.y] = preBoxId;
            nextPos = preBox;
            // printGUI( pos2Id, curPos );
        }
        pos2Id[nextPos.x][nextPos.y] = EMPTY;
        curPos = nextPos;
        // printGUI( pos2Id, curPos );
    }
}

void play( vector<pos>& id2Pos, vector<vector<int>>& pos2Id, pos& curPos, queue<char> control ) {
    while( !control.empty() ) {
        char c = control.front();
        control.pop();
        // printGUI( pos2Id, curPos );
        pos nextPos = getNextPos( c, curPos );
        switch( pos2Id[nextPos.x][nextPos.y] ) {
        case WALL:
            break;
        case EMPTY:
            curPos = nextPos;
            break;
        default:
            // Push boxes
            pushBox( id2Pos, pos2Id, curPos, c );
            break;
        }
    }
}

void playMyself( vector<pos>& id2Pos, vector<vector<int>>& pos2Id, pos& curPos ) {
    printGUI( pos2Id, curPos );
    enableRawMode(); // 开启字符缓冲模式
    while( 1 ) {
        char c = getMoveMent();
        if( c == 0 ) {  // 按 'q' 退出
            break;
        }
        // printGUI( pos2Id, curPos );
        pos nextPos = getNextPos( c, curPos );
        switch( pos2Id[nextPos.x][nextPos.y] ) {
        case WALL:
            break;
        case EMPTY:
            curPos = nextPos;
            break;
        default:
            // Push boxes
            pushBox( id2Pos, pos2Id, curPos, c );
            break;
        }
        printGUI( pos2Id, curPos );
    }
    disableRawMode();
}

typedef long long ll;
ll sumCoordinates( vector<pos> const id2Pos, vector<vector<int>> const pos2Id ) {
    ll res = 0;
    for( auto line : pos2Id ) {
        for( auto id : line ) {
            if( id >= 0 )
                res += 100 * id2Pos[id].x + id2Pos[id].y;
        }
    }
    return res;
}

int main() {
    vector<pos> id2Pos;
    vector<vector<int>> pos2Id;
    char buf[BUF] = "\0";
    pos start;
    FILE* input = fopen( "input.txt", "r" );
    while( !feof( input ) && fgets( buf, BUF, input ) ) {
        vector<int> row;
        for( char c : buf ) {
            if( c != '\n' && c != '\0' ) {
                switch( c ) {
                case '#':
                    row.push_back( WALL );
                    break;
                case '.':
                    row.push_back( EMPTY );
                    break;
                case 'O':
                    row.push_back( id2Pos.size() );
                    id2Pos.push_back( pos( pos2Id.size(), row.size() - 1 ) );
                    break;
                case '@':
                    row.push_back( EMPTY );
                    start = pos( pos2Id.size(), row.size() - 1 );
                    break;
                default:
                    break;
                }
            }
        }
        pos2Id.push_back( row );
    }
    FILE* move = fopen( "move.txt", "r" );
    queue<char> control;
    while( !feof( move ) && fgets( buf, BUF, move ) ) {
        for( char c : buf ) {
            if( c != '\n' && c != '\0' ) { control.push( c ); }
        }
    }
    // playMyself( id2Pos, pos2Id, start,control );
    playMyself( id2Pos, pos2Id, start );
    cout << sumCoordinates( id2Pos, pos2Id ) << endl;
    return 0;
}