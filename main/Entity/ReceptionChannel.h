#pragma once

/**
 * @file ReceptionChannel.h
 * @brief Định nghĩa thực thể Kênh tiếp nhận (ReceptionChannel)
 * [PHẦN THÊM MỚI]
 * Đại diện cho một kênh tiếp nhận liên lạc của trung tâm CSKH (Hotline, Email, Facebook, Zalo,...)
 * Thuộc tính:
 *   - Mã ID: Kế thừa BaseEntity, định dạng tự động CN001, CN002,...
 *   - Tên kênh tiếp nhận (name)
 *   - Mô tả kênh tiếp nhận (description)
 *   - Trạng thái hoạt động (active: true/false)
 */

#include <string>
#include "BaseEntity.h"


class ReceptionChannel : public BaseEntity
{
private:
    std::string name;         // Tên kênh tiếp nhận (VD: Hotline 1900, Fanpage Facebook, Email CSKH,...)
    std::string description;  // Mô tả kênh tiếp nhận
    bool active;         // Trạng thái (true: Đang hoạt động, false: Tạm ngưng)

public:
    // Constructor (ID sẽ được Repository cấp tự động dạng CNxxx khi gọi add)
    ReceptionChannel(
        const std::string& name = "",
        const std::string& description = ""
    );

    // Getter
    const std::string& getName() const;
    const std::string& getDescription() const;
    bool isActive() const;

    // Setter (có kiểm tra dữ liệu hợp lệ)
    void setName(const std::string& name);
    void setDescription(const std::string& description);

    // Triển khai phương thức thuần ảo từ BaseEntity để in thông tin ra màn hình
    void printInfo() const override;

    // Các phương thức nghiệp vụ quản lý trạng thái
    void activate();
    void deactivate();
};
