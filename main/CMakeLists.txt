# ==============================================================================
# File cấu hình CMake cho dự án Bài Tập Lớn Lập Trình Hướng Đối Tượng (BTL OOP)
# Đề tài: Quản lý Trung tâm Chăm sóc Khách hàng (CSKH)
# ==============================================================================

# Yêu cầu phiên bản CMake tối thiểu
cmake_minimum_required(VERSION 3.15)

# Tên dự án và ngôn ngữ sử dụng
project(BTL_OOP VERSION 1.0 LANGUAGES CXX)

# Thiết lập chuẩn C++11 cho trình biên dịch
set(CMAKE_CXX_STANDARD 11)
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
    FileManager/ServiceGroupFileManager.cpp
    FileManager/TicketFileManager.cpp
    FileManager/AgentFileManager.cpp
    Utilities/Utilities.cpp
    # [PHẦN THÊM MỚI] Các file thực thể Kênh tiếp nhận và Cuộc gọi tiếp nhận
    Entity/ReceptionChannel.cpp
    Entity/InboundCall.cpp
    # [PHẦN THÊM MỚI] Các file quản lý file cho Kênh tiếp nhận và Cuộc gọi tiếp nhận
    FileManager/ReceptionChannelFileManager.cpp
    FileManager/InboundCallFileManager.cpp
    # [PHẦN THÊM MỚI] Các file menu điều khiển giao diện
    app/ReceptionChannelMenu.cpp
    app/InboundCallMenu.cpp
    # [PHẦN MỞ RỘNG] Các module Mức SLA, Phiếu chuyển xử lý và Ca trực
    Entity/SLALevel.cpp
    Entity/TransferTicket.cpp
    Entity/AgentShift.cpp
    FileManager/SLALevelFileManager.cpp
    FileManager/TransferTicketFileManager.cpp
    FileManager/AgentShiftFileManager.cpp
    app/SLALevelMenu.cpp
    app/TransferTicketMenu.cpp
    app/AgentShiftMenu.cpp
    # [PHAN MO RONG] Quan ly Khach hang, Danh gia hai long va Tien ich ngay thang
    Entity/Customer.cpp
    Entity/Feedback.cpp
    FileManager/CustomerFileManager.cpp
    FileManager/FeedbackFileManager.cpp
    app/CustomerMenu.cpp
    app/FeedbackMenu.cpp
)

# Tạo file thực thi 'myprogram' (trên Windows sẽ tự động sinh myprogram.exe)
add_executable(myprogram ${SOURCES})
