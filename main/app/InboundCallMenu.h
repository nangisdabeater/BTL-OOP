#pragma once

#include "../Repository/Repository.h"
#include "../Entity/InboundCall.h"
#include "../Entity/ReceptionChannel.h"

class InboundCallMenu {
private:
    // Tham chiếu trực tiếp tới kho lưu trữ Cuộc gọi tiếp nhận
    Repository<InboundCall>& callRepository;
    // Tham chiếu tới kho Kênh tiếp nhận để kiểm tra mã kênh (khóa ngoại) có tồn tại không khi ghi nhận cuộc gọi
    Repository<ReceptionChannel>& channelRepository;

    void showMenu();        // Hiển thị danh mục chức năng quản lý Cuộc gọi tiếp nhận
    void displayAll();      // Hiển thị toàn bộ danh sách cuộc gọi tiếp nhận
    void findById();        // Tìm kiếm cuộc gọi theo mã ID (CAxxx)
    void add();             // Ghi nhận cuộc gọi đến mới (kiểm tra khóa ngoại kênh tiếp nhận)
    void update();          // Cập nhật trạng thái và ghi chú cuộc gọi
    void filterByChannel(); // Lọc và thống kê danh sách cuộc gọi theo từng kênh tiếp nhận
    void remove();          // Xóa thông tin cuộc gọi khỏi hệ thống

public:
    InboundCallMenu(
        Repository<InboundCall>& cRepo,
        Repository<ReceptionChannel>& chRepo
    );

    void run();             // Chạy vòng lặp điều khiển menu Cuộc gọi tiếp nhận
};
