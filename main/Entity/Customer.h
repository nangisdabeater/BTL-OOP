#pragma once

#include "BaseEntity.h"
#include <string>

/**
 * @file Customer.h
 * @brief Lớp thực thể đại diện cho Khách hàng (Customer) trong hệ thống CSKH
 * Kế thừa từ lớp cơ sở trừu tượng BaseEntity
 */
class Customer : public BaseEntity
{
private:
    std::string fullName;  ///< Họ và tên đầy đủ của khách hàng
    std::string phone;     ///< Số điện thoại liên hệ
    std::string email;     ///< Địa chỉ hòm thư điện tử
    std::string address;   ///< Địa chỉ nơi cư trú

public:
    /**
     * @brief Khởi tạo đối tượng Khách hàng với các thông tin ban đầu
     * @param name Họ và tên khách hàng
     * @param phone Số điện thoại liên hệ
     * @param email Địa chỉ email
     * @param address Địa chỉ cư trú
     */
    Customer(const std::string& name = "",
             const std::string& phone = "",
             const std::string& email = "",
             const std::string& address = "");

    // --- Các phương thức lấy dữ liệu (Getters) ---
    const std::string& getFullName() const;
    const std::string& getPhone() const;
    const std::string& getEmail() const;
    const std::string& getAddress() const;

    // --- Các phương thức cập nhật dữ liệu (Setters với kiểm tra ràng buộc) ---
    void setFullName(const std::string& name);
    void setPhone(const std::string& phone);
    void setEmail(const std::string& email);
    void setAddress(const std::string& address);

    /**
     * @brief In thông tin chi tiết của khách hàng ra màn hình (Ghi đè phương thức thuần ảo)
     */
    void printInfo() const override;
};
