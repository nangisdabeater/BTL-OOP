#pragma once

#include "../Entity/Feedback.h"
#include "../Entity/Customer.h"
#include "../Repository/Repository.h"

class FeedbackMenu {
private:
    // Tham chiếu trực tiếp tới kho lưu trữ Phiếu đánh giá hài lòng
    Repository<Feedback>& repo;
    // Tham chiếu tới kho Khách hàng để kiểm tra mã khách hàng (khóa ngoại) có tồn tại không khi tạo đánh giá mới
    Repository<Customer>& customers;

    void showMenu();   // Hiển thị danh mục chức năng quản lý phiếu đánh giá
    void displayAll(); // Hiển thị toàn bộ danh sách phiếu đánh giá dạng bảng
    void findById();   // Tìm kiếm phiếu đánh giá theo mã ID (DGxxx)
    void add();        // Thêm phiếu đánh giá mới (kiểm tra tồn tại mã khách hàng)
    void update();     // Cập nhật điểm số và nội dung nhận xét
    void remove();     // Xóa phiếu đánh giá khỏi hệ thống

public:
    FeedbackMenu(Repository<Feedback>& r, Repository<Customer>& c);

    void run();        // Chạy vòng lặp điều khiển menu Đánh giá hài lòng
};
