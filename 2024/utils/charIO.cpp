#ifdef UNIX
#include <iostream>
#include <termios.h>
#include <unistd.h>

void enableRawMode() {
    termios term;
    tcgetattr( STDIN_FILENO, &term );          // Obtain current terminal input mode.
    term.c_lflag &= ~( ICANON | ECHO );        // Disable standard mode and echo.
    tcsetattr( STDIN_FILENO, TCSANOW, &term ); // Effective immediately.
}

void disableRawMode() {
    termios term;
    tcgetattr( STDIN_FILENO, &term );
    term.c_lflag |= ( ICANON | ECHO );         // Restore standard mode and echo.
    tcsetattr( STDIN_FILENO, TCSANOW, &term );
}

#endif