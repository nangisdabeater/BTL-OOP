#pragma once

/**
 * @file ServiceGroupMenu.h
 * @brief Lớp điều khiển giao diện Menu và các thao tác CRUD cho Nhóm Dịch Vụ (ServiceGroup)
 * Quản lý các thuộc tính: Mã ID, Tên nhóm dịch vụ, Mô tả, Trạng thái hoạt động.
 * Đảm bảo các ràng buộc: Kiểm tra rỗng, kiểm tra trùng tên, kiểm tra toàn vẹn dữ liệu khi xóa.
 */

#include "app.h"

using namespace std;

class ServiceGroupMenu
{
private:
    Repository<ServiceGroup>& serviceGroupRepository; // Tham chiếu đến kho lưu trữ Nhóm dịch vụ
    Repository<Ticket>& ticketRepository;             // Tham chiếu đến kho Ticket để kiểm tra ràng buộc khóa ngoại

    // Các hàm nội bộ phục vụ giao diện và nghiệp vụ
    void showMenu();          // Hiển thị danh mục các chức năng
    void displayAll();        // Hiển thị danh sách tất cả nhóm dịch vụ (Read)
    void findById();          // Tìm kiếm nhóm dịch vụ theo ID (Read)
    void add();               // Thêm mới nhóm dịch vụ (Create)
    void update();            // Cập nhật thông tin nhóm dịch vụ (Update)
    void remove();            // Xóa nhóm dịch vụ và kiểm tra ràng buộc (Delete)

    // Các hàm tiện ích hỗ trợ nhập liệu
    static string trim(const string& str);
    static string inputNonEmptyString(const string& prompt);

public:
    // Hàm khởi tạo nhận vào tham chiếu của 2 repository
    ServiceGroupMenu(Repository<ServiceGroup>& sgRepo, Repository<Ticket>& tRepo);

    // Hàm điều phối chạy vòng lặp menu
    void run();
};
