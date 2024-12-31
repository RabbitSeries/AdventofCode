#include "bits/stdc++.h"
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

void printGUI( vector<vector<int>> const pos2Id, pos const curPos ) {
    system("clear");
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
            printGUI( pos2Id, curPos );
            pos preBox = boxPath.top();
            boxPath.pop();
            int preBoxId = pos2Id[preBox.x][preBox.y];
            id2Pos[preBoxId] = nextPos;
            pos2Id[nextPos.x][nextPos.y] = preBoxId;
            nextPos = preBox;
            printGUI( pos2Id, curPos );
        }
        pos2Id[nextPos.x][nextPos.y] = EMPTY;
        curPos = nextPos;
        printGUI( pos2Id, curPos );
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
    play( id2Pos, pos2Id, start, control );
    cout << sumCoordinates( id2Pos, pos2Id ) << endl;
}