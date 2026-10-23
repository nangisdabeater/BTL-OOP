#pragma once 

#include <string>
#include <vector>
#include "BaseEntity.h"

class AgentShift : public BaseEntity {
private: 
    std::string shiftName;              // Tên ca trực (VD: Ca sáng, Ca chiều, Ca đêm)
    std::string shiftDate;              // Ngày diễn ra ca trực (DD/MM/YYYY)
    std::string shiftStart;             // Giờ bắt đầu ca trực (HH:MM)
    std::string shiftEnd;               // Giờ kết thúc ca trực (HH:MM)
    // Dùng vector<std::string> để lưu danh sách mã nhân viên (khóa ngoại) thay vì lưu mảng các đối tượng Agent:
    // Giúp một nhân viên có thể tham gia nhiều ca trực khác nhau mà không bị trùng lặp dữ liệu nhân viên.
    std::vector<std::string> agentIds;  // Danh sách mã điện thoại viên được phân công vào ca
    bool status;                        // Trạng thái ca trực (true: Hoạt động, false: Tạm ngưng)

public:
    AgentShift(
        const std::string& shiftName = "",
        const std::string& shiftDate = "",
        const std::string& shiftStart = "",
        const std::string& shiftEnd = ""
    );

    // Các hàm Getter trả về tham chiếu hằng (const&) giúp tránh việc sao chép chuỗi hoặc vector
    const std::string& getShiftName() const;
    const std::string& getShiftDate() const;
    const std::string& getShiftStart() const;
    const std::string& getShiftEnd() const;
    const std::vector<std::string>& getAgentIds() const;
    bool isActive() const;

    // Các hàm Setter cập nhật dữ liệu kèm kiểm tra hợp lệ
    void setShiftName(const std::string& shiftName);
    void setShiftDate(const std::string& shiftDate);
    void setShiftStart(const std::string& shiftStart);
    void setShiftEnd(const std::string& shiftEnd);
    void setAgentIds(const std::vector<std::string>& agentIds);

    // Thêm một mã nhân viên vào ca trực (có kiểm tra không cho thêm trùng lặp)
    void addAgentId(const std::string& agentId);
    // Gỡ một mã nhân viên ra khỏi ca trực
    void removeAgentId(const std::string& agentId);

    // Ghi đè phương thức thuần ảo của lớp cha BaseEntity để in thông tin ca trực
    void printInfo() const override;

    void activate();   // Bật trạng thái hoạt động cho ca trực
    void deactivate(); // Tạm ngưng ca trực
};
