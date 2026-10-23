#pragma once

#include "../Entity/Customer.h"
#include "../Entity/Feedback.h"
#include "../Repository/Repository.h"

class CustomerMenu {
private:
    // Tham chiếu trực tiếp tới kho dữ liệu Khách hàng gốc
    Repository<Customer>& repo;
    // Tham chiếu tới kho Phiếu đánh giá hài lòng để kiểm tra ràng buộc toàn vẹn trước khi xóa khách hàng
    Repository<Feedback>& feedbackRepo;

    void showMenu();   // Hiển thị danh mục các chức năng quản lý khách hàng
    void displayAll(); // Hiển thị toàn bộ danh sách khách hàng dạng bảng
    void findById();   // Tìm kiếm khách hàng theo mã ID
    void add();        // Thêm mới khách hàng và tự động cấp mã KHxxx
    void update();     // Cập nhật thông tin khách hàng
    void remove();     // Xóa khách hàng (chặn xóa nếu khách hàng đang có phiếu đánh giá liên kết)

public:
    CustomerMenu(Repository<Customer>& r, Repository<Feedback>& fbRepo);

    void run();        // Chạy vòng lặp điều khiển menu Khách hàng
};
