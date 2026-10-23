#pragma once

#include "../Repository/Repository.h"
#include "../Entity/ReceptionChannel.h"
#include "../Entity/InboundCall.h"

class ReceptionChannelMenu {
private:
    // Tham chiếu trực tiếp tới kho lưu trữ Kênh tiếp nhận
    Repository<ReceptionChannel>& channelRepository;
    // Tham chiếu tới kho Cuộc gọi tiếp nhận để kiểm tra toàn vẹn dữ liệu trước khi xóa kênh
    Repository<InboundCall>& callRepository;

    void showMenu();     // Hiển thị danh mục chức năng quản lý Kênh tiếp nhận
    void displayAll();   // Hiển thị toàn bộ danh sách kênh tiếp nhận
    void findById();     // Tìm kiếm kênh tiếp nhận theo mã ID (CNxxx)
    void add();          // Thêm kênh tiếp nhận mới (kiểm tra trùng tên kênh)
    void update();       // Cập nhật tên và mô tả kênh tiếp nhận
    void toggleStatus(); // Bật hoặc tắt trạng thái hoạt động của kênh tiếp nhận
    void remove();       // Xóa kênh tiếp nhận (chặn xóa nếu kênh đã có cuộc gọi liên kết)

public:
    ReceptionChannelMenu(Repository<ReceptionChannel>& chRepo, Repository<InboundCall>& cRepo);

    void run();          // Chạy vòng lặp điều khiển menu Kênh tiếp nhận
};
