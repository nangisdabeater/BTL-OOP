#pragma once

#include "../Repository/Repository.h"
#include "../Entity/ServiceGroup.h"
#include "../Entity/Ticket.h"

class ServiceGroupMenu {
private:
    Repository<ServiceGroup>& serviceGroupRepository;

    // Truyền thêm tham chiếu kho Ticket vào đây để kiểm tra ràng buộc khóa ngoại trước khi xóa một nhóm dịch vụ
    Repository<Ticket>& ticketRepository;

    // Đặt các hàm xử lý chức năng ở private vì bên ngoài chỉ cần gọi duy nhất hàm run() để chạy menu
    void showMenu();   // In bảng danh mục chức năng ra màn hình
    void displayAll(); // Hiển thị toàn bộ danh sách nhóm dịch vụ dạng bảng
    void findById();   // Tìm kiếm nhóm dịch vụ theo mã ID
    void add();        // Thêm nhóm dịch vụ mới (có kiểm tra trùng tên)
    void update();     // Cập nhật thông tin nhóm dịch vụ
    void remove();     // Xóa nhóm dịch vụ (có kiểm tra xem còn ticket nào đang gắn với dịch vụ này không)

public:
    // Hàm khởi tạo nhận tham chiếu của 2 kho lưu trữ từ lớp điều phối Application
    ServiceGroupMenu(Repository<ServiceGroup>& sgRepo, Repository<Ticket>& tRepo);

    // Chạy vòng lặp menu quản lý dịch vụ
    void run();
};
