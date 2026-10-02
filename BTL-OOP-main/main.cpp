/**
 * @file main.cpp
 * @brief Điểm khởi đầu của chương trình Quản lý Trung tâm CSKH
 * Khởi tạo đối tượng Application và kích hoạt luồng chạy ứng dụng.
 */

#include "App/App.h"

using namespace std;

int main()
{
    // Khởi tạo đối tượng ứng dụng chính
    Application app;

    // Chạy vòng lặp menu chính của chương trình
    app.run();

    return 0;
}