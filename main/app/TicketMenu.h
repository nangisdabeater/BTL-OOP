#pragma once

#include <string>
#include "../Repository/Repository.h"
#include "../Entity/Ticket.h"
#include "../Entity/ServiceGroup.h"

class TicketMenu {
private:
    Repository<Ticket>& ticketRepository;
    Repository<ServiceGroup>& serviceGroupRepository;

    // Đặt các hàm chức năng ở private để đóng gói luồng xử lý nội bộ, bên ngoài chỉ gọi run()
    void showMenu();   // Hiển thị danh sách các chức năng của menu Ticket
    void displayAll(); // Hiển thị toàn bộ danh sách Ticket dạng bảng
    void findById();   // Tìm kiếm và xem chi tiết Ticket theo mã ID
    void add();        // Tạo mới Ticket (kèm kiểm tra khóa ngoại ServiceGroup)
    void update();     // Cập nhật thông tin và trạng thái xử lý của Ticket
    void remove();     // Xóa Ticket (chặn không cho xóa nếu Ticket đang ở trạng thái IN_PROGRESS)

    // Dùng từ khóa "static" cho 2 hàm chuyển đổi chuỗi vì chúng chỉ xử lý dữ liệu đầu vào rồi trả về chuỗi,
    // không cần truy cập vào các thuộc tính của lớp TicketMenu.
    static std::string ticketStatusToString(TicketStatus status);
    static std::string priorityToString(int priority);

public:
    // Khởi tạo menu với tham chiếu tới kho Ticket và kho Nhóm dịch vụ
    TicketMenu(Repository<Ticket>& tRepo, Repository<ServiceGroup>& sgRepo);

    // Chạy vòng lặp điều khiển menu Ticket
    void run();
};
