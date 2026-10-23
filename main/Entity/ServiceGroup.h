#pragma once

#include <string>
#include "BaseEntity.h"

// Kế thừa để tái sử dụng thuộc tính id và các hàm getId(), setId() của lớp cha
class ServiceGroup : public BaseEntity {
private:
//Đặt tất cả thuộc tính ở phạm vi private để đảm bảo tính Đóng gói:bên ngoài không thể sửa trực tiếp mà phải đi qua các hàm setter có kiểm tra hợp lệ.
    std::string name;        // Tên nhóm dịch vụ (không được rỗng, tối đa 100 ký tự)
    std::string description; // Mô tả chi tiết dịch vụ
    std::string department;  // Phòng ban phụ trách (không được rỗng)
    int slaHours;            // Thời gian cam kết xử lý tiêu chuẩn tính theo giờ (> 0)
    int baseFee;             // Phí dịch vụ cơ bản tính bằng VNĐ (>= 0)
    std::string createdDate; // Ngày ban hành dịch vụ theo định dạng DD/MM/YYYY
    bool active;             // Trạng thái hoạt động (true: Đang hoạt động, false: Tạm ngưng)

public:
//Hàm khởi tạo có tham số mặc định
    ServiceGroup(
        const std::string& name = "",
        const std::string& description = "",
        const std::string& department = "Phong CSKH",
        int slaHours = 24,
        int baseFee = 0,
        const std::string& createdDate = "01/01/2026"
    );

    const std::string& getName() const;
    const std::string& getDescription() const;
    const std::string& getDepartment() const;
    int getSlaHours() const;
    int getBaseFee() const;
    const std::string& getCreatedDate() const;
    bool isActive() const;

    // Các hàm Setter dùng để kiểm tra điều kiện hợp lệ trước khi gán giá trị mới vào thuộc tính
    void setName(const std::string& name);
    void setDescription(const std::string& description);
    void setDepartment(const std::string& department);
    void setSlaHours(int slaHours);
    void setBaseFee(int baseFee);
    void setCreatedDate(const std::string& createdDate);

    // Dùng từ khóa "override" để trình biên dịch kiểm tra chắc chắn hàm này ghi đè đúng
    // phương thức thuần ảo printInfo() của lớp cha BaseEntity, tránh lỗi gõ sai tên hàm.
    void printInfo() const override;

    void activate();   // Bật trạng thái hoạt động của nhóm dịch vụ (active = true)
    void deactivate(); // Tạm ngưng nhóm dịch vụ (active = false), không cho tạo ticket mới vào dịch vụ này
};

// Đặt tên bí danh ServiceManager tương đương với ServiceGroup để đồng bộ tên gọi với sơ đồ Use Case
using ServiceManager = ServiceGroup;
