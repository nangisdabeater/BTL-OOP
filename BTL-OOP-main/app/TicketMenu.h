#pragma once

/**
 * @file TicketMenu.h
 * @brief Lớp điều khiển giao diện Menu và các thao tác CRUD cho Ticket
 * Quản lý các thuộc tính: Mã Ticket, Tiêu đề, Mô tả, Mã khách hàng, Mã nhóm dịch vụ, Trạng thái.
 * Đảm bảo các ràng buộc: Kiểm tra rỗng, kiểm tra khóa ngoại ServiceGroupId, trạng thái hoạt động của nhóm dịch vụ.
 */

#include "App.h"

using namespace std;

class TicketMenu
{
private:
    Repository<Ticket>& ticketRepository;             // Tham chiếu đến kho lưu trữ Ticket
    Repository<ServiceGroup>& serviceGroupRepository; // Tham chiếu đến kho Nhóm dịch vụ (để kiểm tra khóa ngoại)

    // Các hàm nội bộ phục vụ giao diện và nghiệp vụ
    void showMenu();          // Hiển thị danh mục chức năng quản lý Ticket
    void displayAll();        // Hiển thị danh sách tất cả ticket (Read All)
    void findById();          // Tìm kiếm ticket theo ID (Read by ID)
    void add();               // Tạo mới ticket (Create)
    void update();            // Cập nhật thông tin và trạng thái ticket (Update)
    void remove();            // Xóa ticket (Delete)

    // Các hàm tiện ích hỗ trợ
    static string trim(const string& str);
    static string inputNonEmptyString(const string& prompt);
    static string ticketStatusToString(TicketStatus status);

public:
    // Hàm khởi tạo nhận vào tham chiếu của 2 repository
    TicketMenu(Repository<Ticket>& tRepo, Repository<ServiceGroup>& sgRepo);

    // Hàm điều phối chạy vòng lặp menu
    void run();
};
