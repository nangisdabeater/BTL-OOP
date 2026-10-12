#pragma once

#include "../Entity/Feedback.h"
#include "../Entity/Customer.h"
#include "../Repository/Repository.h"

/**
 * @file FeedbackMenu.h
 * @brief Giao diện Menu và các chức năng CRUD cho Phiếu đánh giá hài lòng (Feedback)
 * 
 * Quản lý các thông tin: Mã ID (DGxxx), Mã khách hàng, Mã Ticket, Điểm đánh giá (1-5),
 * Ngày đánh giá (dd/mm/yyyy), Nội dung nhận xét.
 * Kiểm tra ràng buộc khóa ngoại (Mã khách hàng phải tồn tại).
 */
class FeedbackMenu
{
private:
    Repository<Feedback>& repo;       ///< Kho lưu trữ dữ liệu phiếu đánh giá
    Repository<Customer>& customers;  ///< Kho lưu trữ khách hàng để kiểm tra ràng buộc liên kết

    /**
     * @brief Hiển thị bảng danh mục các chức năng quản lý phiếu đánh giá
     */
    void showMenu();

    /**
     * @brief Hiển thị toàn bộ danh sách phiếu đánh giá dưới dạng bảng có căn lề
     */
    void displayAll();

    /**
     * @brief Tìm kiếm và hiển thị thông tin chi tiết phiếu đánh giá theo mã ID
     */
    void findById();

    /**
     * @brief Tiếp nhận thông tin và thêm một phiếu đánh giá mới (kiểm tra tồn tại khách hàng)
     */
    void add();

    /**
     * @brief Cập nhật điểm và nội dung đánh giá của phiếu hiện có
     */
    void update();

    /**
     * @brief Xóa một phiếu đánh giá khỏi hệ thống sau khi xác nhận
     */
    void remove();

public:
    /**
     * @brief Khởi tạo đối tượng FeedbackMenu với kho dữ liệu đánh giá và khách hàng
     * @param r Kho lưu trữ Feedback
     * @param c Kho lưu trữ Customer
     */
    FeedbackMenu(Repository<Feedback>& r, Repository<Customer>& c);

    /**
     * @brief Vòng lặp chính điều khiển giao diện menu đánh giá hài lòng
     */
    void run();
};
