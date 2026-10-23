#pragma once

#include <string>
#include "../Repository/Repository.h"
#include "../Entity/TransferTicket.h"
#include "../Entity/SLALevel.h"

class TransferTicketMenu {
private:
    // Tham chiếu trực tiếp tới kho lưu trữ Phiếu chuyển tuyến xử lý
    Repository<TransferTicket>& transferRepository;
    // Tham chiếu tới kho Mức SLA để kiểm tra mã SLA (khóa ngoại) có tồn tại không khi tạo phiếu chuyển
    Repository<SLALevel>& slaRepository;

    void showMenu();   // Hiển thị danh mục chức năng quản lý Phiếu chuyển xử lý
    void displayAll(); // Hiển thị toàn bộ danh sách phiếu chuyển
    void findById();   // Tìm kiếm phiếu chuyển theo mã ID (TFxxx)
    void add();        // Tạo phiếu chuyển xử lý mới (kiểm tra khóa ngoại mã SLA)
    void update();     // Cập nhật thông tin và trạng thái phiếu chuyển
    void remove();     // Xóa phiếu chuyển (chặn xóa nếu phiếu đã hoàn tất COMPLETED)

    // Hàm tĩnh hỗ trợ chuyển trạng thái enum sang chuỗi hiển thị
    static std::string statusToString(TransferStatus s);
    // Tìm tên mức SLA dựa theo mã SLA để hiển thị kèm trên bảng
    std::string slaNameOf(const std::string& slaId);

public:
    TransferTicketMenu(
        Repository<TransferTicket>& tfRepo,
        Repository<SLALevel>& sRepo
    );
    void run();        // Chạy vòng lặp điều khiển menu Phiếu chuyển xử lý
};
