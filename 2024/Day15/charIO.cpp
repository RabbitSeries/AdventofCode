#include <iostream>
#include <termios.h>
#include <unistd.h>

void enableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);          // 获取当前终端设置
    term.c_lflag &= ~(ICANON | ECHO);        // 关闭规范模式和回显
    tcsetattr(STDIN_FILENO, TCSANOW, &term); // 立即生效
}

void disableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);         // 恢复规范模式和回显
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int main() {
    enableRawMode(); // 开启字符缓冲模式

    char c;
    while (true) {
        c = getchar();
        if (c == 'q') {  // 按 'q' 退出
            break;
        }
        std::cout << "You entered: " << c << std::endl;
    }

    disableRawMode(); // 恢复行缓冲模式
    return 0;
}
