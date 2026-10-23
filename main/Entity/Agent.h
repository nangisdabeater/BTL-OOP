#pragma once 

#include <string>
#include "BaseEntity.h"

// Kế thừa từ BaseEntity để có sẵn thuộc tính id và bắt buộc định nghĩa hàm đa hình printInfo()
class Agent : public BaseEntity {
private:
    // Các thuộc tính đặt ở private để đảm bảo tính đóng gói, chỉ truy cập qua getter/setter
    std::string fullName;       // Họ và tên điện thoại viên
    std::string phoneNumber;    // Số điện thoại di động cá nhân
    std::string email;          // Địa chỉ email liên hệ
    std::string extensionPhone; // Số máy lẻ nội bộ tổng đài
    bool status;                // Trạng thái làm việc (true: Đang hoạt động, false: Tạm ngưng)

public: 
    // Hàm khởi tạo có tham số mặc định giúp linh hoạt khi tạo đối tượng mới
    Agent(
        const std::string &fullName = "",
        const std::string &phoneNumber = "",
        const std::string &email = "",
        const std::string &extensionPhone = ""
    );

    // Các hàm Getter trả về tham chiếu hằng (const&) để tránh copy chuỗi tốn bộ nhớ
    const std::string& getFullName() const;
    const std::string& getPhoneNumber() const;
    const std::string& getEmail() const;
    const std::string& getExtensionPhone() const;
    bool isActive() const;

    // Các hàm Setter kiểm tra dữ liệu đầu vào không được để trống trước khi gán
    void setFullName(const std::string& fullName);
    void setPhoneNumber(const std::string& phoneNumber);
    void setEmail(const std::string& email);
    void setExtensionPhone(const std::string& extensionPhone);

    // Ghi đè phương thức thuần ảo từ lớp cha BaseEntity để hiển thị thông tin điện thoại viên
    void printInfo() const override;

    void activate();   // Kích hoạt trạng thái đang hoạt động
    void deactivate(); // Chuyển sang trạng thái tạm ngưng
};
