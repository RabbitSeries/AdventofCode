#include "box.h"
int opId = 0;
vector<boxPos> getNextBoxVertical( vector<boxPos> const id2BoxPos, vector<vector<int>> const arcade, vector<boxPos> const curLevel, char const control, bool& isBlocked ) {
    map<int, int> nextLevelID;
    vector<boxPos> nextLevel;
    for( int i = 0; i < curLevel.size() && !isBlocked; i++ ) {
        boxPos curBox = curLevel[i];
        {
            pos l = getNextPos( control, curBox.l ), r = getNextPos( control, curBox.r );
            int nextIDL = arcade[l.x][l.y], nextIDR = arcade[r.x][r.y];
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

vector<boxPos> getNextBoxHorizental( vector<boxPos> const id2BoxPos, vector<vector<int>> const  arcade, vector<boxPos> const curLevel, char const control, bool& isBlocked ) {
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
    if( isBox( arcade, nextBoxPos ) ) {
        nextLevel.push_back( getBoxPosAtPos( id2BoxPos, arcade, nextBoxPos ) );

    }
    else if( isWALL( arcade, nextBoxPos ) ) {
        isBlocked = true;

    }
    else {
        assert( nextLevel.size() == 0 );
    }
    return nextLevel;

}

void pushBoxVertical( vector<boxPos> const boxList, vector<boxPos>& id2BoxPos, vector<vector<int>>& arcade, char const control ) {
    for( boxPos b : boxList ) {
        pos l = b.l, r = b.r;
        int boxId = arcade[l.x][l.y];
        boxPos& curBox = id2BoxPos[boxId];
        curBox.l = getNextPos( control, l );
        curBox.r = getNextPos( control, r );
        arcade[l.x][l.y] = CELLEMPTY;
        arcade[r.x][r.y] = CELLEMPTY;
        arcade[curBox.l.x][curBox.l.y] = boxId;
        arcade[curBox.r.x][curBox.r.y] = boxId;
    }
}

void pushBoxHorizontal( vector<boxPos>& boxList, vector<boxPos>& id2BoxPos, vector<vector<int>>& arcade, char const control ) {
    assert( boxList.size() == 1 );
    for( boxPos b : boxList ) {
        pos l = b.l, r = b.r;
        int boxId = arcade[l.x][l.y];
        boxPos& curBox = id2BoxPos[boxId];
        curBox.l = getNextPos( control, l );
        curBox.r = getNextPos( control, r );
        if( control == '>' )
            arcade[l.x][l.y] = CELLEMPTY;
        else
            arcade[r.x][r.y] = CELLEMPTY;
        arcade[curBox.l.x][curBox.l.y] = boxId;
        arcade[curBox.r.x][curBox.r.y] = boxId;
    }
}

void pushBox( vector<boxPos>& id2BoxPos, vector<vector<int>>& arcade, pos& curPos, char control ) {
    // if( opId == 2579 )
        // printGUI( arcade, curPos );
    bool isBlocked = false;
    stack<vector<boxPos>> boxPath;
    vector<boxPos> curLevel;
    // NextPos of start
    pos nextPos = getNextPos( control, curPos );
    curLevel.push_back( getBoxPosAtPos( id2BoxPos, arcade, nextPos ) );
    boxPath.push( curLevel );
    while( !isBlocked && !curLevel.empty() ) {
        vector<boxPos> nextLevel;
        if( control == 'v' || control == '^' ) {
            nextLevel = getNextBoxVertical( id2BoxPos, arcade, curLevel, control, isBlocked );
        }
        else {
            nextLevel = getNextBoxHorizental( id2BoxPos, arcade, curLevel, control, isBlocked );

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
                // printGUI( arcade, curPos );
            vector<boxPos> preBoxList = boxPath.top();
            boxPath.pop();
            if( countWall( arcade ) != 758 ) {
                printGUI( arcade, curPos );

            }
            if( control == '^' || control == 'v' ) {
                pushBoxVertical( preBoxList, id2BoxPos, arcade, control );
            }
            else {
                pushBoxHorizontal( preBoxList, id2BoxPos, arcade, control );
            }
            // if( opId == 2578 )
                // printGUI( arcade, curPos );
        }
        curPos = nextPos;
        // if( opId == 2578 )
            // printGUI( arcade, curPos );
    }
}

bool freshGUI( vector<boxPos>& id2BoxPos, vector<vector<int>>& arcade, pos& curPos ) {
    char c = getMoveMent();
    if( c == 'q' ) {  // q exit
        return false;
    }
    // printGUI( arcade, curPos );
    pos nextPos = getNextPos( c, curPos );
    switch( arcade[nextPos.x][nextPos.y] ) {
    case CELLWALL:
        break;
    case CELLEMPTY:
        curPos = nextPos;
        break;
    default:
        // Push boxes
        pushBox( id2BoxPos, arcade, curPos, c );
        break;
    }
    return true;
}

void play( vector<boxPos>& id2BoxPos, vector<vector<int>>& arcade, pos& curPos, queue<char> control ) {
    // printGUI( arcade, curPos );
    while( !control.empty() ) {
        char c = control.front();
        control.pop();
        // printGUI( arcade, curPos );
        pos nextPos = getNextPos( c, curPos );
        switch( arcade[nextPos.x][nextPos.y] ) {
        case CELLWALL:
            break;
        case CELLEMPTY:
            curPos = nextPos;
            break;
        default:
            // Push boxes
            pushBox( id2BoxPos, arcade, curPos, c );
            break;
        }
        opId++;
        // printGUI( arcade, curPos );
    }
    // printGUI( arcade, curPos );
}

void playMyself( vector<boxPos>& id2BoxPos, vector<vector<int>>& arcade, pos& curPos ) {
    printGUI( arcade, curPos );
    // enableRawMode();
    while( 1 ) {
        if( freshGUI( id2BoxPos, arcade, curPos ) )
            printGUI( arcade, curPos );
        else return;
    }
    // disableRawMode();
}




int main() {
    vector<boxPos> id2BoxPos;
    vector<vector<int>> arcade;
    pos start;
    readMap( arcade, id2BoxPos, start );
    ifstream move( "move.txt" );
    queue<char> control;
    string buf = "\0";
    while( getline( move, buf ) ) {
        for( char c : buf ) {
            if( c != '\n' && c != '\0' ) { control.push( c ); }
        }
    }
    // Error: 1472388
    // cout << sumCoordinates( id2BoxPos, arcade ) << endl;
    // printGUI( arcade, start );
    play( id2BoxPos, arcade, start, control );
    // playMyself( id2BoxPos, arcade, start );
    printGUI( arcade, start );
    cout << sumCoordinates( id2BoxPos, arcade ) << endl;
    return 0;
}