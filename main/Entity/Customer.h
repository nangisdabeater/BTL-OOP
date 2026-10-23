#pragma once

#include "BaseEntity.h"
#include <string>

// Kế thừa từ lớp cơ sở trừu tượng BaseEntity để quản lý thống nhất mã ID (KHxxx)
class Customer : public BaseEntity {
private:
    std::string fullName; // Họ và tên đầy đủ của khách hàng
    std::string phone;    // Số điện thoại liên hệ
    std::string email;    // Địa chỉ hòm thư điện tử
    std::string address;  // Địa chỉ nơi cư trú

public:
    // Hàm khởi tạo có tham số mặc định (ID sẽ được Repository tự động cấp khi thêm mới)
    Customer(const std::string& name = "",
             const std::string& phone = "",
             const std::string& email = "",
             const std::string& address = "");

    // Các phương thức lấy dữ liệu (Getters) trả về tham chiếu hằng để tối ưu bộ nhớ
    const std::string& getFullName() const;
    const std::string& getPhone() const;
    const std::string& getEmail() const;
    const std::string& getAddress() const;

    // Các phương thức cập nhật dữ liệu (Setters) kèm kiểm tra điều kiện hợp lệ
    void setFullName(const std::string& name);
    void setPhone(const std::string& phone);
    void setEmail(const std::string& email);
    void setAddress(const std::string& address);

    // Ghi đè phương thức thuần ảo của BaseEntity để in thông tin chi tiết khách hàng
    void printInfo() const override;
};
