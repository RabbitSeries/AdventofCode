#include <iostream>
#include <iomanip> // for std::fixed and std::setprecision
#include <thread>  // for std::this_thread::sleep_for
#include <chrono>  // for std::chrono::milliseconds
void showProgressBar(int processCnt, int pathCnt) {
    system("clear");
    const int barWidth = 50; // 进度条的宽度
    double progress = static_cast<double>(processCnt) / pathCnt;

    // 显示百分比和进度条
    std::cout << "\rProcessing " << processCnt << " / " << pathCnt << " [";
    int pos = static_cast<int>(barWidth * progress);
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos)
            std::cout << "="; // 已完成部分
        else if (i == pos)
            std::cout << ">"; // 当前进度指针
        else
            std::cout << " "; // 未完成部分
    }
    std::cout << "] " << std::fixed << std::setprecision(2) << (progress * 100.0) << "%" << std::endl;
    // std::cout.flush(); // 刷新输出
}
