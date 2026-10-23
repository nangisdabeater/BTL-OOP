#pragma once

#include <string>
#include "BaseEntity.h"

// Kế thừa từ BaseEntity để quản lý thống nhất mã ID (SLxxx) cho mức cam kết dịch vụ SLA
class SLALevel : public BaseEntity {
private:
    std::string name;       // Tên mức độ cam kết SLA (VD: SLA Tiêu chuẩn, SLA Khẩn cấp)
    int responseMinutes;    // Thời gian cam kết phản hồi tối đa tính bằng phút (> 0)
    int resolveHours;       // Thời gian cam kết xử lý hoàn tất tối đa tính bằng giờ (> 0)
    int priority;           // Mức độ ưu tiên từ 1 (Cao nhất) đến 5 (Thấp nhất)

public:
    // Hàm khởi tạo có tham số mặc định (ID sẽ được Repository tự động cấp khi thêm mới)
    SLALevel(const std::string& name = "", int responseMinutes = 15, int resolveHours = 24, int priority = 3);

    // Các hàm Getter lấy thông tin mức SLA
    const std::string& getName() const;
    int getResponseMinutes() const;
    int getResolveHours() const;
    int getPriority() const;

    // Các hàm Setter cập nhật dữ liệu kèm kiểm tra ràng buộc (ném ngoại lệ nếu sai điều kiện)
    void setName(const std::string& name);
    void setResponseMinutes(int minutes);
    void setResolveHours(int hours);
    void setPriority(int priority);

    // Ghi đè phương thức thuần ảo từ BaseEntity để hiển thị thông tin mức SLA
    void printInfo() const override;
};
