#include "box.h"
int opId = 0;
vector<boxPos> getNextBoxVertical( vector<boxPos> const id2BoxPos, vector<vector<int>> const acrade, vector<boxPos> const curLevel, char const control, bool& isBlocked ) {
    map<int, int> nextLevelID;
    vector<boxPos> nextLevel;
    for( int i = 0; i < curLevel.size() && !isBlocked; i++ ) {
        boxPos curBox = curLevel[i];
        {
            pos l = getNextPos( control, curBox.l ), r = getNextPos( control, curBox.r );
            int nextIDL = acrade[l.x][l.y], nextIDR = acrade[r.x][r.y];
            if( nextIDL == CELLWALL || nextIDR == CELLWALL ) {
                isBlocked = true;
                nextLevel.clear();
                // Unable to move.
                return nextLevel;
            }
            // Avoid dulplicated eppend.
            if( nextIDL != CELLEMPTY )
                nextLevelID[nextIDL]++;
            if( nextIDR != CELLEMPTY )
                nextLevelID[nextIDR]++;
        }
    }
    for( auto m : nextLevelID ) {
        nextLevel.push_back( id2BoxPos[m.first] );
    }
    return nextLevel;
}

vector<boxPos> getNextBoxHorizental( vector<boxPos> const id2BoxPos, vector<vector<int>> const  acrade, vector<boxPos> const curLevel, char const control, bool& isBlocked ) {
    assert( curLevel.size() == 1 );
    vector<boxPos> nextLevel;
    boxPos curBox = curLevel[0];
    pos nextBoxPos;
    if( control == '<' ) {
        nextBoxPos = getNextPos( control, curBox.l );
    }
    else {
        nextBoxPos = getNextPos( control, curBox.r );
    }
    if( isBox( acrade, nextBoxPos ) ) {
        nextLevel.push_back( getBoxPosAtPos( id2BoxPos, acrade, nextBoxPos ) );

    }
    else if( isWALL( acrade, nextBoxPos ) ) {
        isBlocked = true;

    }
    else {
        assert( nextLevel.size() == 0 );
    }
    return nextLevel;

}

void pushBoxVertical( vector<boxPos> const boxList, vector<boxPos>& id2BoxPos, vector<vector<int>>& acrade, char const control ) {
    for( boxPos b : boxList ) {
        pos l = b.l, r = b.r;
        int boxId = acrade[l.x][l.y];
        boxPos& curBox = id2BoxPos[boxId];
        curBox.l = getNextPos( control, l );
        curBox.r = getNextPos( control, r );
        acrade[l.x][l.y] = CELLEMPTY;
        acrade[r.x][r.y] = CELLEMPTY;
        acrade[curBox.l.x][curBox.l.y] = boxId;
        acrade[curBox.r.x][curBox.r.y] = boxId;
    }
}

void pushBoxHorizontal( vector<boxPos>& boxList, vector<boxPos>& id2BoxPos, vector<vector<int>>& acrade, char const control ) {
    assert( boxList.size() == 1 );
    for( boxPos b : boxList ) {
        pos l = b.l, r = b.r;
        int boxId = acrade[l.x][l.y];
        boxPos& curBox = id2BoxPos[boxId];
        curBox.l = getNextPos( control, l );
        curBox.r = getNextPos( control, r );
        if( control == '>' )
            acrade[l.x][l.y] = CELLEMPTY;
        else
            acrade[r.x][r.y] = CELLEMPTY;
        acrade[curBox.l.x][curBox.l.y] = boxId;
        acrade[curBox.r.x][curBox.r.y] = boxId;
    }
}

