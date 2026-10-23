#pragma once

#include "../Repository/Repository.h"
#include "../Entity/SLALevel.h"
#include "../Entity/TransferTicket.h"

class SLALevelMenu {
private:
    // Tham chiếu trực tiếp tới kho lưu trữ Mức cam kết SLA
    Repository<SLALevel>& slaRepository;
    // Tham chiếu tới kho Phiếu chuyển xử lý để kiểm tra toàn vẹn dữ liệu trước khi xóa một mức SLA
    Repository<TransferTicket>& transferRepository;

    void showMenu();   // Hiển thị danh mục chức năng quản lý Mức SLA
    void displayAll(); // Hiển thị toàn bộ danh sách mức SLA
    void findById();   // Tìm kiếm mức SLA theo mã ID (SLxxx)
    void add();        // Thêm mức SLA mới (kiểm tra trùng tên, thời gian > 0)
    void update();     // Cập nhật thông tin mức SLA
    void remove();     // Xóa mức SLA (chặn xóa nếu đang có phiếu chuyển tuyến tham chiếu tới)

public:
    SLALevelMenu(Repository<SLALevel>& repo, Repository<TransferTicket>& tfRepo);
    void run();        // Chạy vòng lặp điều khiển menu Mức SLA
};
