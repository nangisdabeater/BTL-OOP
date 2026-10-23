#pragma once

#include <string>
#include "BaseEntity.h"

// Kế thừa từ BaseEntity để quản lý thống nhất mã ID (CNxxx) cho từng kênh tiếp nhận
class ReceptionChannel : public BaseEntity {
private:
    std::string name;        // Tên kênh tiếp nhận (VD: Hotline 1900, Fanpage Facebook, Email CSKH,...)
    std::string description; // Mô tả chi tiết về kênh tiếp nhận
    bool active;             // Trạng thái hoạt động (true: Đang hoạt động, false: Tạm ngưng)

public:
    // Hàm khởi tạo có tham số mặc định (ID sẽ được Repository cấp tự động dạng CNxxx khi gọi add)
    ReceptionChannel(
        const std::string& name = "",
        const std::string& description = ""
    );

    // Các hàm Getter lấy thông tin kênh tiếp nhận (dùng const& để tránh copy chuỗi)
    const std::string& getName() const;
    const std::string& getDescription() const;
    bool isActive() const;

    // Các hàm Setter cập nhật thông tin kèm kiểm tra không được để trống
    void setName(const std::string& name);
    void setDescription(const std::string& description);

    // Ghi đè phương thức thuần ảo từ BaseEntity để in thông tin kênh tiếp nhận ra màn hình
    void printInfo() const override;

    void activate();   // Bật trạng thái hoạt động cho kênh tiếp nhận
    void deactivate(); // Ngưng hoạt động kênh tiếp nhận
};