void pushBox( vector<boxPos>& id2BoxPos, vector<vector<int>>& acrade, pos& curPos, char control ) {
    // if( opId == 2579 )
        // printGUI( acrade, curPos );
    bool isBlocked = false;
    stack<vector<boxPos>> boxPath;
    vector<boxPos> curLevel;
    // NextPos of start
    pos nextPos = getNextPos( control, curPos );
    curLevel.push_back( getBoxPosAtPos( id2BoxPos, acrade, nextPos ) );
    boxPath.push( curLevel );
    while( !isBlocked && !curLevel.empty() ) {
        vector<boxPos> nextLevel;
        if( control == 'v' || control == '^' ) {
            nextLevel = getNextBoxVertical( id2BoxPos, acrade, curLevel, control, isBlocked );
        }
        else {
            nextLevel = getNextBoxHorizental( id2BoxPos, acrade, curLevel, control, isBlocked );

        }
        if( !isBlocked ) {
            if( !nextLevel.empty() )
                boxPath.push( nextLevel );
            curLevel = nextLevel;
        }
        else {
            nextLevel.clear();
            curLevel.clear();
        }
    }
    if( !isBlocked ) {
        while( !boxPath.empty() ) {
            // if( opId == 2578 )
                // printGUI( acrade, curPos );
            vector<boxPos> preBoxList = boxPath.top();
            boxPath.pop();
            if( countWall( acrade ) != 758 ) {
                printGUI( acrade, curPos );

            }
            if( control == '^' || control == 'v' ) {
                pushBoxVertical( preBoxList, id2BoxPos, acrade, control );
            }
            else {
                pushBoxHorizontal( preBoxList, id2BoxPos, acrade, control );
            }
            // if( opId == 2578 )
                // printGUI( acrade, curPos );
        }
        curPos = nextPos;
        // if( opId == 2578 )
            // printGUI( acrade, curPos );
    }
}

bool freshGUI( vector<boxPos>& id2BoxPos, vector<vector<int>>& acrade, pos& curPos ) {
    char c = getMoveMent();
    if( c == 'q' ) {  // 按 'q' 退出
        return false;
    }
    // printGUI( acrade, curPos );
    pos nextPos = getNextPos( c, curPos );
    switch( acrade[nextPos.x][nextPos.y] ) {
    case CELLWALL:
        break;
    case CELLEMPTY:
        curPos = nextPos;
        break;
    default:
        // Push boxes
        pushBox( id2BoxPos, acrade, curPos, c );
        break;
    }
    return true;
}

void play( vector<boxPos>& id2BoxPos, vector<vector<int>>& acrade, pos& curPos, queue<char> control ) {
    // printGUI( acrade, curPos );
    while( !control.empty() ) {
        char c = control.front();
        control.pop();
        // printGUI( acrade, curPos );
        pos nextPos = getNextPos( c, curPos );
        switch( acrade[nextPos.x][nextPos.y] ) {
        case CELLWALL:
            break;
        case CELLEMPTY:
            curPos = nextPos;
            break;
        default:
            // Push boxes
            pushBox( id2BoxPos, acrade, curPos, c );
            break;
        }
        opId++;
        // printGUI( acrade, curPos );
    }
    // printGUI( acrade, curPos );
}

void playMyself( vector<boxPos>& id2BoxPos, vector<vector<int>>& acrade, pos& curPos ) {
    printGUI( acrade, curPos );
    enableRawMode(); // 开启字符缓冲模式
    while( 1 ) {
        if( freshGUI( id2BoxPos, acrade, curPos ) )
            printGUI( acrade, curPos );
        else return;
    }
    disableRawMode();
}




int main() {
    vector<boxPos> id2BoxPos;
    vector<vector<int>> acrade;
    char buf[BUF] = "\0";
    pos start;
    FILE* input = fopen( "input.txt", "r" );
    while( !feof( input ) && fgets( buf, BUF, input ) ) {
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
                    id2BoxPos.push_back( boxPos( pos( acrade.size(), row.size() - 2 ), pos( acrade.size(), row.size() - 1 ) ) );
                    break;
                case '@':
                    start = pos( acrade.size(), row.size() );
                    row.push_back( CELLEMPTY );
                    row.push_back( CELLEMPTY );
                    break;
                default:
                    break;
                }
            }
        }
        acrade.push_back( row );
    }
    FILE* move = fopen( "move.txt", "r" );
    queue<char> control;
    while( !feof( move ) && fgets( buf, BUF, move ) ) {
        for( char c : buf ) {
            if( c != '\n' && c != '\0' ) { control.push( c ); }
        }
    }
    // Error: 1472388
    // cout << sumCoordinates( id2BoxPos, acrade ) << endl;
    // printGUI( acrade, start );
    // play( id2BoxPos, acrade, start, control );
    playMyself( id2BoxPos, acrade, start );
    printGUI( acrade, start );
    cout << sumCoordinates( id2BoxPos, acrade ) << endl;
    return 0;
}