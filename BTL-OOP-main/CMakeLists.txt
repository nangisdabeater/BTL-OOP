# ==============================================================================
# File cấu hình CMake cho dự án Bài Tập Lớn Lập Trình Hướng Đối Tượng (BTL OOP)
# Đề tài: Quản lý Trung tâm Chăm sóc Khách hàng (CSKH)
# ==============================================================================

# Yêu cầu phiên bản CMake tối thiểu
cmake_minimum_required(VERSION 3.15)

# Tên dự án và ngôn ngữ sử dụng
project(BTL_OOP VERSION 1.0 LANGUAGES CXX)

# Thiết lập chuẩn C++17 cho trình biên dịch
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Khai báo các thư mục chứa file header (.h)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/app
    ${CMAKE_CURRENT_SOURCE_DIR}/Entity
    ${CMAKE_CURRENT_SOURCE_DIR}/Repository
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities
    ${CMAKE_CURRENT_SOURCE_DIR}/FileManager
)

# Danh sách toàn bộ các file mã nguồn (.cpp) của dự án
set(SOURCES
    main.cpp
    app/App.cpp
    app/ServiceGroupMenu.cpp
    app/TicketMenu.cpp
    app/AgentMenu.cpp
    Entity/ServiceGroup.cpp
    Entity/Ticket.cpp
    Entity/Agent.cpp
    FileManager/ServiceGroupFileMenuFile.cpp
    FileManager/TicketMenuFile.cpp
    FileManager/AgentFileManager.cpp
    Utilities/Utilities.cpp
)

# Tạo file thực thi 'myprogram' (trên Windows sẽ tự động sinh myprogram.exe)
add_executable(myprogram ${SOURCES})
