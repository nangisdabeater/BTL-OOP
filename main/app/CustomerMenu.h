#pragma once

#include "../Entity/Customer.h"
#include "../Entity/Feedback.h"
#include "../Repository/Repository.h"

/**
 * @file CustomerMenu.h
 * @brief Giao diện Menu và các chức năng CRUD cho Thực thể Khách hàng (Customer)
 * 
 * Quản lý các thông tin: Mã ID (KHxxx), Họ và tên, Số điện thoại, Email, Địa chỉ.
 * Cung cấp các thao tác hiển thị danh sách dạng bảng, tìm kiếm, thêm mới, cập nhật và xóa.
 */
class CustomerMenu
{
private:
    Repository<Customer>& repo;              ///< Kho lưu trữ dữ liệu khách hàng
    Repository<Feedback>& feedbackRepo;      ///< Kho lưu trữ phiếu đánh giá để kiểm tra ràng buộc khi xóa

    /**
     * @brief Hiển thị bảng danh mục các chức năng quản lý khách hàng
     */
    void showMenu();

    /**
     * @brief Hiển thị toàn bộ danh sách khách hàng dưới dạng bảng có căn lề
     */
    void displayAll();

    /**
     * @brief Tìm kiếm và hiển thị thông tin chi tiết một khách hàng theo mã ID
     */
    void findById();

    /**
     * @brief Tiếp nhận thông tin và thêm một khách hàng mới vào hệ thống
     */
    void add();

    /**
     * @brief Cập nhật thông tin của một khách hàng hiện có
     */
    void update();

    /**
     * @brief Xóa một khách hàng khỏi hệ thống sau khi xác nhận
     */
    void remove();

public:
    /**
     * @brief Khởi tạo đối tượng CustomerMenu với kho dữ liệu khách hàng và đánh giá
     * @param r Tham chiếu đến Repository<Customer>
     * @param fbRepo Tham chiếu đến Repository<Feedback>
     */
    CustomerMenu(Repository<Customer>& r, Repository<Feedback>& fbRepo);

    /**
     * @brief Vòng lặp chính điều khiển giao diện menu khách hàng
     */
    void run();
};
