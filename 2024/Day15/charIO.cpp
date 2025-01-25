#include <iostream>
#include <termios.h>
#include <unistd.h>

void enableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);          // Obtain current terminal input mode.
    term.c_lflag &= ~(ICANON | ECHO);        // Disable standard mode and echo.
    tcsetattr(STDIN_FILENO, TCSANOW, &term); // Effective immediately.
}

void disableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);         // Restore standard mode and echo.
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int main() {
    enableRawMode(); // Eable raw mode.

    char c;
    while (true) {
        c = getchar();
        if (c == 'q') {  // Press 'q' to exit.
            break;
        }
        std::cout << "You entered: " << c << std::endl;
    }

    disableRawMode(); // Restore buffer moed.
    return 0;
}
